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

device_settings_t deviceSettings;

bool loadConfigurationsFromJSON() {
    File _file = SPIFFS.open(CONFIG_FILE_NAME, "r");
    if (!_file) {
        return false;
    }

    // Allocate a buffer to hold the JSON data
    StaticJsonDocument<6144> _doc;

    // Read and parse the JSON data
    DeserializationError error = deserializeJson(_doc, _file);
    if (error) {
        return false;
    }

    // Close the file
    _file.close();

    // Extract configuration settings from the JSON
    // strcpy(deviceSettings.calibrationDate, _doc["calibrationDate"]); // "Unknown"

    JsonArray gyroscopeMisalignment = _doc["gyroscopeMisalignment"];
    deviceSettings.gyroscopeMisalignment[0][0] = gyroscopeMisalignment[0]; // 1
    deviceSettings.gyroscopeMisalignment[0][1] = gyroscopeMisalignment[1]; // 0
    deviceSettings.gyroscopeMisalignment[0][2] = gyroscopeMisalignment[2]; // 0
    deviceSettings.gyroscopeMisalignment[1][0] = gyroscopeMisalignment[3]; // 0
    deviceSettings.gyroscopeMisalignment[1][1] = gyroscopeMisalignment[4]; // 1
    deviceSettings.gyroscopeMisalignment[1][2] = gyroscopeMisalignment[5]; // 0
    deviceSettings.gyroscopeMisalignment[2][0] = gyroscopeMisalignment[6]; // 0
    deviceSettings.gyroscopeMisalignment[2][1] = gyroscopeMisalignment[7]; // 0
    deviceSettings.gyroscopeMisalignment[2][2] = gyroscopeMisalignment[8]; // 1

    JsonArray gyroscopeSensitivity = _doc["gyroscopeSensitivity"];
    deviceSettings.gyroscopeSensitivity[0] = gyroscopeSensitivity[0]; // 1
    deviceSettings.gyroscopeSensitivity[1] = gyroscopeSensitivity[1]; // 1
    deviceSettings.gyroscopeSensitivity[2] = gyroscopeSensitivity[2]; // 1

    JsonArray gyroscopeOffset = _doc["gyroscopeOffset"];
    deviceSettings.gyroscopeOffset[0] = gyroscopeOffset[0]; // 0
    deviceSettings.gyroscopeOffset[1] = gyroscopeOffset[1]; // 0
    deviceSettings.gyroscopeOffset[2] = gyroscopeOffset[2]; // 0

    JsonArray accelerometerMisalignment = _doc["accelerometerMisalignment"];
    deviceSettings.accelerometerMisalignment[0][0] = accelerometerMisalignment[0]; // 1
    deviceSettings.accelerometerMisalignment[0][1] = accelerometerMisalignment[1]; // 0
    deviceSettings.accelerometerMisalignment[0][2] = accelerometerMisalignment[2]; // 0
    deviceSettings.accelerometerMisalignment[1][0] = accelerometerMisalignment[3]; // 0
    deviceSettings.accelerometerMisalignment[1][1] = accelerometerMisalignment[4]; // 1
    deviceSettings.accelerometerMisalignment[1][2] = accelerometerMisalignment[5]; // 0
    deviceSettings.accelerometerMisalignment[2][0] = accelerometerMisalignment[6]; // 0
    deviceSettings.accelerometerMisalignment[2][1] = accelerometerMisalignment[7]; // 0
    deviceSettings.accelerometerMisalignment[2][2] = accelerometerMisalignment[8]; // 1

    JsonArray accelerometerSensitivity = _doc["accelerometerSensitivity"];
    deviceSettings.accelerometerSensitivity[0] = accelerometerSensitivity[0]; // 1
    deviceSettings.accelerometerSensitivity[1] = accelerometerSensitivity[1]; // 1
    deviceSettings.accelerometerSensitivity[2] = accelerometerSensitivity[2]; // 1

    JsonArray accelerometerOffset = _doc["accelerometerOffset"];
    deviceSettings.accelerometerOffset[0] = accelerometerOffset[0]; // 0
    deviceSettings.accelerometerOffset[1] = accelerometerOffset[1]; // 0
    deviceSettings.accelerometerOffset[2] = accelerometerOffset[2]; // 0

    JsonArray softIronMatrix = _doc["softIronMatrix"];
    deviceSettings.softIronMatrix[0][0] = softIronMatrix[0]; // 1
    deviceSettings.softIronMatrix[0][1] = softIronMatrix[1]; // 0
    deviceSettings.softIronMatrix[0][2] = softIronMatrix[2]; // 0
    deviceSettings.softIronMatrix[1][0] = softIronMatrix[3]; // 0
    deviceSettings.softIronMatrix[1][1] = softIronMatrix[4]; // 1
    deviceSettings.softIronMatrix[1][2] = softIronMatrix[5]; // 0
    deviceSettings.softIronMatrix[2][0] = softIronMatrix[6]; // 0
    deviceSettings.softIronMatrix[2][1] = softIronMatrix[7]; // 0
    deviceSettings.softIronMatrix[2][2] = softIronMatrix[8]; // 1

    JsonArray hardIronOffset = _doc["hardIronOffset"];
    deviceSettings.hardIronOffset[0] = hardIronOffset[0]; // 0
    deviceSettings.hardIronOffset[1] = hardIronOffset[1]; // 0
    deviceSettings.hardIronOffset[2] = hardIronOffset[2]; // 0

    JsonArray highGAccelerometerMisalignment = _doc["highGAccelerometerMisalignment"];
    deviceSettings.highGAccelerometerMisalignment[0][0] = highGAccelerometerMisalignment[0]; // 1
    deviceSettings.highGAccelerometerMisalignment[0][1] = highGAccelerometerMisalignment[1]; // 0
    deviceSettings.highGAccelerometerMisalignment[0][2] = highGAccelerometerMisalignment[2]; // 0
    deviceSettings.highGAccelerometerMisalignment[1][0] = highGAccelerometerMisalignment[3]; // 0
    deviceSettings.highGAccelerometerMisalignment[1][1] = highGAccelerometerMisalignment[4]; // 1
    deviceSettings.highGAccelerometerMisalignment[1][2] = highGAccelerometerMisalignment[5]; // 0
    deviceSettings.highGAccelerometerMisalignment[2][0] = highGAccelerometerMisalignment[6]; // 0
    deviceSettings.highGAccelerometerMisalignment[2][1] = highGAccelerometerMisalignment[7]; // 0
    deviceSettings.highGAccelerometerMisalignment[2][2] = highGAccelerometerMisalignment[8]; // 1

    JsonArray highGAccelerometerSensitivity = _doc["highGAccelerometerSensitivity"];
    deviceSettings.highGAccelerometerSensitivity[0] = highGAccelerometerSensitivity[0]; // 1
    deviceSettings.highGAccelerometerSensitivity[1] = highGAccelerometerSensitivity[1]; // 1
    deviceSettings.highGAccelerometerSensitivity[2] = highGAccelerometerSensitivity[2]; // 1

    JsonArray highGAccelerometerOffset = _doc["highGAccelerometerOffset"];
    deviceSettings.highGAccelerometerOffset[0] = highGAccelerometerOffset[0]; // 0
    deviceSettings.highGAccelerometerOffset[1] = highGAccelerometerOffset[1]; // 0
    deviceSettings.highGAccelerometerOffset[2] = highGAccelerometerOffset[2]; // 0

    strcpy(deviceSettings.deviceName,               _doc["deviceName"]);
    strcpy(deviceSettings.deviceName,               _doc["deviceName"]); // "x-IMU3"
    strcpy(deviceSettings.serialNumber,             _doc["serialNumber"]); // "Unknown"
    strcpy(deviceSettings.firmwareVersion,          _doc["firmwareVersion"]); // "Unknown"
    strcpy(deviceSettings.bootloaderVersion,        _doc["bootloaderVersion"]); // "Unknown"
    strcpy(deviceSettings.hardwareVersion,          _doc["hardwareVersion"]); // "Unknown"
    strcpy(deviceSettings.wirelessFirmwareVersion,  _doc["wirelessFirmwareVersion"]); // "Unknown"
    strcpy(deviceSettings.wiFiMacAddress,           _doc["wiFiMacAddress"]); // "0"
    strcpy(deviceSettings.wiFiIPAddress,            _doc["wiFiIPAddress"]); // "0"
    strcpy(deviceSettings.wiFiClientSsid,           _doc["wiFiClientSsid"]); // "x-IMU3 Network"
    strcpy(deviceSettings.wiFiClientKey,            _doc["wiFiClientKey"]); // "xiotechnologies"
    strcpy(deviceSettings.wiFiClientIPAddress,      _doc["wiFiClientIPAddress"]); // "192.168.1.2"
    strcpy(deviceSettings.wiFiClientNetmask,        _doc["wiFiClientNetmask"]); // "255.255.255.0"
    strcpy(deviceSettings.wiFiClientGateway,        _doc["wiFiClientGateway"]); // "192.168.1.1"
    strcpy(deviceSettings.wiFiAPSsid,               _doc["wiFiAPSsid"]); // nullptr
    strcpy(deviceSettings.wiFiAPKey,                _doc["wiFiAPKey"]); // nullptr
    strcpy(deviceSettings.udpIPAddress,             _doc["udpIPAddress"]); // "0"
    strcpy(deviceSettings.bluetoothName,            _doc["bluetoothName"]); // nullptr
    strcpy(deviceSettings.bluetoothPinCode,         _doc["bluetoothPinCode"]); // "1234"
    strcpy(deviceSettings.dataLoggerFileNamePrefix, _doc["dataLoggerFileNamePrefix"]); // nullptr

    deviceSettings.serialMode                               = _doc["serialMode"]; // 0
    deviceSettings.serialBaudRate                           = _doc["serialBaudRate"]; // 115200
    deviceSettings.serialRtsCtsEnabled                      = _doc["serialRtsCtsEnabled"]; // false
    deviceSettings.serialAccessoryNumberOfBytes             = _doc["serialAccessoryNumberOfBytes"]; // 1024
    deviceSettings.serialAccessoryTerminationByte           = _doc["serialAccessoryTerminationByte"]; // 10
    deviceSettings.serialAccessoryTimeout                   = _doc["serialAccessoryTimeout"]; // 100
    deviceSettings.wirelessMode                             = _doc["wirelessMode"]; // 2
    deviceSettings.externalAntennaeEnabled                  = _doc["externalAntennaeEnabled"]; // false
    deviceSettings.wiFiRegion                               = _doc["wiFiRegion"]; // 1
    deviceSettings.wiFiClientChannel                        = _doc["wiFiClientChannel"]; // 0
    deviceSettings.wiFiClientDhcpEnabled                    = _doc["wiFiClientDhcpEnabled"]; // true
    deviceSettings.wiFiAPChannel                            = _doc["wiFiAPChannel"]; // 36
    deviceSettings.tcpPort                                  = _doc["tcpPort"]; // 7000
    deviceSettings.udpSendPort                              = _doc["udpSendPort"]; // 0
    deviceSettings.udpReceivePort                           = _doc["udpReceivePort"]; // 9000
    deviceSettings.synchronisationEnabled                   = _doc["synchronisationEnabled"]; // true
    deviceSettings.synchronisationNetworkLatency            = _doc["synchronisationNetworkLatency"]; // 1500
    deviceSettings.bluetoothAddress                         = _doc["bluetoothAddress"]; // 0
    deviceSettings.bluetoothDiscoveryMode                   = _doc["bluetoothDiscoveryMode"]; // 2
    deviceSettings.bluetoothPairedAddress                   = _doc["bluetoothPairedAddress"]; // 0
    deviceSettings.bluetoothPairedLinkKey                   = _doc["bluetoothPairedLinkKey"]; // 0
    deviceSettings.dataLoggerEnabled                        = _doc["dataLoggerEnabled"]; // false
    deviceSettings.dataLoggerFileNameTimeEnabled            = _doc["dataLoggerFileNameTimeEnabled"]; // true
    deviceSettings.dataLoggerFileNameCounterEnabled         = _doc["dataLoggerFileNameCounterEnabled"]; // false
    deviceSettings.dataLoggerMaxFileSize                    = _doc["dataLoggerMaxFileSize"]; // 0
    deviceSettings.dataLoggerMaxFilePeriod                  = _doc["dataLoggerMaxFilePeriod"]; // 0
    deviceSettings.axesAlignment                            = _doc["axesAlignment"]; // 0
    deviceSettings.gyroscopeOffsetCorrectionEnabled         = _doc["gyroscopeOffsetCorrectionEnabled"]; // true
    deviceSettings.ahrsAxesConvention                       = _doc["ahrsAxesConvention"]; // 0
    deviceSettings.ahrsGain                                 = _doc["ahrsGain"]; // 0.5
    deviceSettings.ahrsIgnoreMagnetometer                   = _doc["ahrsIgnoreMagnetometer"]; // false
    deviceSettings.ahrsAccelerationRejectionEnabled         = _doc["ahrsAccelerationRejectionEnabled"]; // true
    deviceSettings.ahrsMagneticRejectionEnabled             = _doc["ahrsMagneticRejectionEnabled"]; // true
    deviceSettings.binaryModeEnabled                        = _doc["binaryModeEnabled"]; // true
    deviceSettings.usbDataMessagesEnabled                   = _doc["usbDataMessagesEnabled"]; // true
    deviceSettings.serialDataMessagesEnabled                = _doc["serialDataMessagesEnabled"]; // true
    deviceSettings.tcpDataMessagesEnabled                   = _doc["tcpDataMessagesEnabled"]; // true
    deviceSettings.udpDataMessagesEnabled                   = _doc["udpDataMessagesEnabled"]; // true
    deviceSettings.bluetoothDataMessagesEnabled             = _doc["bluetoothDataMessagesEnabled"]; // true
    deviceSettings.dataLoggerDataMessagesEnabled            = _doc["dataLoggerDataMessagesEnabled"]; // true
    deviceSettings.ahrsMessageType                          = _doc["ahrsMessageType"]; // 0
    deviceSettings.inertialMessageRateDivisor               = _doc["inertialMessageRateDivisor"]; // 8
    deviceSettings.magnetometerMessageRateDivisor           = _doc["magnetometerMessageRateDivisor"]; // 1
    deviceSettings.ahrsMessageRateDivisor                   = _doc["ahrsMessageRateDivisor"]; // 8
    deviceSettings.highGAccelerometerMessageRateDivisor     = _doc["highGAccelerometerMessageRateDivisor"]; // 32
    deviceSettings.temperatureMessageRateDivisor            = _doc["temperatureMessageRateDivisor"]; // 5
    deviceSettings.batteryMessageRateDivisor                = _doc["batteryMessageRateDivisor"]; // 5
    deviceSettings.rssiMessageRateDivisor                   = _doc["rssiMessageRateDivisor"]; // 1

    // Clear the JSON buffer
    _doc.clear();

    return true;
}

bool saveConfigurations() {
    // _file = SPIFFS.open(CONFIG_FILE_NAME, "w");

    // if (!_file) {
    //     return false;
    // }

    // // Create a JSON document
    // StaticJsonDocument<CONFIG_FILE_BUFFER_SIZE> jsonDoc; // Adjust the buffer size as needed

    // // Set the configuration settings in the JSON document
    // jsonDoc["calibrationDate"] = deviceSettings.calibrationDate;
    // jsonDoc["deviceName"] = deviceSettings.deviceName;
    // jsonDoc["serialNumber"] = deviceSettings.serialNumber;

    // // Serialize the JSON document to the file
    // if (!serializeJson(jsonDoc, _file)) {
    //     return false;
    // }

    // // Close the file and empty buffer
    // _file.close();
    // jsonDoc.clear();

    // return true;
}