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

******************************************************************/

#ifndef xioAPI_Types_h
#define xioAPI_Types_h

namespace xioAPI_Types {


// =======================
// === COMMAND MESSAGE ===
// =======================


typedef enum ChargingStatus {
    DISCONNECTED = 0,
    CHARGING,
    CHARGED
} ChargingStatus;


// =======================
// === DEVICE SETTINGS ===
// =======================


typedef enum {
    OFFLINE = 0,
    STANDARD,
    ACCESSORY
} serial_mode_t;

typedef enum {
    BAUDRATE_9600 = 9600,
    BAUDRATE_19200 = 19200,
    BAUDRATE_38400 = 38400,
    BAUDRATE_57600 = 57600,
    BAUDRATE_115200 = 115200,
    BAUDRATE_230400 = 230400,
    BAUDRATE_460800 = 460800,
    BAUDRATE_921600 = 921600
} serial_baudrate_t;

typedef enum wireless_mode_t {
    WIRELESS_DISABLED = 0,
    WIFI_CLIENT,
    WIFI_AP,
    BLUETOOTH
} wireless_mode_t;

typedef enum wireless_region_t {
    UNITED_STATES = 1,
    EUROPE = 2,
    JAPAN = 3
} wireless_region_t;

typedef enum wireless_channels_t {
    CHANNEL_0 = 0,      // Search all channels
    CHANNEL_1,          // 2.4 Ghz
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    CHANNEL_10,
    CHANNEL_11,
    CHANNEL_12,         // Invalid for US
    CHANNEL_13,         // Invalid for US
    CHANNEL_14,         // Invalid for US and EU
    CHANNEL_36 = 36,    // 5 GHz
    CHANNEL_40 = 40,
    CHANNEL_44 = 44,
    CHANNEL_48 = 48,
    CHANNEL_52 = 52,
    CHANNEL_56 = 56,
    CHANNEL_60 = 60,
    CHANNEL_64 = 64,
    CHANNEL_100 = 100,
    CHANNEL_104 = 104,
    CHANNEL_108 = 108,
    CHANNEL_112 = 112,
    CHANNEL_116 = 116,
    CHANNEL_120 = 120,  // Invalid for US
    CHANNEL_124 = 124,  // Invalid for US
    CHANNEL_128 = 128,  // Invalid for US
    CHANNEL_132 = 132,
    CHANNEL_136 = 136,
    CHANNEL_140 = 140,
    CHANNEL_149 = 149,  // Invalid for EU and JA
    CHANNEL_153 = 153,  // Invalid for EU and JA
    CHANNEL_157 = 157,  // Invalid for EU and JA
    CHANNEL_161 = 161,  // Invalid for EU and JA
    CHANNEL_165 = 165,  // Invalid for EU and JA
} wireless_channels_t;

typedef enum wireless_ap_channels_t {
    AP_CHANNEL_1 = 1,      // 2.4 Ghz
    AP_CHANNEL_2,
    AP_CHANNEL_3,
    AP_CHANNEL_4,
    AP_CHANNEL_5,
    AP_CHANNEL_6,
    AP_CHANNEL_7,
    AP_CHANNEL_8,
    AP_CHANNEL_9,
    AP_CHANNEL_10,
    AP_CHANNEL_11,
    AP_CHANNEL_12,         // Invalid for US
    AP_CHANNEL_13,         // Invalid for US
    AP_CHANNEL_14,         // Invalid for US and EU
    AP_CHANNEL_36 = 36,    // 5 GHz
    AP_CHANNEL_40 = 40,
    AP_CHANNEL_44 = 44,
    AP_CHANNEL_48 = 48,
    AP_CHANNEL_149 = 149,  // Invalid for EU and JA
    AP_CHANNEL_153 = 153,  // Invalid for EU and JA
    AP_CHANNEL_157 = 157,  // Invalid for EU and JA
    AP_CHANNEL_161 = 161,  // Invalid for EU and JA
    AP_CHANNEL_165 = 165,  // Invalid for EU and JA
} wireless_ap_channels_t;

typedef enum bluetooth_discovery_mode_t {
    BLUETOOTH_DISABLED = 0,
    ENABLED,
    LIMITED
} bluetooth_discovery_mode_t;

typedef enum axes_alignment_t {
    /**
     * TODO: Parse what these mean and add comments
     * 
     * Variable order is sensor XYZ relative to body XYZ
     * Example: If the body X-axis is aligned with the sensor Y-axis and 
     * the body Y-axis is aligned with the sensor X-axis but pointing in 
     * the opposite direction, then the alignment is "+Y-X+Z" or pY_mX_pZ = 9.
    */

    pX_pY_pZ = 0,
    pX_mZ_pY,
    pX_mY_mZ,
    pX_pZ_mY,
    mX_pY_mZ,
    mX_pZ_pY,
    mX_mY_pZ,
    mX_mZ_pY,

    pY_mX_pZ,
    pY_mZ_mX,
    pY_pX_mZ,
    pY_pZ_pX,
    mY_pX_pZ,
    mY_mZ_pX,
    mY_mX_mZ,
    mY_pZ_mX,

    pZ_pY_mX,
    pZ_pX_pY,
    pZ_mY_pX,
    pZ_mX_mY,
    mZ_pY_pX,
    mZ_mX_pY,
    mZ_mY_mX,
    mZ_pX_mY
} axes_alignment_t;

typedef enum ahrs_axes_convention_t {
    NORTH_WEST_UP = 0,
    EAST_NORTH_UP,
    NORTH_EAST_DOWN
} ahrs_axes_convention_t;

typedef enum ahrs_message_type_t {
    QUATERNION = 0,
    ROTATION_MATRIX,
    EULER_ANGLES,
    LINEAR_ACCELERATION,
    EARTH_ACCELERATION
} ahrs_message_type_t;

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