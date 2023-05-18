/******************************************************************
    @file       ThetisAPI.h
    @brief      Arduino Library for Thetis using the xio API 
    @author     David Such
    @copyright  Please see the accompanying LICENSE file

    Code:       David Such
    Modified:   Braidan Duffy
    Version:    1.1.0
    Date:       13/04/23
    Modifed:    17/05/2023

    1.0.0       Original Release.       13/04/23
    1.1.0       Restylized for Thetis implementation 17/05/2023

    Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
            (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
            and using information in the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
            Uses the Madgwick Quaternion Class.

******************************************************************/

#ifndef ThetisAPI_h
#define ThetisAPI_h

#include <Arduino.h>
#include <stdarg.h>

#include "xio_API_Types.h"
#include "xio_API_Protocol.h"


// =============================
// === THETIS API DEFINITION ===
// =============================


class ThetisAPI {
public:
    bool begin(Print* port);


    // =====================
    // === DATA MESSAGES ===
    // =====================


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

    void sendNotification(char *note) {
        // Notification Message Format: "N,timestamp (µs),note\r\n"
        send("N,%lu,%s\r\n", micros(), note);
    }

    void sendError(char *error) {
        // Error Message Format: "F,timestamp (µs),errorMessage\r\n"
        send("F,%lu,%s\r\n", micros(), error);
    }


    // ========================
    // === COMMAND MESSAGES ===
    // ========================
    

    void sendNetworkAnnouncement(NetworkAnnouncement na) {
        /**
         * Network announcement format:
         * {
         * "sync": "[sync]",
         * "name": "[displayName]",
         * "sn": "[serialNumber]",
         * "ip": "[ipAddress]",
         * "port": "[portTCP]",
         * "send": "[sendUDP]",
         * "receive": "[receiveUDP]",
         * "battery": "[batteryPercentage]",
         * "status": "[chargingStatus]"
         * }\r\n
        */
       send("{\"sync\":\"%u\",\"name\":\"%s\",\"sn\":\"%s\",\"ip\":\"%s\",\"port\":\"%u\",\"send\":\"%u\",\"receive\":\"%u\",\"battery\":\"%u\",\"status\":\"%u\"}\r\n", na.sync, na.displayName, na.serialNumber, na.ipAddress, na.portTCP, na.sendUDP, na.receiveUDP, na.batteryPercentage, na.chargingStatus);
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
       send("{\"ping\":{\"interface\":\"%s\",\"deviceName\":\"%s\",\"serialNumber\":\"%s\"}}\r\n", ping.interface, ping.deviceName, ping.serialNumber);
    }

    void sendResponse(char *key, char *value) {
        /**
         * Response format:
         * {
         * "[key]": "[value]"
         * } \r\n
        */
        send("{\"%s\":\"%s\"}\r\n", key, value);
    }

    void sendResponse(char *key, int value) {
        /**
         * Response format:
         * {
         * "[key]": "[value]"
         * } \r\n
        */
        send("{\"%s\":\"%d\"}\r\n", key, value);
    }

    const unsigned long hash(const char *str);
    void checkForCommand();
    bool newCommand();
    char* getCommand();
    char* getValue();
    ValueType getValueType();
    
private:
    Print* _serialPort;
    void print(const char *line);
    void send(const char* message, ...);
    void genJSONKeyValuePair(char* buf, char* key, int value) {
        sprintf(buf, "\"%s\":\"%d\"", key, value);
    }
    void genJSONKeyValuePair(char* buf, char* key, char* value) {
        sprintf(buf, "\"%s\":\"%s\"", key, value);
    }
    ValueType parseValueType(char c);
    
    char _cmd[CMD_SIZE], _value[NOTE_SIZE];
    ValueType _valueType;

#endif // ThetisAPI_h