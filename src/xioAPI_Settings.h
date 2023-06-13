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

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;

#define NUM_SETTINGS 79


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
#define DEFAULT_CONFIG_FILE_NAME "/default.json"
#define CONFIG_FILE_BUFFER_SIZE 6144 // Bytes - as recommended by the ArduinoJson helper (minimum: 3200)
#endif // defined(XIOAPI_USE_SPIFFS) || defined(XIOAPI_USE_SD)

extern File _file; // Create an object to hold the information for the JSON configuration file
extern StaticJsonDocument<6144> _jsonConfigDoc; // Allocate a buffer to hold the JSON data
extern bool _factoryMode;


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
    const char* key;
    unsigned long hash;
    void* value;
    SettingType type;
    size_t len;
};

extern settingTableEntry settingTable[256];
static int settingTableSize = sizeof(settingTable) / sizeof(settingTable[0]);

bool loadConfigurationsFromJSON(bool checkFile=false, const char* filename=CONFIG_FILE_NAME);
bool saveConfigurations();
void updateSetting(const settingTableEntry* entry, JsonVariant newValue);

template <typename T>
inline T getSetting(const char* key) {
    if (_jsonConfigDoc.containsKey(key)) {
        return _jsonConfigDoc[key].as<T>();
    }
    return T();
}

#endif // XIOAPI_SETTINGS_H