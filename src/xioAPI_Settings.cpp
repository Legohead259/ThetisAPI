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
settingTableEntry settingTable[SETTING_TABLE_SIZE] = {
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

    JsonObject root = _jsonConfigDoc.as<JsonObject>();

    for (JsonPair kv : root) {
        unsigned long _jsonKeyHash = hash(kv.key().c_str());
        for (size_t i=0; i<SETTING_TABLE_SIZE; i++) { // Check all keys in the hash table
            if (_jsonKeyHash == settingTable[i].hash) {
                updateSetting(&settingTable[i], kv.value());
                break;
            }
        }
    }

    // Clear the JSON buffer
    // _jsonConfigDoc.clear();
    return true;
}

bool saveConfigurations() {
    _file = SPIFFS.open(CONFIG_FILE_NAME, "w");

    if (!_file) {
        return false;
    }

    JsonObject root = _jsonConfigDoc.as<JsonObject>();

    for (size_t i=0; i<SETTING_TABLE_SIZE; i++) {
        settingTableEntry entry = settingTable[i];
        const char* key = entry.key;
        
        if (key == nullptr) continue; // Skip past the rest of the loop if the key (entry) is empty

        // Determine the value type and add it to the JSON object
        float* array;
        JsonArray jsonArray;
        
        switch (entry.type) {
            case INT:
                root[key] = *(int*)entry.value;
                break;
            case FLOAT:
                root[key] = *(float*)entry.value;
                break;
            case BOOL:
                root[key] = *(bool*)entry.value;
                break;
            case CHAR:
                root[key] = *(uint8_t*)entry.value;
                break;
            case FLOAT_ARRAY:
                array = (float*)entry.value;
                jsonArray = root.createNestedArray(key);
                
                for (size_t j=0; j<entry.len; j++) {
                    jsonArray.add(array[j]);
                }
                break;
            case CHAR_ARRAY:
                root[key] = (const char*)entry.value;
                break;
            default:
                // TODO: Error handling
                break;
        }
    }


    // Serialize the JSON document to the file
    if (!serializeJson(_jsonConfigDoc, _file)) {
        Serial.println("Failed to serialize JSON");
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



