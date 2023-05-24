/******************************************************************
    @file       xioAPI.h
    @brief      Arduino Library for the xio API 
    @author     Braidan Duffy
    @copyright  MIT license

    Code:       Braidan Duffy
    Version:    1.0.0
    Date:       17/05/2023
    Modifed:    17/05/2023

    CHANGELOG:
    v1.0.0 - Initial release

    Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
            (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
            and using information in the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
            Uses the Madgwick Quaternion Class.

******************************************************************/

#ifndef xioAPI_h
#define xioAPI_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "xioAPI_Types.h"
#include "xioAPI_Settings.h"
#include "xioAPI_Protocol.h"

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;

class xioAPI {
public:
    bool begin(Stream* port);
    bool checkForCommand();
    void handleCommand(const char* cmdPtr);
    const char* getCommand();
    const char* getValue();
    ValueType getValueType();
    unsigned long hash(const char *str);

    // ------------------------------
    // --- USER-DEFINED FUNCTIONS ---
    // ------------------------------

    virtual void cmdStrobe() { return; }
    virtual void cmdColour(const char* colorString) { return; }
    virtual void cmdHeading(float heading) { return; }

    // ------------------------
    // --- COMMAND MESSAGES ---
    // ------------------------

    void sendNetworkAnnouncement(NetworkAnnouncement na) {
        /**
         * Network announcement format:
         * {
         * "sync": [sync],
         * "name": "[displayName]",
         * "sn": "[serialNumber]",
         * "ip": "[ipAddress]",
         * "port": [tcpPort],
         * "send": [udpSendPort],
         * "receive": [udpReceivePort],
         * "rssi": [wiFiRSSI]
         * "battery": [batteryPercentage],
         * "status": [chargingStatus]
         * }\r\n
        */
        StaticJsonDocument<256> _doc;
        char _out[256];

        _doc["sync"] = na.sync;
        _doc["name"] = na.displayName;
        _doc["sn"] = na.serialNumber;
        _doc["ip"] = na.ipAddress;
        _doc["port"] = na.tcpPort;
        _doc["send"] = na.udpSendPort;
        _doc["receive"] = na.udpReceivePort;
        _doc["rssi"] = na.rssiPercentage;
        _doc["battery"] = na.batteryPercentage;
        _doc["status"] = na.chargingStatus;

        serializeJson(_doc, _out);
        send("%s\r\n", _out);
    }

    void sendPing(Ping ping) {
        /**
         * Ping format:
         * {
         * "ping": {
         *      "interface": "[interface]",
         *      "deviceName": "[deviceName]",
         *      "serialNumber": "[serialNumber]"
         * }
         * }\r\n
        */
        StaticJsonDocument<96> _doc;
        char _out[96];

        JsonObject root = _doc.createNestedObject("ping");
        root["interface"] = "USB";
        root["deviceName"] = "Thetis";
        root["serialNumber"] = "Unknown";

        serializeJson(_doc, _out);
        send("%s\r\n", _out);
    }

    template<typename T>
    void sendSetting(const char* key, T& value) {
        StaticJsonDocument<128> _doc;
        char _out[128];

        const std::type_info& typeInfo = typeid(variable);

        if (typeInfo == typeid(int)) {
            int value = static_cast<int>(variable);
            // Do something with 'value' as an integer
        }
        else if (typeInfo == typeid(float)) {
            float value = static_cast<float>(variable);
            // Do something with 'value' as a float
        }
        else if (typeInfo == typeid(bool)) {
            bool value = static_cast<bool>(variable);
            // Do something with 'value' as a boolean
        }
        else if (typeInfo == typeid(const char*)) {
            const char* value = static_cast<const char*>(variable);
            // Do something with 'value' as a const char*
        }
        // Add more conditions for other types as needed
        else {
            // Handle unsupported types or fallback behavior
        }

        _doc[key] = value;
        serializeJson(_doc, _out);
        send("%s\r\n", _out);
    }

    void sendSetting(const char* key, const float* value, size_t size) {
        StaticJsonDocument<256> _doc;
        JsonArray _settingArray = _doc.createNestedArray(_cmd);

        for (size_t i=0; i<size; i++) {
            _settingArray.add(value[i]);
        }

        char _out[96];
        serializeJson(_doc, _out);
        send("%s\r\n", _out);
    }

    // void sendSetting(const char* key, float* value[3], size_t nRows, size_t nCols) {
    //     StaticJsonDocument<192> _doc;
    //     Serial.printf("Sending: %s\r\n", key);
    //     JsonArray _settingArray = _doc.createNestedArray(key);
    //     Serial.printf("Sending: %s\r\n", key);

    //     for (size_t i=0; i<nRows; i++) {
    //         for (size_t j=0; j<nCols; j++) {
    //             _settingArray.add(value[i][j]);
    //         }
    //     }

    //     char _out[192];
    //     serializeJson(_doc, _out);
    //     send("%s\r\n", _out);
    // }

    // ---------------------
    // --- DATA MESSAGES ---
    // ---------------------

    void sendInertial(InertialMessage msg) {
        // Inertial Message Format: "I,timestamp (µs),gx,gy,gz,ax,ay,az\r\n"
        send("I,%lu,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f\r\n", msg.timestamp, msg.gx, msg.gy, msg.gz, msg.ax, msg.ay, msg.az);
    }

    void sendMag(MagnetoMessage msg) {
        // Magnetometer Message Format: "M,timestamp (µs),mx,my,mz\r\n"
        send("M,%lu,%0.4f,%0.4f,%0.4f\r\n", msg.timestamp, msg.mx, msg.my, msg.mz);
    }

    void sendTemperature(TemperatureMessage msg) {
        // Temperature Message Format: "T,timestamp (µs),temperature (°C)\r\n"
        send("T,%lu,%0.4f\r\n", msg.timestamp, msg.temp);
    }

    void sendQuaternion(QuaternionMessage msg) {
        // Quaternion Message Format: "Q,timestamp (µs),w,x,y,z\r\n"
        send("Q,%lu,%0.4f,%0.4f,%0.4f,%0.4f\r\n", msg.timestamp, msg.w, msg.x, msg.y, msg.z);
    }

    void sendEulerAngles(EulerMessage msg) {
        // Euler Angles Message Format: "A,timestamp (µs),roll,pitch,yaw\r\n"
        send("A,%lu,%0.4f,%0.4f,%0.4f\r\n", msg.timestamp, msg.roll, msg.pitch, msg.yaw);
    }

    void sendBattery(BatteryMessage msg) {
        // Battery Message Format: "B,timestamp (µs),percentCharged,voltage,status\r\n"
        send("B,%lu,%0.4f,%0.4f,%u\r\n", msg.timestamp, msg.percentCharged, msg.voltage, msg.status);
    }

    void sendRSSI(RSSIMessage msg) {
        // RSSI Message Format: "W,timestamp (µs),percent,power (dBm)\r\n"
        send("W,%lu,%0.4f,%0.4f\r\n", msg.timestamp, msg.percentage, msg.power);
    }

    void sendNotification(const char *note) {
        // Notification Message Format: "N,timestamp (µs),note\r\n"
        send("N,%lu,%s\r\n", micros(), note);
    }

    void sendError(const char *error) {
        // Error Message Format: "F,timestamp (µs),errorMessage\r\n"
        send("F,%lu,%s\r\n", micros(), error);
    }

protected:
    Stream* _serialPort = nullptr;
    bool _isActive = false;
    ValueType _valueType;
    char _cmd[64];
    JsonVariant _value;

    ValueType parseValueType(char c);
    void print(const char *line);
    void send(const char* message, ...);

private:
    const char* _stringValue;
    int _intValue;
    float _floatValue;
    // float _floatValueArray[];
};

extern xioAPI xioapi;

#endif // xioAPI_h