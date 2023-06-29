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

xioAPI api;

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;

/**
 * @brief Initializes the API for communication
 * 
 * @param port The Stream interface to be used for communication
*/
bool xioAPI::begin(Stream* port) {
    _serialPort = port;
    _isActive = true;
    return true;
}


// =========================
// === COMMAND FUNCTIONS ===
// =========================


/**
 * @brief Sends a device setting to the requestor
 * 
 * @param key The setting name (consult the xioAPI documentation for valid names)
 * @param entry The corresponding entry from the settings lookup table
*/
void xioAPI::sendSetting(const settingTableEntry* entry) {
    // Setting format: {"[setting]":[value]}\r\n

    StaticJsonDocument<256> _doc;
    char _out[256];

    const char* key = entry->key;
    SettingType _entryType = entry->type;
    void* _entryValue = entry->value;
    JsonArray _settingArray;
    const float* _array;

    switch (_entryType) {
        case BOOL:
            _doc[key] = *(bool*) _entryValue;
            break;
        case CHAR:
            _doc[key] = *(uint8_t*) _entryValue;
            break;
        case FLOAT:
            _doc[key] = *(float*) _entryValue;
            break;
        case INT:
            _doc[key] = *(int*) _entryValue;
            break;
        case FLOAT_ARRAY:
            _settingArray = _doc.createNestedArray(key);
            _array = static_cast<const float*>(_entryValue);
            for (size_t i=0; i<entry->len; i++) {
                _settingArray.add(_array[i]);
            }
            break;
        case CHAR_ARRAY:
            _doc[key] = (const char*) _entryValue;
            break;
        default:
            return;
    }

    serializeJson(_doc, _out);
    send(true, "%s", _out);
}

/**
 * @brief Sends a ping to the requestor containing device information.
 * 
 * @param ping A Ping structure containing the currently used interface, the device name, and the device serial number
*/
void xioAPI::sendPing(Ping ping) {
    /**
     * Ping format:
     * {
     * "ping": {
     *      "interface": "[interface]",
     *      "deviceName": "[deviceName]",
     *      "serialNumber": "[serialNumber]"
     * }
     * }\r\n
    */
    StaticJsonDocument<96> _doc;
    char _out[96];

    JsonObject root = _doc.createNestedObject("ping");
    root["interface"] = "USB";
    root["deviceName"] = "Thetis";
    root["serialNumber"] = "Unknown";

    serializeJson(_doc, _out);
    send(true, "%s", _out);
}

/**
 * @brief The network announcement message is used by a host to discover and connect to devices on the same network.
 * The message is continuously broadcast by the device on UDP port 10000 at 1 Hz.
 * 
 * @param na A NetworkAnnouncement structure containing the device sync word, name, serial number, IP address, TCP port, UDP send port, UDP receive port, network RSSI, battery percentage, and charging status.
*/
void xioAPI::sendNetworkAnnouncement(NetworkAnnouncement na) {
    /**
     * Network announcement format:
     * {
     * "sync": [sync],
     * "name": "[displayName]",
     * "sn": "[serialNumber]",
     * "ip": "[ipAddress]",
     * "port": [tcpPort],
     * "send": [udpSendPort],
     * "receive": [udpReceivePort],
     * "rssi": [wiFiRSSI]
     * "battery": [batteryPercentage],
     * "status": [chargingStatus]
     * }\r\n
    */
    StaticJsonDocument<256> _doc;
    char _out[256];

    _doc["sync"] = na.sync;
    _doc["name"] = na.displayName;
    _doc["sn"] = na.serialNumber;
    _doc["ip"] = na.ipAddress;
    _doc["port"] = na.tcpPort;
    _doc["send"] = na.udpSendPort;
    _doc["receive"] = na.udpReceivePort;
    _doc["rssi"] = na.rssiPercentage;
    _doc["battery"] = na.batteryPercentage;
    _doc["status"] = na.chargingStatus;

    serializeJson(_doc, _out);
    send(true, "%s", _out);
}

void xioAPI::sendSettingTable() {
    for (size_t i=0; i<SETTING_TABLE_SIZE; i++) {
        if (settingTable[i].key == nullptr) continue; // Skip sending the setting if the key (entry) is empty
        sendSetting(&settingTable[i]);
    }
}

void xioAPI::sendSettingFile() {
    serializeJsonPretty(_jsonConfigDoc, Serial);
}


// =================================
// === COMMAND HANDLER FUNCTIONS ===
// =================================


/**
 * @brief Continuously checks if a command is available from the device interface.
 * If a command is present, it will parse it into a command key and a value for later processing.
 * The command will automatically be processed as soon as it is detected.
*/
void xioAPI::checkForCommand() {
    char buffer[256];
    StaticJsonDocument<256> doc;

    while (_serialPort->available() > 0) { //  Check for xio API Command Messages
        // Read the incoming bytes
        int blen = _serialPort->readBytesUntil(TERMINAL, buffer, BUFFER_SIZE);
        DeserializationError error = deserializeJson(doc, buffer, blen);

        if (error) {
            _serialPort->println("Ran into error"); // DEBUG
            _serialPort->println(error.c_str()); // DEBUG
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

/**
 * @brief Handles the command based upon the key presented. 
*/
void xioAPI::handleCommand(const char* cmdPtr) {
    uint32_t cmdHash = hash(cmdPtr);

    using xioAPI_Protocol::APIKeyHashASCII;

    // First check to see if it is a setting read/write command:
    const size_t numEntries = sizeof(settingTable) / sizeof(settingTable[0]);
    for (size_t i=0; i<numEntries; i++) { // Check all keys in the hash table
        if (settingTable[i].hash == cmdHash) { // If the command key is present in the hash table, then it is a setting read/write
            if (_value.isNull()) { // If the passed value was null, then it is a read command
                sendSetting(&settingTable[i]);
            }
            else {
                updateSetting(&settingTable[i], _value);
                sendSetting(&settingTable[i]);
            }
            
            // Exit function after handle
            return;
        }
    }

    switch(cmdHash) {
        case XIO_DEFAULT:
            loadConfigurationsFromJSON(true, DEFAULT_CONFIG_FILE_NAME);
            break;
        case APPLY:
            // TODO: ignore? Settings are automatically set when configurations loaded
            sendAck("apply");
            break;
        case SAVE:
            saveConfigurations();
            sendAck("save");
            break;
        case TIME:
            if (_value != nullptr) { // Assume that a WRITE command has been sent
                cmdWriteTime();
            }
            cmdReadTime();
            break;
        case PING:
            sendPing(Ping{"USB", settings.deviceName, settings.serialNumber}); // TODO: Update to support multiple interfaces
            break;
        case RESET:
            sendAck("reset");
            cmdReset();
            break;
        case SHUTDOWN:
            sendAck("shutdown");
            cmdShutdown();
            break;
        case STROBE:
            cmdStrobe();
            break;
        case COLOUR:
            cmdColour();
            break;
        case HEADING:
            if (!settings.ahrsIgnoreMagnetometer) {
                cmdHeading();
            }
            send(true, "{heading:%f}", getValue<float>());
            break;
        case xioAPI_Protocol::ACCESSORY:
            cmdSerialAccessory();
            sendAck("serialAccessory");
            break;
        case NOTE:
            sendNotification(getValue<const char*>());
            break;
        case FORMAT:
            cmdFormat();
            sendAck("format");
            break;
        case TEST:
            cmdSelfTest();
            break;
        case BOOTLOADER:
            sendAck("bootloader");
            cmdBootloader();
            break;
        case FACTORY:
            cmdFactory();
            sendAck("factory");
            break;
        case ERASE:
            cmdErase();
            sendAck("erase");
            break;
        case READ_ALL:
            sendSettingTable();
            break;
        case READ_JSON:
            sendSettingFile();
            break;
        default:
            char _buf[128];
            sprintf(_buf, "Did not recognize key: %s as %08x", cmdPtr, cmdHash);
            sendError(_buf);
    }
}


// =========================
// === UTILITY FUNCTIONS ===
// =========================


/**
 * @brief Prints the provided message to the Stream interface
 * 
 * @param line The content to print to the interface
*/
void xioAPI::print(const char *line) {
	if (_serialPort != nullptr) {
		_serialPort->print(line);
	}
}

/**
 * @brief Formats a provided message based on identifiers and additional arguments.
 * See the documentation for snprintf() for additional information
 * 
 * @param writeLineFeed If the message should be followed by a line feed ('\\r\\n') or not
 * @param message The message to be written to the Stream interface as a buffer conforming to snprintf() standards
*/
void xioAPI::send(bool writeLineFeed, const char* message, ...) {
	char buffer[128];

	if (!_isActive) {
		return;
	}

	va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	print(buffer);

    if (writeLineFeed) {
        print("\r\n");
    }
}

/**
 * @brief Clears the value of the `_cmd` pointer 
*/
void xioAPI::clearCmd() {
    memset(_cmd, 0, sizeof(_cmd));
}

/**
 * @brief Clears the value of the `_value` object
*/
void xioAPI::clearValue() {
    _value.clear();
}

/**
 * @deprecated
 * @brief Determines what the type of a given character is based on the JSON standard.
 * 
 * @param c The character to check
 * 
 * @return the type of the given character in terms of the JSON standard
*/
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