# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<!-- 
Release sections
## [TAG] Version X.X.X - YYYY-MM-DD
### Known Bugs
### Added
### Changed
### Fixed
### Deprecated
### Removed
### Security 
-->

## [UNRELEASED] Version 1.1.0 - 2023-06
### Known Bugs
- xioAPI UDP functionality not working as intended. [#6](https://github.com/Legohead259/xioAPI-Arduino/issues/6)
  
### Added
- Added new `updateSetting()` functions that can access the setting lookup table directly
- Added new `xioMatrix` and `xioVector` types
- Added support for UDP WiFi messaging
- Added dependency for `WiFiUDP`

### Changed
- Minor refactor of `sendTime()` to `cmdReadTime()` for clarity and consistency
- Changed type of `displayName`, `ipAddress`, and `serialNumber` to character array from character pointer
- Changed `send()` to better generalize support between different interfaces

### Removed
- Removed `print()` functionality
- Removed `writeLenFeed` arguments since all calls require a linefeed
  
---

## Version 1.0.0 - 2023-06-16
### Known Bugs
- Null Pointer error on some commands
- Configuration storage only compatible with JSON in SPIFFS [#2]([url](https://github.com/Legohead259/xioAPI-Arduino/issues/2))
- ESP32-S2 does not connect to x-IMU3 GUI natively, despite the functions working as intended [#3]([url](https://github.com/Legohead259/xioAPI-Arduino/issues/3))

### Added
- Basic implementation of all xioAPI commands
- Configuration settings and handling
- Added a `readAll` command the sends all settings from the settings lookup table
- Added a `readJson` command the sends the JSON configuration setting file