![open source](https://badgen.net/badge/open/source/blue?icon=github)

### Disclaimer
This library is **NOT OFFICIALLY SUPPORTED OR ENDORSED BY x-io TECHNOLOGIES**. 
This is made in the hopes of making integration with the x-IMU3 GUI easier for hobbyist projects. 
Both x-io Technologies and the author of the library do not offer any warranty on this library and therefore do not accept liability for its use. 
If you need support on a specific project, please create an issue or a pull request and it will be addressed.

# xioAPI-Arduino

An Arduino Library to facilitate serial communication with the xIMU3 GUI data visualisation software. 
Originally forked from the [ReefWing xIMU3 library]([url](https://github.com/Reefwing-Software/Reefwing-xIMU3)), this library has since expanded to be a general implementation of the xioAPI as specified in the [x-IMU3 user manual]([url](https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf)).

## x-io Technologies & the x-IMU3 GUI

[x-io Technologies](https://x-io.co.uk/) Limited is a UK-based engineering company that specialises in the design and production of IMUs. 
It was founded in 2012 by Seb Madgwick during his PhD at the University of Bristol. 
The [Madgwick algorithm](https://github.com/xioTechnologies/Fusion) in its various incarnations, is used in a LOT of modern AHRS libraries.

In addition to their sensor fusion software, x-io also have an IMU data visualisation tool called the x-IMU3 GUI. 
This free open source software is available for all major operating systems (Windows, macOS, and Linux). 
You can download a copy from the [x-io web site](https://x-io.co.uk/x-imu3/). 

## The x-IMU3 API

The API largely consists of command and data messages. 
An excerpt from the x-IMUs user manual follows:

```
The communication protocol consists of two message types:

- Command messages 
- Data messages 

All messages are terminated by a Line Feed (LF) control character. 
This termination byte will not appear anywhere else in a message and 
so can be used to divide a byte stream into individual messages. 

Some messages are terminated with an additional Carriage Return (CR) 
control character.

The first byte of a message indicates the message type. Command messages start with the character "{" (0x7B in hex, 123 in decimal). 
Data messages start with either an uppercase character or a byte value greater than 0x80 (128 in decimal) depending on the message.
You could use the command messages to control your Arduino from the x-IMU3 GUI, and we will demonstrate how to read and parse the commands from the serial port, but first let's look at data messages.
```

This library makes it possible to integrate Arduino projects directly with the xioAPI and make configuration and visualization easier for the hobbyist programmer.

## Data Messages

One of the key things that you will note when looking at the data message format (Figure 1), is that they all include a time stamp (in microseconds). 
This means that you need an IMU library which returns a time stamp with each sensor reading. 
Some IMUs have a dedicated time stamp register, but others do not. 
Therefore, we must use the Arduino `micros()` function to record the time at which the reading is taken.

| MSG ID | TIME STAMP (µs) |         PAYLOAD         | STOP BYTE |
|:------:|:---------------:|:-----------------------:|:---------:|

*Figure 1. x-IMU3 Data Message Format*

Data messages can be either ASCII encoded or binary, depending on the device settings. 
For now, the only supported message format is ASCII. Each value in an ASCII data message is comma separated.

The `MSG_ID` is the first byte in a data message and needs be a single uppercase character indicating the message type (e.g., `I`). 
The next value is the time stamp in microseconds. 
The API specifies a 64-bit unsigned integer for the time stamp, but 32-bits works, and this is what `micros()` returns.

The data payload data type depends on the message. 
For example, the magnetometer message (`MSG_ID = M`) expects three floating point numbers printed to four decimal places. 
The values expected from the IMU are the scaled output (i.e., celcius for temperature, DPS for gyroscope, g for acceleration, and gauss for the magnetometer). 
All messages are terminated by a Line Feed (`\n`) control character. 
This termination byte will not appear anywhere else in a message and so it can be used to divide a byte stream into individual messages.
Most messages are also terminated with an additional Carriage Return (`\r`) control character.

This library includes wrapper messages for all of the data message types. 
For example, to send a magnetometer message you can use the method, `sendMag(MagnetoMessage msg)`. 
The `MagnetoMessage` struct contains all of the information required to properly format this message. 
It is up to you to build the appropriate pipeline from your sensor readings to the message struct.

``` c++
struct MagnetoMessage {
    float mx, my, mz;
    uint32_t  timestamp;
};
```

All of the possible data messages are currently supported.

## Network Announcement Message

If your device is WiFi-capable, then you can connect it wirelessly to the x-IMU3 GUI over a local network connection.
Your device will need to broadcast a Network Announcement Message every second from UDP port 10000.
The Network Announcement Message is a JSON packet specified below and can be called using the library using the `sendNetworkAnnouncement(NetworkAnnouncement na)` function.
You will need to populate the fields of the NetworkAnnouncement structure based upon your device.

``` JSON
{
  "sync": 0,
  "name": "Raven",
  "sn": "0123-4567-89AB-CDEF",
  "ip": "192.168.1.1",
  "port": 7000,
  "send": 8000,
  "receive": 9000,
  "rssi": 100,
  "battery": 100,
  "status": 0
}
```

## Command Messages

Command messages are a JSON object containing a single key/value pair, terminated by the usual `\r\n`.
A sample command message, requesting the device serial number looks like:

``` JSON
{"serialNumber":null}\r\n
```

The library comes with a function, `checkForCommand()` that polls the API's stream interface for any data.
If there is data present, then the function will read the data until the null terminator ('\n').
The read data will then be attempted to format into a JSON buffer using the [ArduinoJson libary]([url](https://github.com/bblanchon/ArduinoJson)).
If the data is not a valid JSON key/value pair, then an error will be returned and the function will continue checking for commands.
But, if there is a JSON string present, it will parse it into separate key and value variables for later use.
The function will also immediately handle the command using the `handleCommand()` function.

## Parsing Command Messages

Since there are many possible command messages that can be sent to the device, it is necessary to handle them in special ways to save memory at the cost of increased processing requirements.
The command value will always be a string and therefore difficult to process in Arduino.
An easy solution is to assign this string a specific numerical ID that we can quickly compare using a `switch-case` statement.
Therefore, this library uses the [DJB2 hash function]([url](https://theartincode.stanis.me/008-djb2/)) to encode the command string into an `unsigned long`.
One thing to note is that the output of the hashing function is dependent on the architecture.
On most Arduino-compatible microcontrollers, the `unsigned long` data type represents 32-bits.
However, on others it may be 64-bits.
If you find that the library is throwing an error about invalid keys, this may be the problem.

## Device Settings

The device settings are stored in a structure called `settings`.
To access them at any time you may call: `settings.accelerometerOffset`, for instance.
Settings can be loaded or saved to a JSON file stored in an onboard filesystem using the `loadConfigurationsFromJSON()` function.
Note that for now, the only supported filesystem for this feature is SPIFFS, commonly used on the ESP32 platforms (this is being addressed in [Issue #2]([url](https://github.com/Legohead259/xioAPI-Arduino/issues/2)))

To change settings, it is recommended to use the `updateSetting()` function, though for now, you will need to pass in the new value manually as a `JsonVariant` object.
This will prevent unintended behavior from affecting the rest of the system.

## Connecting x-IMU3 GUI to your Arduino

Plug you Arduino into a USB port on your PC and work out the name of the port it is connected to. You can see this in the Arduino IDE. Then open up the x-IMU3 GUI app, click on `Connection` (top left) -> `New USB Connection`. In the dialogue box that pops up, select the port with the Arduino connect and click on Connect.

## Using the xioAPI-Arduino Library
