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
// === OVERLOAD FUNCTIONS ===
// ==========================


/**
 * @override super::strobe()
*/
void ThetisAPI::strobe() {
    uint32_t startTime = millis();
    while((millis() - startTime) <= 5000) // Blink white for 5 seconds
        blink(250, WHITE);
}

/**
 * @override super::color()
*/
void ThetisAPI::colour(const char* colorString) {
    Serial.println(colorString);
    uint32_t color = hexStringToUnsignedLong(colorString);
    Serial.println(color);
    setLEDColor(color);
}