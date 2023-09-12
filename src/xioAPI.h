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
#include <WiFiUdp.h>
#include "xioAPI_CircularBuffer.h"
#include "xioAPI_Types.h"
#include "xioAPI_Settings.h"
#include "xioAPI_Protocol.h"
#include "xioAPI_Utility.h"

#define XIOAPI_NETWORK_DISCOVERY_PORT 10000

using namespace xioAPI_Types;
using namespace xioAPI_Protocol;

typedef std::function<void()> CallbackFunction;

class xioAPI {
public:
    bool begin(Stream* port);
    bool begin(Stream* port, WiFiUDP* udp);
    void checkForCommand();
    void handleCommand(const char* cmdPtr);

    const char* getCommand() { return _cmd; }
    
    template <typename T>
    T getValue() { return _value.as<T>(); }

    ValueType getValueType() { return _valueType; }

    // ------------------------
    // --- COMMAND MESSAGES ---
    // ------------------------

    void send(const char* message, ...);
    void sendDataMessage(const char* message, ...);
    void sendSerial(const char* message, size_t size);
    void sendUDP(uint8_t* buffer, size_t size, char* ipAddress=settings.udpIPAddress, int sendPort=settings.udpSendPort);
    void sendSetting(const settingTableEntry* entry);
    void sendAck(const char* cmd) { send("{\"%s\":null}", cmd); }
    void sendPing(Ping ping);
    void sendSelfTestResults(SelfTestResults results);
    void sendNetworkAnnouncement(NetworkAnnouncement na);
    void sendSettingTable();
    void sendSettingFile();

    // Update the internal system time with passed _value. NOTE: `cmdWriteTimeCallbackPtr` must be user-defined before called.
    void cmdWriteTime() { executeUserDefinedCommand(cmdWriteTimeCallbackPtr); }
    
    // Send the internal system time. NOTE: `cmdReadTimeCallbackPtr` must be user-defined before called.
    void cmdReadTime() { executeUserDefinedCommand(cmdReadTimeCallbackPtr); }
    
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

    void sendInertialMessage(InertialMessage msg);
    void sendMagnetometerMessage(MagnetometerMessage msg);
    void sendTemperatureMessage(TemperatureMessage msg);
    void sendQuaternionMessage(QuaternionMessage msg);
    void sendEulerMessage(EulerMessage msg);
    void sendBatteryMessage(BatteryMessage msg);
    void sendRSSIMessage(RSSIMessage msg);
    void sendNotification(const char *note);
    void sendError(const char *error);

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
    WiFiUDP* _udpServer = nullptr;
    bool _isActive = false;
    bool _usbActive = true;
    bool _udpActive = true;
    ValueType _valueType;
    char _cmd[64];
    JsonVariant _value;

    ValueType parseValueType(char c);
    void write();

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
extern CircularBuffer<char,8192> dataASCIIBuffer;

#endif // xioAPI_h