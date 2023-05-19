/******************************************************************
    @file       xio_API_Protocol.h
    @brief      Commands & MSG_IDs for the xio API
    @author     David Such
    @copyright  Please see the accompanying LICENSE file

    Code:       David Such
    Modified:   Braidan Duffy
    Version:    1.2.0
    Date:       24/04/23
    Modifed:    17/05/2023

    1.1.0       Original Release.       24/04/23

    Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
            (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
            and using information in the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).

******************************************************************/

#ifndef xioAPI_Protocol_h
#define xioAPI_Protocol_h

//  Based on API v1.1 - https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf

#define xioAPI_VERSION_MAJOR     1
#define xioAPI_VERSION_MINOR     1
#define xioAPI_VERSION_REVISION  0

namespace xioAPI_Protocol {

/**
 * Hash table for the keys in API messages
 * This makes evaluating the keys received from the API requests much easier.
 * Consult the x-IMU3 user manual for specifics on each setting type
 * 
 * (Hashed using the DJB2 hash function)
*/
enum APIKeyHashASCII {
    // ----------------
    // --- SETTINGS ---
    // ----------------

    CALIBRATION_DATE                            = 0x69AE1A4B,
    GYROSCOPE_MISALIGNMENT                      = 0xC6E2FB88,
    GYROSCOPE_SENSITIVITY                       = 0xB36B4E03,
    GYROSCOPE_OFFSET                            = 0x54042907,
    ACCELEROMETER_MISALIGNMENT                  = 0x05F233A8,
    ACCELEROMETER_SENSITIVITY                   = 0x5B35E82B,
    ACCELEROMETER_OFFSET                        = 0x1E93C927,
    SOFT_IRON_MATRIX                            = 0x48A77FAE,
    HARD_IRON_MATRIX                            = 0x604CAF63,
    HIGHG_ACCELEROMETER_MISALIGNMENT            = 0xA3C1348F,
    HIGHG_ACCELEROMETER_SENSITIVITY             = 0xFB24E832,
    HIGHG_ACCELEROMETER_OFFSET                  = 0x0749E8CE,
    DEVICE_NAME                                 = 0x26210236,
    SERIAL_NUMBER                               = 0x4440862E,
    FIRMWARE_VERSION                            = 0x905952C8,
    BOOTLOADER_VERSION                          = 0x6B037A96,
    HARDWARE_VERSION                            = 0x445818D9,
    SERIAL_MODE                                 = 0xAD8C37EA,
    SERIAL_BAUD_RATE                            = 0x529AE1ED,
    SERIAL_RTC_CTS_ENABLED                      = 0x1B98A393,
    SERIAL_ACCESSORY_NUMBER_OF_BYTES            = 0x2288AC96,
    SERIAL_ACCESSORY_TERMINATION_BYTE           = 0x724D8BCF,
    SERIAL_ACCESSORY_TIMEOUT                    = 0x526AD0D8,
    WIRELESS_MODE                               = 0xADA4E258,
    WIRELESS_FIRMWARE_VERSION                   = 0x7B099616,
    EXTERNAL_ANTENNA_ENABLED                    = 0x7CDBF0F8,
    WIFI_REGION                                 = 0xD0E9C698,
    WIFI_MAC_ADDRESS                            = 0x6EB6736B,
    WIFI_IP_ADDRESS                             = 0x69F3792B,
    WIFI_CLIENT_SSID                            = 0xBCC58466,
    WIFI_CLIENT_KEY                             = 0x3443E7FC,
    WIFI_CLIENT_CHANNEL                         = 0xA16900CC,
    WIFI_CLIENT_DHCP_ENABLED                    = 0x438902FD,
    WIFI_CLIENT_IP_ADDRESS                      = 0x7D2203B2,
    WIFI_CLIENT_NETMASK                         = 0xEA8132E6,
    WIFI_CLIENT_GATEWAY                         = 0xC650DF65,
    WIFI_AP_SSID                                = 0xA7BB4378,
    WIFI_AP_KEY                                 = 0x05150CCE,
    WIFI_AP_CHANNEL                             = 0x0D043E9E,
    TCP_PORT                                    = 0xD42A5931,
    UDP_IP_ADDRESS                              = 0x1CE09BAD,
    UDP_SEND_PORT                               = 0x69438A3D,
    UDP_RECEIVE_PORT                            = 0xCB83F816,
    SYNCHRONISATION_ENABLED                     = 0x35988A1B,
    SYNCHRONISATION_NETWORK_LATENCY             = 0x71194A2A,
    BLUETOOTH_ADDRESS                           = 0xB26E05C1,
    BLUETOOTH_NAME                              = 0xB11E1DDC,
    BLUETOOTH_PIN_CODE                          = 0x415CB7FD,
    BLUETOOTH_DISCOVERY_MODE                    = 0xDE0C0138,
    BLUETOOTH_PAIRED_ADDRESS                    = 0xF726EDB6,
    BLUETOOTH_PAIRED_LINK_KEY                   = 0x52420327,
    DATA_LOGGER_ENABLED                         = 0xE7FE4A2A,
    DATA_LOGGER_FILE_NAME_PREFIX                = 0x8644900E,
    DATA_LOGGER_FILE_NAME_TIME_ENABLED          = 0xDD8AF43A,
    DATA_LOGGER_FILE_NAME_COUNTER_ENABLED       = 0xB6038E0B,
    DATA_LOGGER_MAX_FILE_SIZE                   = 0xB50949E0,
    DATA_LOGGER_MAX_FILE_PERIOD                 = 0x153722A8,
    AXES_ALIGNMENT                              = 0xAAAD3615,
    GYROSCOPE_OFFSET_CORRECTION_ENABLED         = 0x5F5F04EA,
    AHRS_AXES_CONVENTION                        = 0x02CF05E7,
    AHRS_GAIN                                   = 0x588354D2,
    AHRS_IGNORE_MAGNETOMETER                    = 0xD45325FF,
    AHRS_ACCELERATION_REJECTION_ENABLED         = 0x564448AB,
    AHRS_MAGNETIC_REJECTION_ENABLED             = 0x59D809E9,
    BINARY_MODE_ENABLED                         = 0x22FA119A,
    USB_DATA_MESSAGES_ENABLED                   = 0xCAE8216C,
    SERIAL_DATA_MESSAGES_ENABLED                = 0x3FA9FCA2,
    TCP_DATA_MESSAGES_ENABLED                   = 0x7190B809,
    UDP_DATA_MESSAGES_ENABLED                   = 0x3549FD2B,
    BLUETOOTH_DATA_MESSAGES_ENABLED             = 0x96ED95D8,
    DATA_LOGGER_DATA_MESSAGES_ENABLED           = 0x49B88A9C,
    AHRS_MESSAGE_TYPE                           = 0x2BE1B53A,
    INERTIAL_MESSAGE_RATE_DIVISOR               = 0x4F84ED4E,
    MAGNETOMETER_MESSAGE_RATE_DIVISOR           = 0x271938DE,
    AHRS_MESSAGE_RATE_DIVISOR                   = 0xCD61B2E4,
    HIGHG_ACCELEROMETER_MESSAGE_RATE_DIVISOR    = 0x74C3AB38,
    TEMPERATURE_MESSAGE_RATE_DIVISOR            = 0x632401C4,
    BATTERY_MESSAGE_RATE_DIVISOR                = 0xF781B9D1,
    RSSI_MESSAGE_RATE_DIVISOR                   = 0xCA009AF7,

    // ----------------
    // --- COMMANDS ---
    // ----------------

    XIO_DEFAULT = 0x0885548A,
    APPLY       = 0x0F199E4B,
    SAVE        = 0x7C9DCB54,
    TIME        = 0x7C9E7894,
    PING        = 0x7C9C4733,
    RESET       = 0x10474288,
    SHUTDOWN    = 0xFC460361,
    STROBE      = 0x1C93C7F4,
    COLOUR      = 0xF6E37B99,
    HEADING     = 0x3C153F55,
    ACCESSORY   = 0x2B792E11,
    NOTE        = 0x7C9B48BB,
    FORMAT      = 0xFDE634AE,
    TEST        = 0x7C9E6865,
    BOOTLOADER  = 0x9999EDD0,
    FACTORY     = 0x98F9347D,
    ERASE       = 0x0F62DA15
};

/******************************************************************
 *
 * xIMU3 Global Constants
 * 
 ******************************************************************/

#define BUFFER_SIZE 100
#define CMD_SIZE 32
#define NOTE_SIZE 127
#define HASH_SIZE 751

const char NULL_TERMINATOR  = '\0';


// ============================
// === API TYPE DEFINITIONS ===
// ============================


struct QuaternionMessage {
    float w, x, y, z;   // Quaternion vector elements from q[0] to q[3]
    uint32_t timestamp; // System timestamp in microseconds
};

struct EulerMessage {
  float roll;           // Rotation around x-axis in degrees
  float pitch;          // Rotation around y-axis in degrees
  float yaw;            // Rotation around z-axis in degrees
  uint32_t timestamp;   // System timestamp in microseconds
};

typedef struct EulerAngles {
    float roll;
    float pitch;
    float yaw;
} EulerAngles;

struct InertialMessage {
    float ax, ay, az;
    float gx, gy, gz;
    uint32_t timestamp;
};

struct MagnetoMessage {
    float mx, my, mz;
    uint32_t  timestamp;
};

struct TemperatureMessage {
    float temp;           // IMU temperature in degrees Celsius 
    uint32_t  timestamp;  // System timestamp in microseconds
};

struct BatteryMessage {
    float percentCharged;
    float voltage;
    xioAPI_Types::ChargingStatus status;
    uint32_t timestamp;
};

struct RSSIMessage {
    float percentage;
    float power;
    uint32_t timestamp;
};

struct BatteryData {
    float percentCharged; 
    float voltage;
    xioAPI_Types::ChargingStatus status;
};

struct RSSIData {
    float percentage;
    float power;
};

struct RawData {
    int16_t rx, ry, rz;
    uint32_t  timestamp;
};

struct ScaledData {
    float sx, sy, sz;
    uint32_t  timestamp;
};

struct SensorData {
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    uint32_t gTimestamp, aTimestamp, mTimestamp;
};

struct Ping {
    char *interface;
    char *deviceName;
    char *serialNumber;
};

struct NetworkAnnouncement {
    uint16_t sync;
    char *displayName;
    char *serialNumber;
    char *ipAddress;
    uint16_t portTCP;
    uint16_t sendUDP;
    uint16_t receiveUDP;
    uint8_t rssiPercentage;
    uint8_t batteryPercentage;
    uint8_t chargingStatus;
};
}

#endif // xioAPI_Protocol_h