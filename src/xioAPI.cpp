/******************************************************************
    @file       xioAPI.cpp
    @brief      Arduino-compatible implementation of the xioAPI for embedded systems
    @author     Braidan Duffy
    @copyright  MIT License

    Code:       Braidan Duffy
    Version:    1.0.0
    Date:       18/05/2023
    Modified:   18/05/2023

    CHANGELOG:
    v1.0.0 - Original release
    
    Credit - Derived from the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
******************************************************************/

#include "xioAPI.h"

xioAPI xioapi;

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;

bool xioAPI::begin(Stream* port) {
    _serialPort = port;
    _isActive = true;
    return true;
}


// ========================
// === HELPER FUNCTIONS ===
// ========================


ValueType xioAPI::parseValueType(char c) {
    switch(c) {
        case 'n':
            return JSON_NULL;
            break;
        case 't':
        case 'f':
            return JSON_BOOL;
            break;
        case '-': case '0': case '1': case '2': case '3': case '4': 
        case '5': case '6': case '7': case '8': case '9':
            return JSON_NUMBER;
            break;
        case '\"':
            return JSON_STRING;
            break;
        default:
            return JSON_UNDEFINED;
            break;
    }
}

bool xioAPI::checkForCommand() {
    char buffer[256];
    StaticJsonDocument<64> doc;

    if (_serialPort->available() > 0) { //  Check for xio API Command Messages
        // Read the incoming bytes
        int blen = _serialPort->readBytesUntil(TERMINAL, buffer, BUFFER_SIZE);

        DeserializationError error = deserializeJson(doc, buffer, blen);

        if (error) {
            return false;
        }

        JsonObject root = doc.as<JsonObject>();

        for (JsonPair kv : root) {
            _cmd = kv.key().c_str();
            _value = kv.value().as<const char *>();
        }

        return true;
    }
    else {
        // doc.clear();
        return false;
    }
}

void xioAPI::handleCommand(const char* cmdPtr) {
    // Serial.println(cmdPtr); // DEBUG
    uint32_t cmdHash = hash(cmdPtr);
    // Serial.println(cmdHash, HEX); // DEBUG

    using xioAPI_Protocol::APIKeyHashASCII;
    switch(cmdHash) {
        // Check for setting read/writes
        case SERIAL_NUMBER: case DEVICE_NAME:
            if (_value != nullptr) { // Assume that a WRITE command has been sent
                // TODO: Update settings and send back a message with the same JSON as received
            }
            // TODO: sendSetting(cmdPtr);
            break;
        case XIO_DEFAULT:
            // TODO: Set settings to factory defaults (TODOTODO: Make a factory default settings file)
            // and send back a message with the same JSON
            break;
        case APPLY:
            // TODO: ignore? Because I don't like??
            break;
        case SAVE:
            // TODO: Save all settings to config file and send back message with same received JSON
            break;
        case TIME:
            if (_value != nullptr) { // Assume that a WRITE command has been sent
                // TODO: Update RTC with the _value
            }
            // TODO: Send back a message with the current system time formatted in "YYYY-MM-DD hh:mm:ss"
            break;
        case PING:
            sendPing(Ping{"USB", deviceSettings.deviceName, deviceSettings.serialNumber});
            // TODO: determine which interface the command came from and add to ping message
            break;
        case RESET:
            // TODO: Return the message and reset the microcontroller
            break;
        case SHUTDOWN:
            // TODO: Return the message and shutdown the microcontroller
            break;
        case STROBE:
            strobe();
            break;
        case COLOUR:
            colour(getValue());
            break;
        case HEADING:
            // TODO: Update the AHRS heading value - IF ahrsMagnetometerIgnored is TRUE and return the message
            break;
        case xioAPI_Protocol::ACCESSORY:
            // TODO: Transmit data across the OTG serial bus and return the message
            break;
        case NOTE:
            sendNotification(getValue());
            break;
        case FORMAT:
            // TODO: Format the datalogging SD card and return the message
            break;
        case TEST:
            // TODO: Run a self-test routine and send back the response
            break;
        case BOOTLOADER:
            // TODO: Return the message and put the chip into BOOTLOADER mode
            break;
        case FACTORY:
            // TODO: set the device into a factory mode
            break;
        case ERASE:
            // TODO: Erase the config file and return the message
        default:
            char _buf[48];
            sprintf(_buf, "Did not recognize key: %lu", cmdHash);
            sendError(_buf);
    }
}

const char* xioAPI::getCommand() {
    return _cmd;
}

const char* xioAPI::getValue() {
    return _value;
}

ValueType xioAPI::getValueType() {
    return _valueType;
}


// =========================
// === UTILITY FUNCTIONS ===
// =========================


void xioAPI::print(const char *line) {
	if (_serialPort != nullptr) {
		_serialPort->print(line);
	}
}

void xioAPI::send(const char* message, ...) {
	char buffer[128];

	if (!_isActive) {
		return;
	}

	va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	print(buffer);
}

unsigned long xioAPI::hash(const char *str) {
    //  djb2 Hash Function
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}