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

bool loadConfigurationsFromJSON(bool checkFile) {
    if (checkFile) {
        _file = SPIFFS.open(CONFIG_FILE_NAME, "r");
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
    // strcpy(deviceSettings.calibrationDate, _doc["calibrationDate"]); // "Unknown"

    JsonArray gyroscopeMisalignment = _jsonConfigDoc["gyroscopeMisalignment"];
    settings.gyroscopeMisalignment[0] = gyroscopeMisalignment[0]; // 1
    settings.gyroscopeMisalignment[1] = gyroscopeMisalignment[1]; // 0
    settings.gyroscopeMisalignment[2] = gyroscopeMisalignment[2]; // 0
    settings.gyroscopeMisalignment[3] = gyroscopeMisalignment[3]; // 0
    settings.gyroscopeMisalignment[4] = gyroscopeMisalignment[4]; // 1
    settings.gyroscopeMisalignment[5] = gyroscopeMisalignment[5]; // 0
    settings.gyroscopeMisalignment[6] = gyroscopeMisalignment[6]; // 0
    settings.gyroscopeMisalignment[7] = gyroscopeMisalignment[7]; // 0
    settings.gyroscopeMisalignment[8] = gyroscopeMisalignment[8]; // 1

    JsonArray gyroscopeSensitivity = _jsonConfigDoc["gyroscopeSensitivity"];
    settings.gyroscopeSensitivity[0] = gyroscopeSensitivity[0]; // 1
    settings.gyroscopeSensitivity[1] = gyroscopeSensitivity[1]; // 1
    settings.gyroscopeSensitivity[2] = gyroscopeSensitivity[2]; // 1

    JsonArray gyroscopeOffset = _jsonConfigDoc["gyroscopeOffset"];
    settings.gyroscopeOffset[0] = gyroscopeOffset[0]; // 0
    settings.gyroscopeOffset[1] = gyroscopeOffset[1]; // 0
    settings.gyroscopeOffset[2] = gyroscopeOffset[2]; // 0

    JsonArray accelerometerMisalignment = _jsonConfigDoc["accelerometerMisalignment"];
    settings.accelerometerMisalignment[0] = accelerometerMisalignment[0]; // 1
    settings.accelerometerMisalignment[1] = accelerometerMisalignment[1]; // 0
    settings.accelerometerMisalignment[2] = accelerometerMisalignment[2]; // 0
    settings.accelerometerMisalignment[3] = accelerometerMisalignment[3]; // 0
    settings.accelerometerMisalignment[4] = accelerometerMisalignment[4]; // 1
    settings.accelerometerMisalignment[5] = accelerometerMisalignment[5]; // 0
    settings.accelerometerMisalignment[6] = accelerometerMisalignment[6]; // 0
    settings.accelerometerMisalignment[7] = accelerometerMisalignment[7]; // 0
    settings.accelerometerMisalignment[8] = accelerometerMisalignment[8]; // 1

    JsonArray accelerometerSensitivity = _jsonConfigDoc["accelerometerSensitivity"];
    settings.accelerometerSensitivity[0] = accelerometerSensitivity[0]; // 1
    settings.accelerometerSensitivity[1] = accelerometerSensitivity[1]; // 1
    settings.accelerometerSensitivity[2] = accelerometerSensitivity[2]; // 1

    JsonArray accelerometerOffset = _jsonConfigDoc["accelerometerOffset"];
    settings.accelerometerOffset[0] = accelerometerOffset[0]; // 0
    settings.accelerometerOffset[1] = accelerometerOffset[1]; // 0
    settings.accelerometerOffset[2] = accelerometerOffset[2]; // 0

    JsonArray softIronMatrix = _jsonConfigDoc["softIronMatrix"];
    settings.softIronMatrix[0] = softIronMatrix[0]; // 1
    settings.softIronMatrix[1] = softIronMatrix[1]; // 0
    settings.softIronMatrix[2] = softIronMatrix[2]; // 0
    settings.softIronMatrix[3] = softIronMatrix[3]; // 0
    settings.softIronMatrix[4] = softIronMatrix[4]; // 1
    settings.softIronMatrix[5] = softIronMatrix[5]; // 0
    settings.softIronMatrix[6] = softIronMatrix[6]; // 0
    settings.softIronMatrix[7] = softIronMatrix[7]; // 0
    settings.softIronMatrix[8] = softIronMatrix[8]; // 1

    JsonArray hardIronOffset = _jsonConfigDoc["hardIronOffset"];
    settings.hardIronOffset[0] = hardIronOffset[0]; // 0
    settings.hardIronOffset[1] = hardIronOffset[1]; // 0
    settings.hardIronOffset[2] = hardIronOffset[2]; // 0

    JsonArray highGAccelerometerMisalignment = _jsonConfigDoc["highGAccelerometerMisalignment"];
    settings.highGAccelerometerMisalignment[0] = highGAccelerometerMisalignment[0]; // 1
    settings.highGAccelerometerMisalignment[1] = highGAccelerometerMisalignment[1]; // 0
    settings.highGAccelerometerMisalignment[2] = highGAccelerometerMisalignment[2]; // 0
    settings.highGAccelerometerMisalignment[3] = highGAccelerometerMisalignment[3]; // 0
    settings.highGAccelerometerMisalignment[4] = highGAccelerometerMisalignment[4]; // 1
    settings.highGAccelerometerMisalignment[5] = highGAccelerometerMisalignment[5]; // 0
    settings.highGAccelerometerMisalignment[6] = highGAccelerometerMisalignment[6]; // 0
    settings.highGAccelerometerMisalignment[7] = highGAccelerometerMisalignment[7]; // 0
    settings.highGAccelerometerMisalignment[8] = highGAccelerometerMisalignment[8]; // 1

    JsonArray highGAccelerometerSensitivity = _jsonConfigDoc["highGAccelerometerSensitivity"];
    settings.highGAccelerometerSensitivity[0] = highGAccelerometerSensitivity[0]; // 1
    settings.highGAccelerometerSensitivity[1] = highGAccelerometerSensitivity[1]; // 1
    settings.highGAccelerometerSensitivity[2] = highGAccelerometerSensitivity[2]; // 1

    JsonArray highGAccelerometerOffset = _jsonConfigDoc["highGAccelerometerOffset"];
    settings.highGAccelerometerOffset[0] = highGAccelerometerOffset[0]; // 0
    settings.highGAccelerometerOffset[1] = highGAccelerometerOffset[1]; // 0
    settings.highGAccelerometerOffset[2] = highGAccelerometerOffset[2]; // 0

    strcpy(settings.deviceName,               _jsonConfigDoc["deviceName"]);
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

void saveConfigurations() {
    // _file = SPIFFS.open(CONFIG_FILE_NAME, "w");

    // if (!_file) {
    //     return false;
    // }

    // // Set the configuration settings in the JSON document
    // // jsonDoc["calibrationDate"] = settings.calibrationDate;
    // // jsonDoc["deviceName"] = settings.deviceName;
    // // jsonDoc["serialNumber"] = settings.serialNumber;

    // // Serialize the JSON document to the file
    // if (!serializeJson(jsonDoc, _file)) {
    //     return false;
    // }

    // // Close the file and empty buffer
    // _file.close();
    // jsonDoc.clear();

    // return true;
}

// bool updateSetting(unsigned long keyHash, const T& value) {

// }

// void updateSetting(const char* key, float* value[][]) {
//     Serial.println("Got matrix");
// }

