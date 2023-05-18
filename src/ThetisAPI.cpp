/******************************************************************
    @file       ThetisAPI.cpp
    @brief      Arduino Library for the Thetis API based on the xio API
    @author     David Such
    @copyright  Please see the accompanying LICENSE file

    Code:       David Such
    Modified:   Braidan Duffy
    Version:    1.1.0
    Date:       13/04/23
    Modified:   17/05/2023

    1.0.0       Original Release.       13/04/23
    1.1.0       Restylized for Thetis   17/05/2023

    Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
            (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
            and using information in the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
            Uses the Madgwick Quaternion Class.

******************************************************************/

#include "ThetisAPI.h"
#include <math.h>

namespace ThetisAPI {
bool begin(Print* port) {
    _serialPort = port;
    return true;
}


// ========================
// === HELPER FUNCTIONS ===
// ========================


const unsigned long hash(const char *str) {
    //  djb2 Hash Function
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

ValueType parseValueType(char c) {
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

void checkForCommand() {
    char buffer[BUFFER_SIZE];
  
    if (_serialPort.available() > 0) { //  Check for xio API Command Messages
        // Read the incoming bytes
        int blen = _serialPort.readBytesUntil(TERMINAL, buffer, BUFFER_SIZE);
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
    }
    else {
        _cmd[0] = NULL_TERMINATOR;
        _value[0] = NULL_TERMINATOR;
    }
}

bool newCommand() {
  if (_cmd[0] == NULL_TERMINATOR) {
    return false;
  }

  return true;
}

char* getCommand() {
  char *cmdPtr = _cmd;

  return cmdPtr;
}

char* getValue() {
  char *valuePtr = _value;

  return valuePtr;
}

ValueType getValueType() {
  return _valueType;
}


// =========================
// === UTILITY FUNCTIONS ===
// =========================


void print(const char *line) {
	if (_serialPort != nullptr) {
		_serialPort->print(line);
	}
}

void send(const char* message, ...) {
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
}