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
  
    if (_serialPort->available() > 0) { //  Check for xio API Command Messages
        // Read the incoming bytes
        int blen = _serialPort->readBytesUntil(TERMINAL, buffer, BUFFER_SIZE);
        TokenState state = START_JSON;
        
        // Command & Value character count
        int index = 0;
        
        // Parse JSON Command String, extract command, value, and value type using a state machine.
        for(int i = 0; i < blen; i++) {
            switch(state) {
                case START_JSON:
                if (buffer[i] == STRING_DELIM) { 
                    state = START_CMD; 
                }
                break;
                case START_CMD:
                if (buffer[i] == STRING_DELIM) { 
                    state = END_CMD; 
                    _cmd[index] = NULL_TERMINATOR;
                    index = 0;
                }
                else {
                    _cmd[index] = buffer[i];
                    index++;
                }
                break;
                case END_CMD:
                if (buffer[i] == COLON) { state = START_VALUE; }
                break;
                case START_VALUE:
                if (index == 0) {  
                    // Check 1st char of value to detect type
                    _valueType = parseValueType(buffer[i]);
                }
                if (buffer[i] == END_OBJ) { 
                    state = END_VALUE; 
                    _value[index] = NULL_TERMINATOR;
                }
                else {
                    _value[index] = buffer[i];
                    index++;
                }
                break;
                default:
                break;
            }

        }
        return true;
    }
    else {
        _cmd[0] = NULL_TERMINATOR;
        _value[0] = NULL_TERMINATOR;
        return false;
    }
}

void xioAPI::handleCommand(char* cmdPtr) {
    // Serial.println(cmdPtr); // DEBUG
    uint32_t cmdHash = hash(cmdPtr);
    // Serial.println(cmdHash, HEX); // DEBUG

    using xioAPI_Protocol::APIKeyHashASCII;
    switch(cmdHash) {
        case PING:
            sendPing(Ping{"USB", "Thetis-F5", "001"});
            break;
        case NOTE:
            sendNotification(getValue());
            break;
        default:
            // char _buf[32];
            // sprintf(_buf, "Did not recognize key: %lu", cmdHash);
            // sendError(_buf);
            return;
    }
}

char* xioAPI::getCommand() {
    char *cmdPtr = _cmd;

    return cmdPtr;
}

char* xioAPI::getValue() {
    char *valuePtr = _value;

    return valuePtr;
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