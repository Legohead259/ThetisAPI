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
    for (size_t i=0; i<9; i++) { settings.gyroscopeOffset[i] = gyroscopeOffset[i]; }

    JsonArray accelerometerMisalignment = _jsonConfigDoc["accelerometerMisalignment"];
    for (size_t i=0; i<9; i++) { settings.accelerometerMisalignment[i] = accelerometerMisalignment[i]; }

    JsonArray accelerometerSensitivity = _jsonConfigDoc["accelerometerSensitivity"];
    for (size_t i=0; i<9; i++) { settings.accelerometerSensitivity[i] = accelerometerSensitivity[i]; }

    JsonArray accelerometerOffset = _jsonConfigDoc["accelerometerOffset"];
    for (size_t i=0; i<9; i++) { settings.accelerometerOffset[i] = accelerometerOffset[i]; }

    JsonArray softIronMatrix = _jsonConfigDoc["softIronMatrix"];
    for (size_t i=0; i<9; i++) { settings.softIronMatrix[i] = softIronMatrix[i]; }

    JsonArray hardIronOffset = _jsonConfigDoc["hardIronOffset"];
    for (size_t i=0; i<9; i++) { settings.hardIronOffset[i] = hardIronOffset[i]; }

    JsonArray highGAccelerometerMisalignment = _jsonConfigDoc["highGAccelerometerMisalignment"];
    for (size_t i=0; i<9; i++) { settings.highGAccelerometerMisalignment[i] = highGAccelerometerMisalignment[i]; }

    JsonArray highGAccelerometerSensitivity = _jsonConfigDoc["highGAccelerometerSensitivity"];
    for (size_t i=0; i<9; i++) { settings.highGAccelerometerSensitivity[i] = highGAccelerometerSensitivity[i]; }

    JsonArray highGAccelerometerOffset = _jsonConfigDoc["highGAccelerometerOffset"];
    for (size_t i=0; i<9; i++) { settings.highGAccelerometerOffset[i] = highGAccelerometerOffset[i]; }

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
            Serial.println("Updating CHAR ARRAY");
            charPtr = static_cast<char*>(entry->value); // Cast the value pointer to char*
            strncpy(charPtr, newValue, entry->len - 1); // Copy the new value to the setting value
            charPtr[entry->len - 1] = '\0'; // Null-terminate the string
            break;
        default:
            return;
    }
}


