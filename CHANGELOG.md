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