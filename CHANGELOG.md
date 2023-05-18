# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<!-- 
Release sections
### Known Bugs
### Added
### Changed
### Fixed
### Deprecated
### Removed
### Security 
-->
## [UNRELEASED] 1.5.2 - 2023-02-17

### Added
- Added functions to change and log the system state
- Added minimally viable product for integrating the magnetometer ([#29](https://github.com/Legohead259/ThetisLib/issues/29))
- Added WiFi compile flags ([#28](https://github.com/Legohead259/ThetisLib/issues/28)) 
- Added minimally viable product for integrating the LiPo battery gauge ([#31](https://github.com/Legohead259/ThetisLib/issues/31))
- Added compile flags for MAX17048 to prevent inclusion on earlier hardware revisions ([#42](https://github.com/Legohead259/ThetisLib/issues/42))

### Fixed
- Fixed NeoPixel functionality ([#25](https://github.com/Legohead259/ThetisLib/issues/25))
- Fixed RTC timestamp issue ([#21]https://github.com/Legohead259/ThetisLib/issues/21)

### Changed
- Updated Git issue templates to include F5 and G2 ([#23](https://github.com/Legohead259/ThetisLib/issues/23))
- Changed how logging is handled. Now it is coordinated with a hardware timer to log at a user-configurable rate ([#41]https://github.com/Legohead259/ThetisLib/issues/41)
- Changed LSM6DSO32 initialization to check both I2C addresses at startup ([#43](https://github.com/Legohead259/ThetisLib/issues/43))

---

## [UNRELEASED] 1.5.1 - 2022-12-27

### Added
- Added HTTP server to handle GET requests for log enable and disable

---

## 1.5.0 - 2022-12-25

### Known bugs
- Configurations not loading properly into functions ([#22](https://github.com/Legohead259/ThetisLib/issues/22)

### Added
- Added a File Transfer Protocol (FTP) server
- Added WiFi client initialization functions
- Added configuration options for FTP server and username

### Changed
- Migrated sensor data to use the `trace` log level
- Renamed the WiFi source files to fit current naming conventions

### Fixed
- Fixed compile error with `tm` and WiFi functions

---

## 1.4.0 - 2022-11-23

### Added
- Added a massive rework to the logging system
  - Added seven different logging levels that can be used to track events
- Consolidated all logging functions into an object-oriented approach, allowing more flexibility
- Loggers can specify which streams they send data to and what the information looks like
- Ability to dynamically switch between streaming the event log to the Serial monitor or the file system, depending on USB connection
- Expanded the configuration options that can be saved to SPIFFS
  - Log level for print streaming
  - Log level for event file
- Added various `info`, `fatal`, and `verbose` statements throughout code to track flow

### Changed
- Changed static logging behavior to object-oriented
- Moved `fusion.h` to `AHRS/` folder since that makes more sense
- Removed state update behavior in `updateSystemState()`, for now

### Deprecated
- Deprecated the old `logging` files - they currently have the file extension `.bkp` for safekeeping - remove in next update!

### Removed
- Removed BNO055 files
  
---

## 1.3.0 - 2022-11-20 

### Added
- Added `pollGPS()` function
- Added raw IMU readings to logging functions
- Added raw IMU readings to data packet
- Added a function to log data to the file in a binary format
- Added an `updateTimestamp()` function to update the data packet epoch
- Added ability to change IMU information from configuration data
- Added ability to change sensor fusion update rate from configuration data
- Expanded the configuration options that can be saved to SPIFFS
  - More device information
  - WiFi information
  - Sensor information
  - Logging information

### Changed
- Changed log file extension to 'bin' instead of 'csv'
- Changed configuration parsing behavior to be more efficient to add new parameters
- Changed `configData` structure to reflect to configuration options
- Changed some names in `configData` structure

### Fixed
- Fixed minor typos throughout

### Deprecated
- Deprecated all human-readable logging functions

## 1.2.0 - 2022-10-06
- Major refactor

### Added
- Certain telemetry can now be dynamically logged using compiler build flags
- Added GPS timestamp segments to the data structure
- Added `initSPIFFS()` function to filesystem
- Added a `fusion` file to handle the IMU sensor fusion and filtering
- Added several dependency libraries
- Added `data.cpp` source file

### Changed
- Changed type of `state` parameter in the telemetry data structure to `uint8_t` to remove cyclic dependency. `status_t` can be cast to `uint8_t`
- Renamed the data structure from `telemetry` to `data` to better reflect its use in the code
- Migrated all time stamping functions in `logging`
- Changed the return type of `initNeoPixel()` to `bool` for consistency. **NOTE**: This only returns true, in the future we should change it so that it can detect when the NEOPIXEL_EN gate is open
- Moved all sensor fusion functions from `lsm6dso32` to `fusion` to make future development easier
- Changed declaration of `data` to fix memory addressing error

### Removed
- Removed the `packetSize` parameter from the telemetry data structure. This can be replaced by `sizeof(data)` in practice.
- Removed the Kalman filter files since they are being replaced by the proper Kalman library downloaded from PIO
- Removed MicroNMEA library since it is now a downloadable dependency
- Removed `gps_enable` parameter from the configuration data structure
---

## 1.1.6 - 2022-07-29
### Added
- Added configuration data struct to `ThetisConfig`

### Changed
- Imported the `loadConfigurations()` function from the main application into `ThetisConfig`
- Changed `DEBUG_SERIAL` to `DEBUG_SERIAL_PORT` in  `ThetisConfig`
- Changed `DEBUG_SERIAL` to `DEBUG_SERIAL_PORT` in `ThetisFs`
---

## 1.1.5 - 2022-07-22
### Fixed
- Compile error with `currentState`; moved declaration to main application
- Compile error with `writeTelemetryData()`; moved function to main application

## 1.1.4 - 2022-07-22 [REVOKED]
### Added
- Added `GPS_DEBUG` flag support for debug print statements with the GPS radio ([#6](https://github.com/Legohead259/ThetisLib/issues/6))
- Added 'BNO055_DEBUG` flag support for debug print statements with the BNO055 sensor ([#6](https://github.com/Legohead259/ThetisLib/issues/6))
- Added 'LSM6DSO_DEBUG` flag support for debug print statements with the LSM6DSO32 sensor ([#6](https://github.com/Legohead259/ThetisLib/issues/6))

### Changed
- Moved `writeTelemetryData()` to `ThetisFS` instead of main library file

## 1.1.3 - 2022-07-22
### Added
- Added `NEOPIXEL_DEBUG` flag support for debug print statements with the NeoPixel features ([#6](https://github.com/Legohead259/ThetisLib/issues/6)) 
### Changed
- Changed LED pulse behavior from a linear cylce to sinusoidal

### Fixed
- Fixed LED pulse behavior to be more noticable ([#7](https://github.com/Legohead259/ThetisLib/issues/7))
---

## 1.1.2 - 2022-07-20
### Fixed
- Fixed debug serial compile error by declaring it at the top of `ThetisFS.h`. NOTE: This needs to be done in the Thetis variants file
- Fixed compile error due to old `tm` variable calls in `ThetisLib.h`; replaced with `timeElements`.
---

## 1.1.1 - 2022-07-20
### Fixed
- Compile error due to a random 'S' in `ThetisLib.h`
---

## 1.1.0 - 2022-07-20
### Changed
- Moved status enumerator to main library file from NeoPixel library
- Changed `telemetry_t.state` data type to `Status_t` from `uint8_t`
---

## 1.0.0 - 2022-07-17
### Added
- Issue templates
- Added License
- Information required for release
---

## 0.3.0 - 2022-07-15
### Added
- Added config file library and functions

### Fixed
- Renamed time elements variable, `tm` to maintain compatibility with WiFi libraries

### Removed
- Dependancy on Ethernet library
---

## 0.2.2 - 2022-07-14
### Fixed
- Buffer overflow error when formatting timestamp ([Thetis-Firmware #1](https://github.com/Legohead259/Thetis-Firmware/issues/1))

## 0.2.1 - 2022-07-01
### Known Bugs
- [CRITICAL] Thetis crashes on log enable ([Thetis-Firmware #1](https://github.com/Legohead259/Thetis-Firmware/issues/1))

### Added
- Broke out NeoPixel functions to seperate files
- Added ability to set headers when calling `initLogFile()`
- Added function to parse system time to ISO8601 standard

### Changed
- Changed library version number in properties file

### Fixed
- Arguments issue with prototype functions in `ThetisFS.h`
- Major bug in `getISO8601Time_RTC()` prototype function definition in `ThetisLib.h`
- Various oither minor bugs
- Log enable counter not starting properly

### Removed
- Removed some redundant NeoPixel functions
---

## 0.2.0 - 2022-06-25
### Added
- Added filters and IMU helps to the IMU library
- 
### Changed
- Change Mahoney AHRS to accept different kp and ki and refresh rate parameters on construction
- Began fragmenting library into seperate header files in corresponding folders
---

## 0.1.3 - 2022-06-17
### Added
- Added 2nd Order Butterworth filter
---

## 0.1.2 - 2022-06-10
### Added
- Added telemetry file handlers
- Added the telemetry data packet definition
---

## 0.1.1 - 2022-06-03
### Added
- Added GPS callback functions and global functions

### Fixed
- Fix a bug with the reference pointer in `initLSM6DSO32()`

### Removed
- Removed DEBUG statements in the log button ISR
---

## 0.1.0 - 2022-06-01
### Added 
- Core functionality
- Added initialization functions for BNO055 and LSM6DSO
- Added GPS initialization function
- Added NeoPixel functions
---
