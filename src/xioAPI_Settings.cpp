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

#include "xioAPI_Settings.h"

File _file;
StaticJsonDocument<6144> _jsonConfigDoc;
device_settings_t settings;
bool _factoryMode = false;
settingTableEntry settingTable[256] = {
    {"calibrationDate", CALIBRATION_DATE, &settings.calibrationDate, CHAR_ARRAY, 32},
    {"gyroscopeMisalignment", GYROSCOPE_MISALIGNMENT, &settings.gyroscopeMisalignment, FLOAT_ARRAY, 9},
    {"gyroscopeSensitivity", GYROSCOPE_SENSITIVITY, &settings.gyroscopeSensitivity, FLOAT_ARRAY, 3},
    {"gyroscopeOffset", GYROSCOPE_OFFSET, &settings.gyroscopeOffset, FLOAT_ARRAY, 3},
    {"AccelerometerMisalignment", ACCELEROMETER_MISALIGNMENT, &settings.accelerometerMisalignment, FLOAT_ARRAY, 9},
    {"accelerometerSensitivity", ACCELEROMETER_SENSITIVITY, &settings.accelerometerSensitivity, FLOAT_ARRAY, 3},
    {"accelerometerOffset", ACCELEROMETER_OFFSET, &settings.accelerometerOffset, FLOAT_ARRAY, 3},
    {"softIronMatrix", SOFT_IRON_MATRIX, &settings.softIronMatrix, FLOAT_ARRAY, 9},
    {"hardIronOffset", HARD_IRON_OFFSET, &settings.hardIronOffset, FLOAT_ARRAY, 3},
    {"highGAccelerometerMisalignment", HIGHG_ACCELEROMETER_MISALIGNMENT, &settings.highGAccelerometerMisalignment, FLOAT_ARRAY, 9},
    {"highGAccelerometerSensitivity", HIGHG_ACCELEROMETER_SENSITIVITY, &settings.highGAccelerometerSensitivity, FLOAT_ARRAY, 3},
    {"highGAccelerometerOffset", HIGHG_ACCELEROMETER_OFFSET, &settings.highGAccelerometerOffset, FLOAT_ARRAY, 3},
    {"deviceName", DEVICE_NAME, &settings.deviceName, CHAR_ARRAY, 32},
    {"serialNumber", SERIAL_NUMBER, &settings.serialNumber, CHAR_ARRAY, 20},
    {"firmwareVersion", FIRMWARE_VERSION, &settings.firmwareVersion, CHAR_ARRAY, 10},
    {"bootloaderVersion", BOOTLOADER_VERSION, &settings.bootloaderVersion, CHAR_ARRAY, 10},
    {"hardwareVersion", HARDWARE_VERSION, &settings.hardwareVersion, CHAR_ARRAY, 8},
    {"serialMode", SERIAL_MODE, &settings.serialMode, INT},
    {"serialBaudRate", SERIAL_BAUD_RATE, &settings.serialBaudRate, INT},
    {"serialRtsCtsEnabled", SERIAL_RTS_CTS_ENABLED, &settings.serialRtsCtsEnabled, BOOL},
    {"serialAccessoryNumberOfBytes", SERIAL_ACCESSORY_NUMBER_OF_BYTES, &settings.serialAccessoryNumberOfBytes, INT},
    {"serialAccessoryTerminationByte", SERIAL_ACCESSORY_TERMINATION_BYTE, &settings.serialAccessoryTerminationByte, CHAR},
    {"serialAccessoryTimeout", SERIAL_ACCESSORY_TIMEOUT, &settings.serialAccessoryTimeout, INT},
    {"wirelessMode", WIRELESS_MODE, &settings.wirelessMode, INT},
    {"wirelessFirmwareVersion", WIRELESS_FIRMWARE_VERSION, &settings.wirelessFirmwareVersion, CHAR_ARRAY, 10},
    {"externalAntennaeEnabled", EXTERNAL_ANTENNAE_ENABLED, &settings.externalAntennaeEnabled, BOOL},
    {"wiFiRegion", WIFI_REGION, &settings.wiFiRegion, INT},
    {"wiFiMacAddress", WIFI_MAC_ADDRESS, &settings.wiFiMacAddress, CHAR_ARRAY, 18},
    {"wiFiIPAddress", WIFI_IP_ADDRESS, &settings.wiFiIPAddress, CHAR_ARRAY, 16},
    {"wiFiClientSsid", WIFI_CLIENT_SSID, &settings.wiFiClientSsid, CHAR_ARRAY, 64},
    {"wiFiClientKey", WIFI_CLIENT_KEY, &settings.wiFiClientKey, CHAR_ARRAY, 64},
    {"wiFiClientChannel", WIFI_CLIENT_CHANNEL, &settings.wiFiClientChannel, INT},
    {"wiFiClientDhcpEnabled", WIFI_CLIENT_DHCP_ENABLED, &settings.wiFiClientDhcpEnabled, BOOL},
    {"wiFiClientIPAddress", WIFI_CLIENT_IP_ADDRESS, &settings.wiFiClientIPAddress, CHAR_ARRAY, 16},
    {"wiFiClientNetmask", WIFI_CLIENT_NETMASK, &settings.wiFiClientNetmask, CHAR_ARRAY, 16},
    {"wiFiClientGateway", WIFI_CLIENT_GATEWAY, &settings.wiFiClientGateway, CHAR_ARRAY, 16},
    {"wiFiAPSsid", WIFI_AP_SSID, &settings.wiFiAPSsid, CHAR_ARRAY, 64},
    {"wiFiAPKey", WIFI_AP_KEY, &settings.wiFiAPKey, CHAR_ARRAY, 64},
    {"wiFiAPChannel", WIFI_AP_CHANNEL, &settings.wiFiAPChannel, INT},
    {"tcpPort", TCP_PORT, &settings.tcpPort, INT},
    {"udpIPAddress", UDP_IP_ADDRESS, &settings.udpIPAddress, CHAR_ARRAY, 16},
    {"udpSendPort", UDP_SEND_PORT, &settings.udpSendPort, INT},
    {"udpReceivePort", UDP_RECEIVE_PORT, &settings.udpReceivePort, INT},
    {"synchronisationEnabled", SYNCHRONISATION_ENABLED, &settings.synchronisationEnabled, BOOL},
    {"synchronisationNetworkLatency", SYNCHRONISATION_NETWORK_LATENCY, &settings.synchronisationNetworkLatency, INT},
    {"bluetoothAddress", BLUETOOTH_ADDRESS, &settings.bluetoothAddress, INT},
    {"bluetoothName", BLUETOOTH_NAME, &settings.bluetoothName, CHAR_ARRAY, 32},
    {"bluetoothPinCode", BLUETOOTH_PIN_CODE, &settings.bluetoothPinCode, CHAR_ARRAY, 5},
    {"bluetoothDiscoveryMode", BLUETOOTH_DISCOVERY_MODE, &settings.bluetoothDiscoveryMode, INT},
    {"bluetoothPairedAddress", BLUETOOTH_PAIRED_ADDRESS, &settings.bluetoothPairedAddress, INT},
    {"bluetoothPairedLinkKey", BLUETOOTH_PAIRED_LINK_KEY, &settings.bluetoothPairedLinkKey, INT},
    {"dataLoggerEnabled", DATA_LOGGER_ENABLED, &settings.dataLoggerEnabled, BOOL},
    {"dataLoggerFileNamePrefix", DATA_LOGGER_FILE_NAME_PREFIX, &settings.dataLoggerFileNamePrefix, CHAR_ARRAY, 16},
    {"dataLoggerFileNameTimeEnabled", DATA_LOGGER_FILE_NAME_TIME_ENABLED, &settings.dataLoggerFileNameTimeEnabled, BOOL},
    {"dataLoggerFileNameCounterEnabled", DATA_LOGGER_FILE_NAME_COUNTER_ENABLED, &settings.dataLoggerFileNameCounterEnabled, BOOL},
    {"dataLoggerMaxFileSize", DATA_LOGGER_MAX_FILE_SIZE, &settings.dataLoggerMaxFileSize, INT},
    {"dataLoggerMaxFilePeriod", DATA_LOGGER_MAX_FILE_PERIOD, &settings.dataLoggerMaxFilePeriod, INT},
    {"axesAlignment", AXES_ALIGNMENT, &settings.axesAlignment, INT},
    {"gyroscopeOffsetCorrectionEnabled", GYROSCOPE_OFFSET_CORRECTION_ENABLED, &settings.gyroscopeOffsetCorrectionEnabled, BOOL},
    {"ahrsAxesConvention", AHRS_AXES_CONVENTION, &settings.ahrsAxesConvention, INT},
    {"ahrsGain", AHRS_GAIN, &settings.ahrsGain, FLOAT},
    {"ahrsIgnoreMagnetometer", AHRS_IGNORE_MAGNETOMETER, &settings.ahrsIgnoreMagnetometer, BOOL},
    {"ahrsAccelerationRejectionEnabled", AHRS_ACCELERATION_REJECTION_ENABLED, &settings.ahrsAccelerationRejectionEnabled, BOOL},
    {"ahrsMagneticRejectionEnabled", AHRS_MAGNETIC_REJECTION_ENABLED, &settings.ahrsMagneticRejectionEnabled, BOOL},
    {"binaryModeEnabled", BINARY_MODE_ENABLED, &settings.binaryModeEnabled, BOOL},
    {"usbDataMessagesEnabled", USB_DATA_MESSAGES_ENABLED, &settings.usbDataMessagesEnabled, BOOL},
    {"serialDataMessagesEnabled", SERIAL_DATA_MESSAGES_ENABLED, &settings.serialDataMessagesEnabled, BOOL},
    {"tcpDataMessagesEnabled", TCP_DATA_MESSAGES_ENABLED, &settings.tcpDataMessagesEnabled, BOOL},
    {"udpDataMessagesEnabled", UDP_DATA_MESSAGES_ENABLED, &settings.udpDataMessagesEnabled, BOOL},
    {"bluetoothDataMessagesEnabled", BLUETOOTH_DATA_MESSAGES_ENABLED, &settings.bluetoothDataMessagesEnabled, BOOL},
    {"dataLoggerDataMessagesEnabled", DATA_LOGGER_DATA_MESSAGES_ENABLED, &settings.dataLoggerDataMessagesEnabled, BOOL},
    {"ahrsMessageType", AHRS_MESSAGE_TYPE, &settings.ahrsMessageType, INT},
    {"inertialMessageRateDivisor", INERTIAL_MESSAGE_RATE_DIVISOR, &settings.inertialMessageRateDivisor, INT},
    {"magnetometerMessageRateDivisor", MAGNETOMETER_MESSAGE_RATE_DIVISOR, &settings.magnetometerMessageRateDivisor, INT},
    {"ahrsMessageRateDivisor", AHRS_MESSAGE_RATE_DIVISOR, &settings.ahrsMessageRateDivisor, INT},
    {"highGAccelerometerMessageRateDivisor", HIGHG_ACCELEROMETER_MESSAGE_RATE_DIVISOR, &settings.highGAccelerometerMessageRateDivisor, INT},
    {"temperatureMessageRateDivisor", TEMPERATURE_MESSAGE_RATE_DIVISOR, &settings.temperatureMessageRateDivisor, INT},
    {"batteryMessageRateDivisor", BATTERY_MESSAGE_RATE_DIVISOR, &settings.batteryMessageRateDivisor, INT},
    {"rssiMessageRateDivisor", RSSI_MESSAGE_RATE_DIVISOR, &settings.rssiMessageRateDivisor, INT},
};

bool loadConfigurationsFromJSON(bool checkFile, const char* filename) {
    if (checkFile) {
        _file = SPIFFS.open(filename, "r");
        if (!_file) {
            return false;
        }

        // Read and parse the JSON data
        DeserializationError error = deserializeJson(_jsonConfigDoc, _file);
        if (error) {
            return false;
        }

        // Close the file
        _file.close();
    }

    // Extract configuration settings from the JSON
    strcpy(settings.calibrationDate, _jsonConfigDoc["calibrationDate"]); // "Unknown"

    JsonArray gyroscopeMisalignment = _jsonConfigDoc["gyroscopeMisalignment"];
    for (size_t i=0; i<9; i++) { settings.gyroscopeMisalignment[i] = gyroscopeMisalignment[i]; }

    JsonArray gyroscopeSensitivity = _jsonConfigDoc["gyroscopeSensitivity"];
    for (size_t i=0; i<3; i++) { settings.gyroscopeSensitivity[i] = gyroscopeSensitivity[i]; }

    JsonArray gyroscopeOffset = _jsonConfigDoc["gyroscopeOffset"];
    for (size_t i=0; i<3; i++) { settings.gyroscopeOffset[i] = gyroscopeOffset[i]; }

    JsonArray accelerometerMisalignment = _jsonConfigDoc["accelerometerMisalignment"];
    for (size_t i=0; i<9; i++) { settings.accelerometerMisalignment[i] = accelerometerMisalignment[i]; }

    JsonArray accelerometerSensitivity = _jsonConfigDoc["accelerometerSensitivity"];
    for (size_t i=0; i<3; i++) { settings.accelerometerSensitivity[i] = accelerometerSensitivity[i]; }

    JsonArray accelerometerOffset = _jsonConfigDoc["accelerometerOffset"];
    for (size_t i=0; i<3; i++) { settings.accelerometerOffset[i] = accelerometerOffset[i]; }

    JsonArray softIronMatrix = _jsonConfigDoc["softIronMatrix"];
    for (size_t i=0; i<9; i++) { settings.softIronMatrix[i] = softIronMatrix[i]; }

    JsonArray hardIronOffset = _jsonConfigDoc["hardIronOffset"];
    for (size_t i=0; i<3; i++) { settings.hardIronOffset[i] = hardIronOffset[i]; }

    JsonArray highGAccelerometerMisalignment = _jsonConfigDoc["highGAccelerometerMisalignment"];
    for (size_t i=0; i<9; i++) { settings.highGAccelerometerMisalignment[i] = highGAccelerometerMisalignment[i]; }

    JsonArray highGAccelerometerSensitivity = _jsonConfigDoc["highGAccelerometerSensitivity"];
    for (size_t i=0; i<3; i++) { settings.highGAccelerometerSensitivity[i] = highGAccelerometerSensitivity[i]; }

    JsonArray highGAccelerometerOffset = _jsonConfigDoc["highGAccelerometerOffset"];
    for (size_t i=0; i<3; i++) { settings.highGAccelerometerOffset[i] = highGAccelerometerOffset[i]; }

    strcpy(settings.deviceName,               _jsonConfigDoc["deviceName"]); // "x-IMU3"
    strcpy(settings.serialNumber,             _jsonConfigDoc["serialNumber"]); // "Unknown"
    strcpy(settings.firmwareVersion,          _jsonConfigDoc["firmwareVersion"]); // "Unknown"
    strcpy(settings.bootloaderVersion,        _jsonConfigDoc["bootloaderVersion"]); // "Unknown"
    strcpy(settings.hardwareVersion,          _jsonConfigDoc["hardwareVersion"]); // "Unknown"
    strcpy(settings.wirelessFirmwareVersion,  _jsonConfigDoc["wirelessFirmwareVersion"]); // "Unknown"
    strcpy(settings.wiFiMacAddress,           _jsonConfigDoc["wiFiMacAddress"]); // "0"
    strcpy(settings.wiFiIPAddress,            _jsonConfigDoc["wiFiIPAddress"]); // "0"
    strcpy(settings.wiFiClientSsid,           _jsonConfigDoc["wiFiClientSsid"]); // "x-IMU3 Network"
    strcpy(settings.wiFiClientKey,            _jsonConfigDoc["wiFiClientKey"]); // "xiotechnologies"
    strcpy(settings.wiFiClientIPAddress,      _jsonConfigDoc["wiFiClientIPAddress"]); // "192.168.1.2"
    strcpy(settings.wiFiClientNetmask,        _jsonConfigDoc["wiFiClientNetmask"]); // "255.255.255.0"
    strcpy(settings.wiFiClientGateway,        _jsonConfigDoc["wiFiClientGateway"]); // "192.168.1.1"
    strcpy(settings.wiFiAPSsid,               _jsonConfigDoc["wiFiAPSsid"]); // nullptr
    strcpy(settings.wiFiAPKey,                _jsonConfigDoc["wiFiAPKey"]); // nullptr
    strcpy(settings.udpIPAddress,             _jsonConfigDoc["udpIPAddress"]); // "0"
    strcpy(settings.bluetoothName,            _jsonConfigDoc["bluetoothName"]); // nullptr
    strcpy(settings.bluetoothPinCode,         _jsonConfigDoc["bluetoothPinCode"]); // "1234"
    strcpy(settings.dataLoggerFileNamePrefix, _jsonConfigDoc["dataLoggerFileNamePrefix"]); // nullptr

    settings.serialMode                               = _jsonConfigDoc["serialMode"]; // 0
    settings.serialBaudRate                           = _jsonConfigDoc["serialBaudRate"]; // 115200
    settings.serialRtsCtsEnabled                      = _jsonConfigDoc["serialRtsCtsEnabled"]; // false
    settings.serialAccessoryNumberOfBytes             = _jsonConfigDoc["serialAccessoryNumberOfBytes"]; // 1024
    settings.serialAccessoryTerminationByte           = _jsonConfigDoc["serialAccessoryTerminationByte"]; // 10
    settings.serialAccessoryTimeout                   = _jsonConfigDoc["serialAccessoryTimeout"]; // 100
    settings.wirelessMode                             = _jsonConfigDoc["wirelessMode"]; // 2
    settings.externalAntennaeEnabled                  = _jsonConfigDoc["externalAntennaeEnabled"]; // false
    settings.wiFiRegion                               = _jsonConfigDoc["wiFiRegion"]; // 1
    settings.wiFiClientChannel                        = _jsonConfigDoc["wiFiClientChannel"]; // 0
    settings.wiFiClientDhcpEnabled                    = _jsonConfigDoc["wiFiClientDhcpEnabled"]; // true
    settings.wiFiAPChannel                            = _jsonConfigDoc["wiFiAPChannel"]; // 36
    settings.tcpPort                                  = _jsonConfigDoc["tcpPort"]; // 7000
    settings.udpSendPort                              = _jsonConfigDoc["udpSendPort"]; // 0
    settings.udpReceivePort                           = _jsonConfigDoc["udpReceivePort"]; // 9000
    settings.synchronisationEnabled                   = _jsonConfigDoc["synchronisationEnabled"]; // true
    settings.synchronisationNetworkLatency            = _jsonConfigDoc["synchronisationNetworkLatency"]; // 1500
    settings.bluetoothAddress                         = _jsonConfigDoc["bluetoothAddress"]; // 0
    settings.bluetoothDiscoveryMode                   = _jsonConfigDoc["bluetoothDiscoveryMode"]; // 2
    settings.bluetoothPairedAddress                   = _jsonConfigDoc["bluetoothPairedAddress"]; // 0
    settings.bluetoothPairedLinkKey                   = _jsonConfigDoc["bluetoothPairedLinkKey"]; // 0
    settings.dataLoggerEnabled                        = _jsonConfigDoc["dataLoggerEnabled"]; // false
    settings.dataLoggerFileNameTimeEnabled            = _jsonConfigDoc["dataLoggerFileNameTimeEnabled"]; // true
    settings.dataLoggerFileNameCounterEnabled         = _jsonConfigDoc["dataLoggerFileNameCounterEnabled"]; // false
    settings.dataLoggerMaxFileSize                    = _jsonConfigDoc["dataLoggerMaxFileSize"]; // 0
    settings.dataLoggerMaxFilePeriod                  = _jsonConfigDoc["dataLoggerMaxFilePeriod"]; // 0
    settings.axesAlignment                            = _jsonConfigDoc["axesAlignment"]; // 0
    settings.gyroscopeOffsetCorrectionEnabled         = _jsonConfigDoc["gyroscopeOffsetCorrectionEnabled"]; // true
    settings.ahrsAxesConvention                       = _jsonConfigDoc["ahrsAxesConvention"]; // 0
    settings.ahrsGain                                 = _jsonConfigDoc["ahrsGain"]; // 0.5
    settings.ahrsIgnoreMagnetometer                   = _jsonConfigDoc["ahrsIgnoreMagnetometer"]; // false
    settings.ahrsAccelerationRejectionEnabled         = _jsonConfigDoc["ahrsAccelerationRejectionEnabled"]; // true
    settings.ahrsMagneticRejectionEnabled             = _jsonConfigDoc["ahrsMagneticRejectionEnabled"]; // true
    settings.binaryModeEnabled                        = _jsonConfigDoc["binaryModeEnabled"]; // true
    settings.usbDataMessagesEnabled                   = _jsonConfigDoc["usbDataMessagesEnabled"]; // true
    settings.serialDataMessagesEnabled                = _jsonConfigDoc["serialDataMessagesEnabled"]; // true
    settings.tcpDataMessagesEnabled                   = _jsonConfigDoc["tcpDataMessagesEnabled"]; // true
    settings.udpDataMessagesEnabled                   = _jsonConfigDoc["udpDataMessagesEnabled"]; // true
    settings.bluetoothDataMessagesEnabled             = _jsonConfigDoc["bluetoothDataMessagesEnabled"]; // true
    settings.dataLoggerDataMessagesEnabled            = _jsonConfigDoc["dataLoggerDataMessagesEnabled"]; // true
    settings.ahrsMessageType                          = _jsonConfigDoc["ahrsMessageType"]; // 0
    settings.inertialMessageRateDivisor               = _jsonConfigDoc["inertialMessageRateDivisor"]; // 8
    settings.magnetometerMessageRateDivisor           = _jsonConfigDoc["magnetometerMessageRateDivisor"]; // 1
    settings.ahrsMessageRateDivisor                   = _jsonConfigDoc["ahrsMessageRateDivisor"]; // 8
    settings.highGAccelerometerMessageRateDivisor     = _jsonConfigDoc["highGAccelerometerMessageRateDivisor"]; // 32
    settings.temperatureMessageRateDivisor            = _jsonConfigDoc["temperatureMessageRateDivisor"]; // 5
    settings.batteryMessageRateDivisor                = _jsonConfigDoc["batteryMessageRateDivisor"]; // 5
    settings.rssiMessageRateDivisor                   = _jsonConfigDoc["rssiMessageRateDivisor"]; // 1

    // Clear the JSON buffer
    // _jsonConfigDoc.clear();
    return true;
}

bool saveConfigurations() {
    _file = SPIFFS.open(CONFIG_FILE_NAME, "w");

    if (!_file) {
        return false;
    }

    // Set the configuration settings in the JSON document
    _jsonConfigDoc["calibrationDate"] = settings.calibrationDate;

    JsonArray gyroscopeMisalignment;
    for (size_t i=0; i<9; i++) { gyroscopeMisalignment.add(settings.gyroscopeMisalignment[i]); }
    _jsonConfigDoc["gyroscopeMisalignment"] = gyroscopeMisalignment;

    JsonArray gyroscopeSensitivity;
    for (size_t i=0; i<3; i++) { gyroscopeSensitivity.add(settings.gyroscopeSensitivity[i]); }
    _jsonConfigDoc["gyroscopeSensitivity"] = gyroscopeSensitivity;

    JsonArray gyroscopeOffset;
    for (size_t i=0; i<3; i++) { gyroscopeOffset.add(settings.gyroscopeOffset[i]); }
    _jsonConfigDoc["gyroscopeOffset"] = gyroscopeOffset;

    JsonArray accelerometerMisalignment;
    for (size_t i=0; i<9; i++) { accelerometerMisalignment.add(settings.accelerometerMisalignment[i]); }
    _jsonConfigDoc["accelerometerMisalignment"] = accelerometerMisalignment;

    JsonArray accelerometerSensitivity;
    for (size_t i=0; i<3; i++) { accelerometerSensitivity.add(settings.accelerometerSensitivity[i]); }
    _jsonConfigDoc["accelerometerSensitivity"] = accelerometerSensitivity;

    JsonArray accelerometerOffset;
    for (size_t i=0; i<3; i++) { accelerometerOffset.add(settings.accelerometerOffset[i]); }
    _jsonConfigDoc["accelerometerOffset"] = accelerometerOffset;

    JsonArray softIronMatrix;
    for (size_t i=0; i<9; i++) { softIronMatrix.add(settings.softIronMatrix[i]); }
    _jsonConfigDoc["softIronMatrix"] = softIronMatrix;

    JsonArray hardIronOffset;
    for (size_t i=0; i<3; i++) { hardIronOffset.add(settings.hardIronOffset[i]); }
    _jsonConfigDoc["hardIronOffset"] = hardIronOffset;

    JsonArray highGAccelerometerMisalignment;
    for (size_t i=0; i<9; i++) { highGAccelerometerMisalignment.add(settings.highGAccelerometerMisalignment[i]); }
    _jsonConfigDoc["highGAccelerometerMisalignment"] = highGAccelerometerMisalignment;

    JsonArray highGAccelerometerSensitivity;
    for (size_t i=0; i<3; i++) { highGAccelerometerSensitivity.add(settings.highGAccelerometerSensitivity[i]); }
    _jsonConfigDoc["highGAccelerometerSensitivity"] = highGAccelerometerSensitivity;

    JsonArray highGAccelerometerOffset;
    for (size_t i=0; i<3; i++) { highGAccelerometerOffset.add(settings.highGAccelerometerOffset[i]); }
    _jsonConfigDoc["highGAccelerometerOffset"] = highGAccelerometerOffset;

    _jsonConfigDoc["deviceName"]                            = settings.deviceName;
    _jsonConfigDoc["serialNumber"]                          = settings.serialNumber;
    _jsonConfigDoc["firmwareVersion"]                       = settings.firmwareVersion;
    _jsonConfigDoc["bootloaderVersion"]                     = settings.bootloaderVersion;
    _jsonConfigDoc["hardwareVersion"]                       = settings.hardwareVersion;
    _jsonConfigDoc["serialBaudRate"]                        = settings.serialBaudRate;
    _jsonConfigDoc["serialRtsCtsEnabled"]                   = settings.serialRtsCtsEnabled;
    _jsonConfigDoc["serialAccessoryNumberOfBytes"]          = settings.serialAccessoryNumberOfBytes;
    _jsonConfigDoc["serialAccessoryTerminationByte"]        = settings.serialAccessoryTerminationByte;
    _jsonConfigDoc["serialAccessoryTimeout"]                = settings.serialAccessoryTimeout;
    _jsonConfigDoc["wirelessMode"]                          = settings.wirelessMode;
    _jsonConfigDoc["wirelessFirmwareVersion"]               = settings.wirelessFirmwareVersion;
    _jsonConfigDoc["externalAntennaeEnabled"]               = settings.externalAntennaeEnabled;
    _jsonConfigDoc["wiFiRegion"]                            = settings.wiFiRegion;
    _jsonConfigDoc["wiFiMacAddress"]                        = settings.wiFiMacAddress;
    _jsonConfigDoc["wiFiIPAddress"]                         = settings.wiFiIPAddress;
    _jsonConfigDoc["wiFiClientSsid"]                        = settings.wiFiClientSsid;
    _jsonConfigDoc["wiFiClientKey"]                         = settings.wiFiClientKey;
    _jsonConfigDoc["wiFiClientChannel"]                     = settings.wiFiClientChannel;
    _jsonConfigDoc["wiFiClientDhcpEnabled"]                 = settings.wiFiClientDhcpEnabled;
    _jsonConfigDoc["wiFiClientIPAddress"]                   = settings.wiFiClientIPAddress;
    _jsonConfigDoc["wiFiClientNetmask"]                     = settings.wiFiClientNetmask;
    _jsonConfigDoc["wiFiClientGateway"]                     = settings.wiFiClientGateway;
    _jsonConfigDoc["wiFiAPSsid"]                            = settings.wiFiAPSsid;
    _jsonConfigDoc["wiFiAPKey"]                             = settings.wiFiAPKey;
    _jsonConfigDoc["wiFiAPChannel"]                         = settings.wiFiAPChannel;
    _jsonConfigDoc["tcpPort"]                               = settings.tcpPort;
    _jsonConfigDoc["udpIPAddress"]                          = settings.udpIPAddress;
    _jsonConfigDoc["udpSendPort"]                           = settings.udpSendPort;
    _jsonConfigDoc["udpReceivePort"]                        = settings.udpReceivePort;
    _jsonConfigDoc["synchronisationEnabled"]                = settings.synchronisationEnabled;
    _jsonConfigDoc["synchronisationNetworkLatency"]         = settings.synchronisationNetworkLatency;
    _jsonConfigDoc["bluetoothAddress"]                      = settings.bluetoothAddress;
    _jsonConfigDoc["bluetoothName"]                         = settings.bluetoothName;
    _jsonConfigDoc["bluetoothPinCode"]                      = settings.bluetoothPinCode;
    _jsonConfigDoc["bluetoothDiscoveryMode"]                = settings.bluetoothDiscoveryMode;
    _jsonConfigDoc["bluetoothPairedAddress"]                = settings.bluetoothPairedAddress;
    _jsonConfigDoc["bluetoothPairedLinkKey"]                = settings.bluetoothPairedLinkKey;
    _jsonConfigDoc["dataLoggerEnabled"]                     = settings.dataLoggerEnabled;
    _jsonConfigDoc["dataLoggerFileNamePrefix"]              = settings.dataLoggerFileNamePrefix;
    _jsonConfigDoc["dataLoggerFileNameTimeEnabled"]         = settings.dataLoggerFileNameTimeEnabled;
    _jsonConfigDoc["dataLoggerFileNameCounterEnabled"]      = settings.dataLoggerFileNameCounterEnabled;
    _jsonConfigDoc["dataLoggerMaxFileSize"]                 = settings.dataLoggerMaxFileSize;
    _jsonConfigDoc["dataLoggerMaxFilePeriod"]               = settings.dataLoggerMaxFilePeriod;
    _jsonConfigDoc["axesAlignment"]                         = settings.axesAlignment;
    _jsonConfigDoc["gyroscopeOffsetCorrectionEnabled"]      = settings.gyroscopeOffsetCorrectionEnabled;
    _jsonConfigDoc["ahrsAxesConvention"]                    = settings.ahrsAxesConvention;
    _jsonConfigDoc["ahrsGain"]                              = settings.ahrsGain;
    _jsonConfigDoc["ahrsIgnoreMagnetometer"]                = settings.ahrsIgnoreMagnetometer;
    _jsonConfigDoc["ahrsAccelerationRejectionEnabled"]      = settings.ahrsAccelerationRejectionEnabled;
    _jsonConfigDoc["ahrsMagneticRejectionEnabled"]          = settings.ahrsMagneticRejectionEnabled;
    _jsonConfigDoc["binaryModeEnabled"]                     = settings.binaryModeEnabled;
    _jsonConfigDoc["usbDataMessagesEnabled"]                = settings.usbDataMessagesEnabled;
    _jsonConfigDoc["serialDataMessagesEnabled"]             = settings.serialDataMessagesEnabled;
    _jsonConfigDoc["tcpDataMessagesEnabled"]                = settings.tcpDataMessagesEnabled;
    _jsonConfigDoc["udpDataMessagesEnabled"]                = settings.udpDataMessagesEnabled;
    _jsonConfigDoc["bluetoothDataMessagesEnabled"]          = settings.bluetoothDataMessagesEnabled;
    _jsonConfigDoc["dataLoggerDataMessagesEnabled"]         = settings.dataLoggerDataMessagesEnabled;
    _jsonConfigDoc["ahrsMessageType"]                       = settings.ahrsMessageType;
    _jsonConfigDoc["inertialMessageRateDivisor"]            = settings.inertialMessageRateDivisor;
    _jsonConfigDoc["magnetometerMessageRateDivisor"]        = settings.magnetometerMessageRateDivisor;
    _jsonConfigDoc["ahrsMessageRateDivisor"]                = settings.ahrsMessageRateDivisor;
    _jsonConfigDoc["highGAccelerometerMessageRateDivisor"]  = settings.highGAccelerometerMessageRateDivisor;
    _jsonConfigDoc["temperatureMessageRateDivisor"]         = settings.temperatureMessageRateDivisor;
    _jsonConfigDoc["batteryMessageRateDivisor"]             = settings.batteryMessageRateDivisor;
    _jsonConfigDoc["rssiMessageRateDivisor"]                = settings.rssiMessageRateDivisor;

    // Serialize the JSON document to the file
    if (!serializeJson(_jsonConfigDoc, _file)) {
        return false;
    }

    // Close the file and empty buffer
    _file.close();

    return true;
}

void updateSetting(const settingTableEntry* entry, JsonVariant newValue) {
    bool* boolPtr;
    uint8_t* uint8Ptr;
    char* charPtr;
    float* floatPtr;
    int* intPtr;

    switch (entry->type) {
        case BOOL: 
            boolPtr = static_cast<bool*>(entry->value); // Cast the value pointer to bool*
            *boolPtr = newValue.as<bool>(); // Assign the new value to the setting value
            break;
        case CHAR: 
            uint8Ptr = static_cast<uint8_t*>(entry->value); // Cast the value pointer to uint8_t*
            *uint8Ptr = newValue.as<uint8_t>(); // Assign the new value to the setting value
            break;
        case FLOAT: 
            floatPtr = static_cast<float*>(entry->value); // Cast the value pointer to float*
            *floatPtr = newValue.as<float>(); // Assign the new value to the setting value
            break;
        case INT: 
            intPtr = static_cast<int*>(entry->value); // Cast the value pointer to int*
            *intPtr = newValue.as<int>(); // Assign the new value to the setting value
            break;
        case FLOAT_ARRAY:
            floatPtr = static_cast<float*>(entry->value); // Cast the value pointer to float*
            for (size_t i=0; i<entry->len; i++) { // Copy the new array values to the setting value
                floatPtr[i] = newValue[i];
            }
            break;
        case CHAR_ARRAY:
            charPtr = static_cast<char*>(entry->value); // Cast the value pointer to char*
            strncpy(charPtr, newValue, entry->len - 1); // Copy the new value to the setting value
            charPtr[entry->len - 1] = '\0'; // Null-terminate the string
            break;
        default:
            return;
    }
}



