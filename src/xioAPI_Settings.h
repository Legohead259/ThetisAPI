/******************************************************************
    @file       xioAPI_Settings.h
    @brief      Arduino Library for the xio API. This file focusses
                specifically on settings and management. Heavily
                relies on ArduinoJson
    @author     Braidan Duffy
    @copyright  MIT license

    Code:       Braidan Duffy
    Version:    1.0.0
    Date:       19/05/2023
    Modifed:    19/05/2023

    CHANGELOG:
    v1.0.0 - Initial release

    TODO:
    Add support for user-defined storage spaces (i.e. SPIFFS or SD or EEPROM)

    Credit - Derived from the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).

******************************************************************/

#ifndef XIOAPI_SETTINGS_H
#define XIOAPI_SETTINGS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "xioAPI_Types.h"
#include "xioAPI_Protocol.h"

#ifdef LOGGER_H
#include "../../filesystem/logger.h"
#endif // LOGGER_H

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;


// ===================================
// === USER-DEFINED STORAGE SPACES ===
// ===================================


#define XIOAPI_USE_SPIFFS   // Use SPIFFS (SPI Flash File System) to store configuration data (default)
// #define XIOAPI_USE_SD       // Use SD flash storage to store configuration data
#define XIOAPI_USE_EEPROM   // Use EEPROM (Electronically Eraseable Programmable Read-Only Memory) to store configuration data


#ifdef XIOAPI_USE_SPIFFS
#include <SPIFFS.h>
#endif // XIOAPI_USE_SPIFFS

#ifdef XIOAPI_USE_SD
#include <SD.h>
#endif // XIOAPI_USE_SD

#if defined(XIOAPI_USE_SPIFFS) || defined(XIOAPI_USE_SD)
#define CONFIG_FILE_NAME "/config.json"
#define CONFIG_FILE_BUFFER_SIZE 6144 // Bytes - as recommended by the ArduinoJson helper (minimum: 3200)
#endif // defined(XIOAPI_USE_SPIFFS) || defined(XIOAPI_USE_SD)

extern File _file; // Create an object to hold the information for the JSON configuration file
extern StaticJsonDocument<6144> _jsonConfigDoc; // Allocate a buffer to hold the JSON data


// ============================
// === SETTINGS DEFINITIONS ===
// ============================


typedef struct device_settings_t {
    char calibrationDate[32]; // "YYYY-MM-DD hh:mm:ss"
    float gyroscopeMisalignment[9];
    float gyroscopeSensitivity[3];
    float gyroscopeOffset[3]; 
    float accelerometerMisalignment[9];
    float accelerometerSensitivity[3];
    float accelerometerOffset[3];
    float softIronMatrix[9];
    float hardIronOffset[3];
    float highGAccelerometerMisalignment[9];
    float highGAccelerometerSensitivity[3];
    float highGAccelerometerOffset[3];
    char deviceName[32]; // "x-IMU3"
    char serialNumber[20]; // "XXXX-XXXX-XXXX-XXXX"
    char firmwareVersion[10]; // "vXX.YY.ZZ"
    char bootloaderVersion[10]; // "vXX.YY.ZZ"
    char hardwareVersion[8]; // "revXYY"
    serial_mode_t serialMode;
    serial_baudrate_t serialBaudRate;
    bool serialRtsCtsEnabled;
    int serialAccessoryNumberOfBytes;
    uint8_t serialAccessoryTerminationByte;
    int serialAccessoryTimeout;
    wireless_mode_t wirelessMode;
    char wirelessFirmwareVersion[10]; // "vXX.YY.ZZ"
    bool externalAntennaeEnabled;
    wireless_region_t wiFiRegion;
    char wiFiMacAddress[18]; // "01.23.45.67.89.AB."
    char wiFiIPAddress[16]; // "192.168.254.254"
    char wiFiClientSsid[64];
    char wiFiClientKey[64];
    wireless_channels_t wiFiClientChannel;
    bool wiFiClientDhcpEnabled;
    char wiFiClientIPAddress[16]; // "192.168.254.254"
    char wiFiClientNetmask[16]; // "255.255.255.255"
    char wiFiClientGateway[16]; // "192.168.254.254"
    char wiFiAPSsid[64];
    char wiFiAPKey[64];
    wireless_ap_channels_t wiFiAPChannel;
    int tcpPort;
    char udpIPAddress[16]; // "192.168.254.254"
    int udpSendPort;
    int udpReceivePort;
    bool synchronisationEnabled;
    int synchronisationNetworkLatency;
    int bluetoothAddress;
    char bluetoothName[32]; // "x-IMU3.XXXX-XXXX-XXXX-XXXX"
    char bluetoothPinCode[5]; // "1234"
    bluetooth_discovery_mode_t bluetoothDiscoveryMode;
    int bluetoothPairedAddress;
    int bluetoothPairedLinkKey;
    bool dataLoggerEnabled;
    char dataLoggerFileNamePrefix[16];
    bool dataLoggerFileNameTimeEnabled;
    bool dataLoggerFileNameCounterEnabled;
    int dataLoggerMaxFileSize;
    int dataLoggerMaxFilePeriod;
    axes_alignment_t axesAlignment;
    bool gyroscopeOffsetCorrectionEnabled;
    ahrs_axes_convention_t ahrsAxesConvention;
    float ahrsGain;
    bool ahrsIgnoreMagnetometer;
    bool ahrsAccelerationRejectionEnabled;
    bool ahrsMagneticRejectionEnabled;
    bool binaryModeEnabled;
    bool usbDataMessagesEnabled;
    bool serialDataMessagesEnabled;
    bool tcpDataMessagesEnabled;
    bool udpDataMessagesEnabled;
    bool bluetoothDataMessagesEnabled;
    bool dataLoggerDataMessagesEnabled;
    ahrs_message_type_t ahrsMessageType;
    int inertialMessageRateDivisor;
    int magnetometerMessageRateDivisor;
    int ahrsMessageRateDivisor;
    int highGAccelerometerMessageRateDivisor;
    int temperatureMessageRateDivisor;
    int batteryMessageRateDivisor;
    int rssiMessageRateDivisor;
} device_settings_t;

extern device_settings_t settings;

typedef enum {
    BOOL,
    CHAR,
    FLOAT,
    INT,
    FLOAT_ARRAY,
    CHAR_ARRAY
} SettingType;

struct settingTableEntry {
    unsigned long key;
    void* value;
    SettingType type;
    size_t len;
};

const struct settingTableEntry settingTable[] = {
    {CALIBRATION_DATE, &settings.calibrationDate, CHAR_ARRAY, 32},
    {GYROSCOPE_MISALIGNMENT, &settings.gyroscopeMisalignment, FLOAT_ARRAY, 9},
    {GYROSCOPE_SENSITIVITY, &settings.gyroscopeSensitivity, FLOAT_ARRAY, 3},
    {GYROSCOPE_OFFSET, &settings.gyroscopeOffset, FLOAT_ARRAY, 3},
    {ACCELEROMETER_MISALIGNMENT, &settings.accelerometerMisalignment, FLOAT_ARRAY, 9},
    {ACCELEROMETER_SENSITIVITY, &settings.accelerometerSensitivity, FLOAT_ARRAY, 3},
    {ACCELEROMETER_OFFSET, &settings.accelerometerOffset, FLOAT_ARRAY, 3},
    {SOFT_IRON_MATRIX, &settings.softIronMatrix, FLOAT_ARRAY, 9},
    {HARD_IRON_OFFSET, &settings.hardIronOffset, FLOAT_ARRAY, 3},
    {HIGHG_ACCELEROMETER_MISALIGNMENT, &settings.highGAccelerometerMisalignment, FLOAT_ARRAY, 9},
    {HIGHG_ACCELEROMETER_SENSITIVITY, &settings.highGAccelerometerSensitivity, FLOAT_ARRAY, 3},
    {HIGHG_ACCELEROMETER_OFFSET, &settings.highGAccelerometerOffset, FLOAT_ARRAY, 3},
    {DEVICE_NAME, &settings.deviceName, CHAR_ARRAY, 32},
    {SERIAL_NUMBER, &settings.serialNumber, CHAR_ARRAY, 20},
    {FIRMWARE_VERSION, &settings.firmwareVersion, CHAR_ARRAY, 10},
    {BOOTLOADER_VERSION, &settings.bootloaderVersion, CHAR_ARRAY, 10},
    {HARDWARE_VERSION, &settings.hardwareVersion, CHAR_ARRAY, 8},
    {SERIAL_MODE, &settings.serialMode, INT},
    {SERIAL_BAUD_RATE, &settings.serialBaudRate, INT},
    {SERIAL_RTC_CTS_ENABLED, &settings.serialRtsCtsEnabled, BOOL},
    {SERIAL_ACCESSORY_NUMBER_OF_BYTES, &settings.serialAccessoryNumberOfBytes, INT},
    {SERIAL_ACCESSORY_TERMINATION_BYTE, &settings.serialAccessoryTerminationByte, CHAR},
    {SERIAL_ACCESSORY_TIMEOUT, &settings.serialAccessoryTimeout, INT},
    {WIRELESS_MODE, &settings.wirelessMode, INT},
    {WIRELESS_FIRMWARE_VERSION, &settings.wirelessFirmwareVersion, CHAR_ARRAY, 10},
    {EXTERNAL_ANTENNAE_ENABLED, &settings.externalAntennaeEnabled, BOOL},
    {WIFI_REGION, &settings.wiFiRegion, INT},
    {WIFI_MAC_ADDRESS, &settings.wiFiMacAddress, CHAR_ARRAY, 18},
    {WIFI_IP_ADDRESS, &settings.wiFiIPAddress, CHAR_ARRAY, 16},
    {WIFI_CLIENT_SSID, &settings.wiFiClientSsid, CHAR_ARRAY, 64},
    {WIFI_CLIENT_KEY, &settings.wiFiClientKey, CHAR_ARRAY, 64},
    {WIFI_CLIENT_CHANNEL, &settings.wiFiClientChannel, INT},
    {WIFI_CLIENT_DHCP_ENABLED, &settings.wiFiClientDhcpEnabled, BOOL},
    {WIFI_CLIENT_IP_ADDRESS, &settings.wiFiClientIPAddress, CHAR_ARRAY, 16},
    {WIFI_CLIENT_NETMASK, &settings.wiFiClientNetmask, CHAR_ARRAY, 16},
    {WIFI_CLIENT_GATEWAY, &settings.wiFiClientGateway, CHAR_ARRAY, 16},
    {WIFI_AP_SSID, &settings.wiFiAPSsid, CHAR_ARRAY, 64},
    {WIFI_AP_KEY, &settings.wiFiAPKey, CHAR_ARRAY, 64},
    {WIFI_AP_CHANNEL, &settings.wiFiAPChannel, INT},
    {TCP_PORT, &settings.tcpPort, INT},
    {UDP_IP_ADDRESS, &settings.udpIPAddress, CHAR_ARRAY, 16},
    {UDP_SEND_PORT, &settings.udpSendPort, INT},
    {UDP_RECEIVE_PORT, &settings.udpReceivePort, INT},
    {SYNCHRONISATION_ENABLED, &settings.synchronisationEnabled, BOOL},
    {SYNCHRONISATION_NETWORK_LATENCY, &settings.synchronisationNetworkLatency, INT},
    {BLUETOOTH_ADDRESS, &settings.bluetoothAddress, INT},
    {BLUETOOTH_NAME, &settings.bluetoothName, CHAR_ARRAY, 32},
    {BLUETOOTH_PIN_CODE, &settings.bluetoothPinCode, CHAR_ARRAY, 5},
    {BLUETOOTH_DISCOVERY_MODE, &settings.bluetoothDiscoveryMode, INT},
    {BLUETOOTH_PAIRED_ADDRESS, &settings.bluetoothPairedAddress, INT},
    {BLUETOOTH_PAIRED_LINK_KEY, &settings.bluetoothPairedLinkKey, INT},
    {DATA_LOGGER_ENABLED, &settings.dataLoggerEnabled, BOOL},
    {DATA_LOGGER_FILE_NAME_PREFIX, &settings.dataLoggerFileNamePrefix, CHAR_ARRAY, 16},
    {DATA_LOGGER_FILE_NAME_TIME_ENABLED, &settings.dataLoggerFileNameTimeEnabled, BOOL},
    {DATA_LOGGER_FILE_NAME_COUNTER_ENABLED, &settings.dataLoggerFileNameCounterEnabled, BOOL},
    {DATA_LOGGER_MAX_FILE_SIZE, &settings.dataLoggerMaxFileSize, INT},
    {DATA_LOGGER_MAX_FILE_PERIOD, &settings.dataLoggerMaxFilePeriod, INT},
    {AXES_ALIGNMENT, &settings.axesAlignment, INT},
    {GYROSCOPE_OFFSET_CORRECTION_ENABLED, &settings.gyroscopeOffsetCorrectionEnabled, BOOL},
    {AHRS_AXES_CONVENTION, &settings.ahrsAxesConvention, INT},
    {AHRS_GAIN, &settings.ahrsGain, FLOAT},
    {AHRS_IGNORE_MAGNETOMETER, &settings.ahrsIgnoreMagnetometer, BOOL},
    {AHRS_ACCELERATION_REJECTION_ENABLED, &settings.ahrsAccelerationRejectionEnabled, BOOL},
    {AHRS_MAGNETIC_REJECTION_ENABLED, &settings.ahrsMagneticRejectionEnabled, BOOL},
    {BINARY_MODE_ENABLED, &settings.binaryModeEnabled, BOOL},
    {USB_DATA_MESSAGES_ENABLED, &settings.usbDataMessagesEnabled, BOOL},
    {SERIAL_DATA_MESSAGES_ENABLED, &settings.serialDataMessagesEnabled, BOOL},
    {TCP_DATA_MESSAGES_ENABLED, &settings.tcpDataMessagesEnabled, BOOL},
    {UDP_DATA_MESSAGES_ENABLED, &settings.udpDataMessagesEnabled, BOOL},
    {BLUETOOTH_DATA_MESSAGES_ENABLED, &settings.bluetoothDataMessagesEnabled, BOOL},
    {DATA_LOGGER_DATA_MESSAGES_ENABLED, &settings.dataLoggerDataMessagesEnabled, BOOL},
    {AHRS_MESSAGE_TYPE, &settings.ahrsMessageType, INT},
    {INERTIAL_MESSAGE_RATE_DIVISOR, &settings.inertialMessageRateDivisor, INT},
    {MAGNETOMETER_MESSAGE_RATE_DIVISOR, &settings.magnetometerMessageRateDivisor, INT},
    {AHRS_MESSAGE_RATE_DIVISOR, &settings.ahrsMessageRateDivisor, INT},
    {HIGHG_ACCELEROMETER_MESSAGE_RATE_DIVISOR, &settings.highGAccelerometerMessageRateDivisor, INT},
    {TEMPERATURE_MESSAGE_RATE_DIVISOR, &settings.temperatureMessageRateDivisor, INT},
    {BATTERY_MESSAGE_RATE_DIVISOR, &settings.batteryMessageRateDivisor, INT},
    {RSSI_MESSAGE_RATE_DIVISOR, &settings.rssiMessageRateDivisor, INT},
};

static int settingTableSize = sizeof(settingTable) / sizeof(settingTable[0]);

bool loadConfigurationsFromJSON(bool checkFile=false);
void saveConfigurations();

template<typename T>
inline void updateSetting(const char* key, const T& value) {
    // Serial.print("Key: "); Serial.print(key); 
    // Serial.print(" Value: "); Serial.print(value);
    // Serial.println();

    if (_jsonConfigDoc.containsKey(key)) {
        // Serial.println("Found key");
        _jsonConfigDoc[key] = value; // Update value inside JSON buffer
        // Serial.println(_jsonConfigDoc[key].as<T>());
        loadConfigurationsFromJSON(); // Update settings struct
        // Serial.println("Updated value");
    }
}

// template<>
// inline void updateSetting(const char* key, float (*value)[3][3]) {
//     if (_jsonConfigDoc.containsKey(key)) {
//         Serial.println("Got matrix");
//     }
// }

template <typename T>
inline T getSetting(const char* key) {
    if (_jsonConfigDoc.containsKey(key)) {
        return _jsonConfigDoc[key].as<T>();
    }
    return T();
}

#endif // XIOAPI_SETTINGS_H