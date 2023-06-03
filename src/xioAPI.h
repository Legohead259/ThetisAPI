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
    void checkForCommand();
    void handleCommand(const char* cmdPtr);
    unsigned long hash(const char *str);

    const char* getCommand() { return _cmd; }
    
    template <typename T>
    T getValue() { return _value.as<T>(); }

    ValueType getValueType() { return _valueType; }

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

    void sendSetting(const char* key, const settingTableEntry* entry) {
        // Setting format: {"[setting]":[value]}\r\n

        StaticJsonDocument<256> _doc;
        char _out[256];

        SettingType _entryType = entry->type;
        void* _entryValue = entry->value;
        JsonArray _settingArray;
        const float* _array;

        switch (_entryType) {
            case BOOL:
                _doc[key] = *(bool*) _entryValue;
                break;
            case CHAR:
                _doc[key] = *(uint8_t*) _entryValue;
                break;
            case FLOAT:
                _doc[key] = *(float*) _entryValue;
                break;
            case INT:
                _doc[key] = *(int*) _entryValue;
                break;
            case FLOAT_ARRAY:
                _settingArray = _doc.createNestedArray(_cmd);
                _array = static_cast<const float*>(_entryValue);
                for (size_t i=0; i<entry->len; i++) {
                    _settingArray.add(_array[i]);
                }
                break;
            case CHAR_ARRAY:
                _doc[key] = (const char*) _entryValue;
                break;
            default:
                return;
        }

        serializeJson(_doc, _out);
        send("%s\r\n", _out);
    }

    // --------------------------------
    // --- COMMAND ACKNOWLEDGEMENTS ---
    // --------------------------------

    void ackApply() { send("{\"apply\":null}\r\n"); }
    void ackSave() { send("{\"save\":null}\r\n"); }

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
    void clearCmd();
    void clearValue();
};

extern xioAPI xioapi;

#endif // xioAPI_h