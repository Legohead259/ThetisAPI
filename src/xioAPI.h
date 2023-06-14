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
#include "xioAPI_Utility.h"
#include "TimeLib.h" 

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;

typedef void (*CallbackFunction)();

class xioAPI {
public:
    bool begin(Stream* port);
    void checkForCommand();
    void handleCommand(const char* cmdPtr);

    const char* getCommand() { return _cmd; }
    
    template <typename T>
    T getValue() { return _value.as<T>(); }

    ValueType getValueType() { return _valueType; }

    // ------------------------
    // --- COMMAND MESSAGES ---
    // ------------------------

    void sendSetting(const settingTableEntry* entry);
    void sendAck(const char* cmd) { send(true, "{\"%s\":null}", cmd); }
    void sendPing(Ping ping);
    void sendSelfTestResults(SelfTestResults results);
    void sendNetworkAnnouncement(NetworkAnnouncement na);
    void sendSettingTable();
    void sendSettingFile();

    // Update the internal system time with passed _value. NOTE: `cmdWriteTimeCallbackPtr` must be user-defined before called.
    void cmdWriteTime() { executeUserDefinedCommand(cmdWriteTimeCallbackPtr); }
    
    // Send the internal system time. NOTE: `cmdReadTimeCallbackPtr` must be user-defined before called.
    void sendTime() { executeUserDefinedCommand(cmdReadTimeCallbackPtr); }
    
    // Soft resets the device. NOTE: `cmdResetCallbackPtr` must be user-defined before called.
    void cmdReset() { executeUserDefinedCommand(cmdResetCallbackPtr); }

    // Shuts down the device. NOTE: `cmdShutdownCallbackPtr` must be user-defined before called.
    void cmdShutdown() { executeUserDefinedCommand(cmdShutdownCallbackPtr); }

    // Flashes the onboard LED white for 5-seconds. NOTE: `cmdStrobeCallbackPtr` must be user-defined before called.
    void cmdStrobe() { executeUserDefinedCommand(cmdStrobeCallbackPtr); }

    // Sets the onboard LED to the specified color. NOTE: `cmdColourCallbackPtr` must be user-defined before called.
    void cmdColour() { executeUserDefinedCommand(cmdColourCallbackPtr); }

    // Resets the AHRS heading to the specified value. NOTE: `cmdHeadingCallbackPtr` must be user-defined before called.
    void cmdHeading() { executeUserDefinedCommand(cmdHeadingCallbackPtr); }

    // Sends a specified message to the Serial accessory device. NOTE: `cmdSerialAccessoryCallbackPtr` must be user-defined before called.
    void cmdSerialAccessory() { executeUserDefinedCommand(cmdSerialAccessoryCallbackPtr); }

    // Formats the device storage to FAT32 - ERASES ALL DATA. NOTE: `cmdFormatCallbackPtr` must be user-defined before called.
    void cmdFormat() { executeUserDefinedCommand(cmdFormatCallbackPtr); }

    // Puts the device into a self-test diagnostic mode and sends the results. NOTE: `cmdSelfTestCallbackPtr` must be user-defined before called.
    void cmdSelfTest() { executeUserDefinedCommand(cmdSelfTestCallbackPtr); }

    // Puts the device into the bootloader mode. NOTE: `cmdBootloaderCallbackPtr` must be user-defined before called.
    void cmdBootloader() { executeUserDefinedCommand(cmdBootloaderCallbackPtr); }

    // Puts the device into a factory mode where READ-ONLY settings can be written. NOTE: `cmdFactoryCallbackPtr` must be user-defined before called.
    void cmdFactory() { executeUserDefinedCommand(cmdFactoryCallbackPtr); }

    // Erases the device's EEPROM storage. NOTE: `cmdEraseCallbackPtr` must be user-defined before called.
    void cmdErase() { executeUserDefinedCommand(cmdEraseCallbackPtr); }

    // ---------------------
    // --- DATA MESSAGES ---
    // ---------------------

    void sendInertial(InertialMessage msg) {
        // Inertial Message Format: "I,timestamp (µs),gx,gy,gz,ax,ay,az\r\n"
        send(true, "I,%lu,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f", msg.timestamp, msg.gx, msg.gy, msg.gz, msg.ax, msg.ay, msg.az);
    }

    void sendMag(MagnetoMessage msg) {
        // Magnetometer Message Format: "M,timestamp (µs),mx,my,mz\r\n"
        send(true, "M,%lu,%0.4f,%0.4f,%0.4f", msg.timestamp, msg.mx, msg.my, msg.mz);
    }

    void sendTemperature(TemperatureMessage msg) {
        // Temperature Message Format: "T,timestamp (µs),temperature (°C)\r\n"
        send(true, "T,%lu,%0.4f", msg.timestamp, msg.temp);
    }

    void sendQuaternion(QuaternionMessage msg) {
        // Quaternion Message Format: "Q,timestamp (µs),w,x,y,z\r\n"
        send(true, "Q,%lu,%0.4f,%0.4f,%0.4f,%0.4f", msg.timestamp, msg.w, msg.x, msg.y, msg.z);
    }

    void sendEulerAngles(EulerMessage msg) {
        // Euler Angles Message Format: "A,timestamp (µs),roll,pitch,yaw\r\n"
        send(true, "A,%lu,%0.4f,%0.4f,%0.4f", msg.timestamp, msg.roll, msg.pitch, msg.yaw);
    }

    void sendBattery(BatteryMessage msg) {
        // Battery Message Format: "B,timestamp (µs),percentCharged,voltage,status\r\n"
        send(true, "B,%lu,%0.4f,%0.4f,%u", msg.timestamp, msg.percentCharged, msg.voltage, msg.status);
    }

    void sendRSSI(RSSIMessage msg) {
        // RSSI Message Format: "W,timestamp (µs),percent,power (dBm)\r\n"
        send(true, "W,%lu,%0.4f,%0.4f", msg.timestamp, msg.percentage, msg.power);
    }

    void sendNotification(const char *note) {
        // Notification Message Format: "N,timestamp (µs),note\r\n"
        send(true, "N,%lu,%s", micros(), note);
    }

    void sendError(const char *error) {
        // Error Message Format: "F,timestamp (µs),errorMessage\r\n"
        send(true, "F,%lu,%s", micros(), error);
    }

    // --------------------------------
    // --- COMMAND CALLBACK SETTERS ---
    // --------------------------------

    void setCmdReadTimeCallback(CallbackFunction cbPtr) { cmdReadTimeCallbackPtr = cbPtr; }
    void setCmdWriteTimeCallback(CallbackFunction cbPtr) { cmdWriteTimeCallbackPtr = cbPtr; }
    void setCmdResetCallback(CallbackFunction cbPtr) { cmdResetCallbackPtr = cbPtr; }
    void setCmdShutdownCallback(CallbackFunction cbPtr) { cmdShutdownCallbackPtr = cbPtr; }
    void setCmdStrobeCallback(CallbackFunction cbPtr) { cmdStrobeCallbackPtr = cbPtr; }
    void setCmdColourCallback(CallbackFunction cbPtr) { cmdColourCallbackPtr = cbPtr; }
    void setCmdHeadingCallback(CallbackFunction cbPtr) { cmdHeadingCallbackPtr = cbPtr; }
    void setCmdSerialAccessoryCallback(CallbackFunction cbPtr) { cmdSerialAccessoryCallbackPtr = cbPtr; }
    void setCmdFormatCallback(CallbackFunction cbPtr) { cmdFormatCallbackPtr = cbPtr; }
    void setCmdSelfTestCallback(CallbackFunction cbPtr) { cmdSelfTestCallbackPtr = cbPtr; }
    void setCmdBootloaderCallback(CallbackFunction cbPtr) { cmdBootloaderCallbackPtr = cbPtr; }
    void setCmdFactoryCallback(CallbackFunction cbPtr) { cmdFactoryCallbackPtr = cbPtr; }
    void setCmdEraseCallback(CallbackFunction cbPtr) { cmdEraseCallbackPtr = cbPtr; }

protected:
    Stream* _serialPort = nullptr;
    bool _isActive = false;
    ValueType _valueType;
    char _cmd[64];
    JsonVariant _value;

    ValueType parseValueType(char c);
    void print(const char *line);
    void send(bool writeLineFeed, const char* message, ...);

private:
    void clearCmd();
    void clearValue();
    CallbackFunction cmdReadTimeCallbackPtr = nullptr;
    CallbackFunction cmdWriteTimeCallbackPtr = nullptr;
    CallbackFunction cmdResetCallbackPtr = nullptr;
    CallbackFunction cmdShutdownCallbackPtr = nullptr;
    CallbackFunction cmdStrobeCallbackPtr = nullptr;
    CallbackFunction cmdColourCallbackPtr = nullptr;
    CallbackFunction cmdHeadingCallbackPtr = nullptr;
    CallbackFunction cmdSerialAccessoryCallbackPtr = nullptr;
    CallbackFunction cmdFormatCallbackPtr = nullptr;
    CallbackFunction cmdSelfTestCallbackPtr = nullptr;
    CallbackFunction cmdBootloaderCallbackPtr = nullptr;
    CallbackFunction cmdFactoryCallbackPtr = nullptr;
    CallbackFunction cmdEraseCallbackPtr = nullptr;

    void executeUserDefinedCommand(CallbackFunction cbPtr) { if (cbPtr != nullptr) cbPtr(); else return; }
};

extern xioAPI api;

#endif // xioAPI_h