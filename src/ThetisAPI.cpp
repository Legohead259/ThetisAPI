/******************************************************************
    @file       ThetisAPI.cpp
    @brief      Implementation of the xioAPI for Thetis
    @author     Braidan Duffy
    @copyright  MIT License

    Code:       Braidan Duffy
    Version:    1.0.0
    Date:       17/05/2023
    Modified:   18/05/2023

    CHANGELOG:
    v1.0.0 - Original release
    
    Credit - Derived from the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
            Uses the Madgwick Quaternion Class.
******************************************************************/

#include "ThetisAPI.h"
#include <math.h>

ThetisAPI api;


// ==========================
// === OVERRIDE FUNCTIONS ===
// ==========================


/**
 * The strobe command is send to the device to strobe the LED bright white for 5 seconds.
 * The is "strobe" and the value is null.
 * This command can be used to quickly find a specific device when using multiple devices.
 * TODO: Make asynchronous (non-blocking)
 * 
 * @override super::strobe()
*/
void ThetisAPI::strobe() {
    // TODO: Send acknowledgement message
    uint32_t startTime = millis();
    while((millis() - startTime) <= 5000) // Blink white for 5 seconds
        blink(250, WHITE);
}

/**
 * The colour command is sent to the device to set the LED color.
 * The key is "colour" and the value is either a Red Green Blue (RGB) hexidecimal triplet expressed as a string, or null.
 * Setting the colour will override the default LED behavior.
 * A value of null will restore the normal behavior.
 * 
 * @param colorString the hexadecimal string representation of the RGB color
 * @override super::color()
*/
void ThetisAPI::colour(const char* colorString) {
    if (colorString == nullptr) {
        // TODO: Set the LED behavior flag back to NORMAL.
    }

    Serial.println(colorString);
    uint32_t color = hexStringToUnsignedLong(colorString);
    Serial.println(color);
    setLEDColor(color);

    // TODO: Send acknowledgement message
}