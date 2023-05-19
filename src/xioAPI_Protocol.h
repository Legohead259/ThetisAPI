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

#undef DEFAULT 

namespace xioAPI_Protocol {

/**
 * Command message hash table
 * This makes evaluating the keys received from the API requests much easier
 * NOTE: Not all keys are included
 * 
 * (Hashed using the DJB2 hash function)
*/
enum CommandMessageHashASCII {
    CALIBRATION_DATE                = 0x69AE1A4B,
    GYROSCOPE_MISALIGNMENT          = 0xC6E2FB88,
    GYROSCOPE_SENSITIVITY           = 0x1B3D340B,
    GYROSCOPE_OFFSET                = 0x54042907,
    ACCELEROMETER_MISALIGNMENT      = 0x05F233A8,
    ACCELEROMETER_SENSITIVITY       = 0x5B35E82B,
    PING                            = 0x7C9C4733,
    SERIAL_NUMBER                   = 0x4440862E,
    SERIAL_MODE                     = 0xAD8C37EA,
    SERIAL_BAUDRATE                 = 0x52AC6E0D,
    DEVICE_NAME                     = 0x26210236,
    DEFAULT                         = 0x0885548A,
    APPLY                           = 0x0F199E4B,
    SAVE                            = 0x7C9DCB54,
    TIME                            = 0x7C9E7894,
    RESET                           = 0x10474288,
    SHUTDOWN                        = 0xFC460361,
    STROBE                          = 0x1C93C7F4,
    COLOUR                          = 0xF6E37B99,
    HEADING                         = 0x3C153F55,
    ACCESSORY                       = 0x2B792E11,
    NOTE                            = 0x7C9B48BB,
    FORMAT                          = 0xFDE634AE,
    TEST                            = 0x7C9E6865,
    BOOTLOADER                      = 0x9999EDD0,
    FACTORY                         = 0x98F9347D,
    ERASE                           = 0x0F62DA15,
    FIRMWARE_VERSION                = 0x905952C8,
    BOOTLOADER_VERSION              = 0x6B037A96,
    HARDWARE_VERSION                = 0x445818D9,
    SERIAL_RTC_CTS_ENABLED          = 0x1B98A393,
    WIRELESS_MODE                   = 0xADA4E258,
    WIRELESS_FIRMWARE_VERSION       = 0x7B099616,
    EXTERNAL_ANTENNA_ENABLED        = 0x7CDBF0F8,
    WIFI_REGION                     = 0xD0E9C698,
    WIFI_MAC_ADDRESS                = 0x6EB6736B,
    SYNCHRONIZATION_ENABLED         = 0x6131CCA2,
    UNKNOWN_KEY                     = 120
};

#define DEFAULT 1

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