/******************************************************************
    @file       xio_API_Types.h
    @brief      Types for use with the xIO API Arduino Library
    @author     David Such
    @copyright  Please see the accompanying LICENSE file

    Code:       David Such
    Modified:   Braidan Duffy

    Version:    1.0.0
    Date:       13/04/23
    Modifed:    17/05/2023

    1.0.0       Original Release.                       13/04/23
    1.1.0       Restylized for Thetis implementation    17/05/2023

    Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
            (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
            and using information in the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
            Uses the Madgwick Quaternion Class.

******************************************************************/

#ifndef xioAPI_Types_h
#define xioAPI_Types_h

namespace xioAPI_Types {


// =======================
// === COMMAND MESSAGE ===
// =======================


enum MessageIdentifierASCII {
    EULER_MESSAGE_IDENT = 'A',
};

typedef enum ChargingStatus {
    DISCONNECTED = 0,
    CHARGING,
    CHARGED
} ChargingStatus;

typedef enum SerialMode {
    OFFLINE = 0,
    STANDARD,
    ACCESSORY
} SerialMode;

typedef enum TokenState {
    START_JSON = 0,
    START_CMD,
    END_CMD,
    START_VALUE,
    END_VALUE,
    END_JSON
} TokenState;

typedef enum CommandTokens {
    START_OBJ = '{',
    STRING_DELIM = '\"',
    COLON = ':',
    END_OBJ = '}',
    TERMINAL = '\n'
} CommandTokens;

typedef enum ValueType {
    JSON_STRING = 0,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL,
    JSON_UNDEFINED
} ValueType;
}
#endif // xioAPI_Types_h