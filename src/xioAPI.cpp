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

void xioAPI::checkForCommand() {
    char buffer[256];
    StaticJsonDocument<256> doc;

    while (_serialPort->available() > 0) { //  Check for xio API Command Messages
        // Read the incoming bytes
        int blen = _serialPort->readBytesUntil(TERMINAL, buffer, BUFFER_SIZE);
        DeserializationError error = deserializeJson(doc, buffer, blen);

        if (error) {
            Serial.println("Ran into error"); // DEBUG
            Serial.println(error.c_str()); // DEBUG
            return;
        }

        JsonObject root = doc.as<JsonObject>();

        for (JsonPair kv : root) {
            strcpy(_cmd, kv.key().c_str());
            _value = kv.value();
        }

        handleCommand(_cmd);
    }
}

void xioAPI::handleCommand(const char* cmdPtr) {
    uint32_t cmdHash = hash(cmdPtr);

    using xioAPI_Protocol::APIKeyHashASCII;

    // First check to see if it is a setting read/write command:
    const size_t numEntries = sizeof(settingTable) / sizeof(settingTable[0]);
    for (size_t i=0; i<numEntries; i++) { // Check all keys in the hash table
        if (settingTable[i].key == cmdHash) { // If the command key is present in the hash table, then it is a setting read/write
            if (_value.isNull()) { // If the passed value was null, then it is a read command
                sendSetting(cmdPtr, &settingTable[i]);
            }
            else {
                updateSetting(&settingTable[i], _value);
                sendSetting(cmdPtr, &settingTable[i]);
            }
            
            // Exit function after handle
            return;
        }
    }

    switch(cmdHash) {
        case XIO_DEFAULT:
            // TODO: Set settings to factory defaults (TODOTODO: Make a factory default settings file)
            // and send back a message with the same JSON
            break;
        case APPLY:
            // TODO: ignore? Because I don't like??
            ackApply();
            break;
        case SAVE:
            saveConfigurations();
            ackSave();
            break;
        case TIME:
            if (_value != nullptr) { // Assume that a WRITE command has been sent
                // TODO: Update RTC with the _value
            }
            // TODO: Send back a message with the current system time formatted in "YYYY-MM-DD hh:mm:ss"
            break;
        case PING:
            sendPing(Ping{"USB", settings.deviceName, settings.serialNumber});
            // TODO: determine which interface the command came from and add to ping message
            break;
        case RESET:
            // TODO: Return the message and reset the microcontroller
            break;
        case SHUTDOWN:
            // TODO: Return the message and shutdown the microcontroller
            break;
        case STROBE:
            cmdStrobe();
            break;
        case COLOUR:
            cmdColour(_value);
            break;
        case HEADING:
            if (settings.ahrsIgnoreMagnetometer) {
                // sendResponse(_cmd, _value);
            }
            else {
                cmdHeading(atof(_value));
            }
            break;
        case xioAPI_Protocol::ACCESSORY:
            // TODO: Transmit data across the OTG serial bus and return the message
            break;
        case NOTE:
            sendNotification(getValue<const char*>());
            break;
        case FORMAT:
            // TODO: Format the data logging SD card and return the message
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
            char _buf[128];
            sprintf(_buf, "Did not recognize key: %s as %lu", cmdPtr, cmdHash);
            sendError(_buf);
    }
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

void xioAPI::clearCmd() {
    memset(_cmd, 0, sizeof(_cmd));
}

void xioAPI::clearValue() {
    _value.clear();
}

unsigned long xioAPI::hash(const char *str) {
    //  djb2 Hash Function
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}