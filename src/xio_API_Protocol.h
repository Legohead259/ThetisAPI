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

#ifndef xio_API_Protocol_h
#define xio_API_Protocol_h

//  Based on API v1.1 - https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf

#define xIMU3_API_VERSION_MAJOR     1
#define xIMU3_API_VERSION_MINOR     1
#define xIMU3_API_VERSION_REVISION  0

/******************************************************************
 *
 * xIMU3 Command Message Hash Table - Not all keys included
 * 
 ******************************************************************/

#define xIMU3_ping                      100  
#define xIMU3_serialNumber              353  
#define xIMU3_serialMode                445  
#define xIMU3_serialBaudRate            211  
#define xIMU3_deviceName                168  
#define xIMU3_default                   555  
#define xIMU3_apply                     80  
#define xIMU3_save                      329  
#define xIMU3_time                      372  
#define xIMU3_reset                     639  
#define xIMU3_shutdown                  69  
#define xIMU3_strobe                    367  
#define xIMU3_colour                    499  
#define xIMU3_heading                   185  
#define xIMU3_accessory                 500  
#define xIMU3_note                      293  
#define xIMU3_format                    97  
#define xIMU3_test                      735  
#define xIMU3_bootloader                259  
#define xIMU3_factory                   582  
#define xIMU3_erase                     624  
#define xIMU3_firmwareVersion           19  
#define xIMU3_bootloaderVersion         687  
#define xIMU3_hardwareVersion           421  
#define xIMU3_serialRtsCtsEnabled       434  
#define xIMU3_wirelessMode              36  
#define xIMU3_wirelessFirmwareVersion   178  
#define xIMU3_externalAntennaeEnabled   616  
#define xIMU3_wiFiRegion                70  
#define xIMU3_wiFiMacAddress            306  
#define xIMU3_synchronisationEnabled    453  
#define xIMU3_unknownKey                120 

/******************************************************************
 *
 * xIMU3 Global Constants
 * 
 ******************************************************************/

#define BUFFER_SIZE 100
#define CMD_SIZE 24
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
  uint32_t  timestamp;  // System timestamp in microseconds
};

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
    ChargingStatus status;
    uint32_t timestamp;
}

struct RSSIMessage {
    float percentage;
    float power;
    uint32_t timestamp;
};

/******************************************************************
 *
 * Quaternion Class Definition - 
 * 
 ******************************************************************/

class Quaternion {
    public:
        Quaternion(void);
        Quaternion(const float w, const float x, const float y, const float z);
        Quaternion getConjugate(void) const;
        EulerAngles getEulerAngles(void) const;

        float q[4];     //  Euler Parameters
        uint32_t  timestamp;

    private:
        float radiansToDegrees(float radians) const;
};

//------------------------------------------------------------------------------
// Variables

float q[4];

//------------------------------------------------------------------------------
// Methods

Quaternion::Quaternion(void) {
    q[0] = 1.0f;
    q[1] = 0.0f;
    q[2] = 0.0f;
    q[3] = 0.0f;
}

Quaternion::Quaternion(const float w, const float x, const float y, const float z) {
    q[0] = w;
    q[1] = x;
    q[2] = y;
    q[3] = z;
}

Quaternion Quaternion::getConjugate(void) const {
    Quaternion conjugate;
    conjugate.q[0] = q[0];
    conjugate.q[1] = -q[1];
    conjugate.q[2] = -q[2];
    conjugate.q[3] = -q[3];
    return conjugate;
}

EulerAngles Quaternion::getEulerAngles(void) const {
    EulerAngles eulerAngles;

    eulerAngles.roll = radiansToDegrees(atan2(2.0f * (q[2] * q[3] - q[0] * q[1]), 2.0f * q[0] * q[0] - 1.0f + 2.0f * q[3] * q[3]));
    eulerAngles.pitch = radiansToDegrees(-atan((2.0f * (q[1] * q[3] + q[0] * q[2])) / sqrt(1.0f - pow((2.0f * q[1] * q[3] + 2.0f * q[0] * q[2]), 2.0f))));
    eulerAngles.yaw = radiansToDegrees(atan2(2.0f * (q[1] * q[2] - q[0] * q[3]), 2.0f * q[0] * q[0] - 1.0f + 2.0f * q[1] * q[1]));

    return eulerAngles;
}

float Quaternion::radiansToDegrees (float radians) const {
    return 57.2957795130823f * radians;
}

#endif // xio_API_Protocol_h