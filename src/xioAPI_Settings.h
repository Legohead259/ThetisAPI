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

const struct {
  unsigned long key;
  void* value;
} settingTable[] = {
    {CALIBRATION_DATE, &settings.calibrationDate},
    {GYROSCOPE_MISALIGNMENT, &settings.gyroscopeMisalignment},
    {GYROSCOPE_SENSITIVITY, &settings.gyroscopeSensitivity},
    {GYROSCOPE_OFFSET, &settings.gyroscopeOffset},
    {ACCELEROMETER_MISALIGNMENT, &settings.accelerometerMisalignment},
    {ACCELEROMETER_SENSITIVITY, &settings.accelerometerSensitivity},
    {ACCELEROMETER_OFFSET, &settings.accelerometerOffset},
    {SOFT_IRON_MATRIX, &settings.softIronMatrix},
    {HARD_IRON_OFFSET, &settings.hardIronOffset},
    {HIGHG_ACCELEROMETER_MISALIGNMENT, &settings.highGAccelerometerMisalignment},
    {HIGHG_ACCELEROMETER_SENSITIVITY, &settings.highGAccelerometerSensitivity},
    {HIGHG_ACCELEROMETER_OFFSET, &settings.highGAccelerometerOffset},
    {DEVICE_NAME, settings.deviceName},
    {SERIAL_NUMBER, settings.serialNumber},
    {FIRMWARE_VERSION, settings.firmwareVersion},
    {BOOTLOADER_VERSION, settings.bootloaderVersion},
    {HARDWARE_VERSION, settings.hardwareVersion},
    {SERIAL_MODE, &settings.serialMode},
    {SERIAL_BAUD_RATE, &settings.serialBaudRate},
    {SERIAL_RTC_CTS_ENABLED, &settings.serialRtsCtsEnabled},
    {SERIAL_ACCESSORY_NUMBER_OF_BYTES, &settings.serialAccessoryNumberOfBytes},
    {SERIAL_ACCESSORY_TERMINATION_BYTE, &settings.serialAccessoryTerminationByte},
    {SERIAL_ACCESSORY_TIMEOUT, &settings.serialAccessoryTimeout},
    {WIRELESS_MODE, &settings.wirelessMode},
    {WIRELESS_FIRMWARE_VERSION, settings.wirelessFirmwareVersion},
    {EXTERNAL_ANTENNAE_ENABLED, &settings.externalAntennaeEnabled},
    {WIFI_REGION, &settings.wiFiRegion},
    {WIFI_MAC_ADDRESS, settings.wiFiMacAddress},
    {WIFI_IP_ADDRESS, settings.wiFiIPAddress},
    {WIFI_CLIENT_SSID, settings.wiFiClientSsid},
    {WIFI_CLIENT_KEY, settings.wiFiClientKey},
    {WIFI_CLIENT_CHANNEL, &settings.wiFiClientChannel},
    {WIFI_CLIENT_DHCP_ENABLED, &settings.wiFiClientDhcpEnabled},
    {WIFI_CLIENT_IP_ADDRESS, settings.wiFiClientIPAddress},
    {WIFI_CLIENT_NETMASK, settings.wiFiClientNetmask},
    {WIFI_CLIENT_GATEWAY, settings.wiFiClientGateway},
    {WIFI_AP_SSID, settings.wiFiAPSsid},
    {WIFI_AP_KEY, settings.wiFiAPKey},
    {WIFI_AP_CHANNEL, &settings.wiFiAPChannel},
    {TCP_PORT, &settings.tcpPort},
    {UDP_IP_ADDRESS, settings.udpIPAddress},
    {UDP_SEND_PORT, &settings.udpSendPort},
    {UDP_RECEIVE_PORT, &settings.udpReceivePort},
    {SYNCHRONISATION_ENABLED, &settings.synchronisationEnabled},
    {SYNCHRONISATION_NETWORK_LATENCY, &settings.synchronisationNetworkLatency},
    {BLUETOOTH_ADDRESS, &settings.bluetoothAddress},
    {BLUETOOTH_NAME, settings.bluetoothName},
    {BLUETOOTH_PIN_CODE, settings.bluetoothPinCode},
    {BLUETOOTH_DISCOVERY_MODE, &settings.bluetoothDiscoveryMode},
    {BLUETOOTH_PAIRED_ADDRESS, &settings.bluetoothPairedAddress},
    {BLUETOOTH_PAIRED_LINK_KEY, &settings.bluetoothPairedLinkKey},
    {DATA_LOGGER_ENABLED, &settings.dataLoggerEnabled},
    {DATA_LOGGER_FILE_NAME_PREFIX, settings.dataLoggerFileNamePrefix},
    {DATA_LOGGER_FILE_NAME_TIME_ENABLED, &settings.dataLoggerFileNameTimeEnabled},
    {DATA_LOGGER_FILE_NAME_COUNTER_ENABLED, &settings.dataLoggerFileNameCounterEnabled},
    {DATA_LOGGER_MAX_FILE_SIZE, &settings.dataLoggerMaxFileSize},
    {DATA_LOGGER_MAX_FILE_PERIOD, &settings.dataLoggerMaxFilePeriod},
    {AXES_ALIGNMENT, &settings.axesAlignment},
    {GYROSCOPE_OFFSET_CORRECTION_ENABLED, &settings.gyroscopeOffsetCorrectionEnabled},
    {AHRS_AXES_CONVENTION, &settings.ahrsAxesConvention},
    {AHRS_GAIN, &settings.ahrsGain},
    {AHRS_IGNORE_MAGNETOMETER, &settings.ahrsIgnoreMagnetometer},
    {AHRS_ACCELERATION_REJECTION_ENABLED, &settings.ahrsAccelerationRejectionEnabled},
    {AHRS_MAGNETIC_REJECTION_ENABLED, &settings.ahrsMagneticRejectionEnabled},
    {BINARY_MODE_ENABLED, &settings.binaryModeEnabled},
    {USB_DATA_MESSAGES_ENABLED, &settings.usbDataMessagesEnabled},
    {SERIAL_DATA_MESSAGES_ENABLED, &settings.serialDataMessagesEnabled},
    {TCP_DATA_MESSAGES_ENABLED, &settings.tcpDataMessagesEnabled},
    {UDP_DATA_MESSAGES_ENABLED, &settings.udpDataMessagesEnabled},
    {BLUETOOTH_DATA_MESSAGES_ENABLED, &settings.bluetoothDataMessagesEnabled},
    {DATA_LOGGER_DATA_MESSAGES_ENABLED, &settings.dataLoggerDataMessagesEnabled},
    {AHRS_MESSAGE_TYPE, &settings.ahrsMessageType},
    {INERTIAL_MESSAGE_RATE_DIVISOR, &settings.inertialMessageRateDivisor},
    {MAGNETOMETER_MESSAGE_RATE_DIVISOR, &settings.magnetometerMessageRateDivisor},
    {AHRS_MESSAGE_RATE_DIVISOR, &settings.ahrsMessageRateDivisor},
    {HIGHG_ACCELEROMETER_MESSAGE_RATE_DIVISOR, &settings.highGAccelerometerMessageRateDivisor},
    {TEMPERATURE_MESSAGE_RATE_DIVISOR, &settings.temperatureMessageRateDivisor},
    {BATTERY_MESSAGE_RATE_DIVISOR, &settings.batteryMessageRateDivisor},
    {RSSI_MESSAGE_RATE_DIVISOR, &settings.rssiMessageRateDivisor},
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