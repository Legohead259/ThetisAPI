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

#include "xioAPI.h"

#include "../../misc/neopixel.h"


// =============================
// === THETIS API DEFINITION ===
// =============================


class ThetisAPI: public xioAPI {
public:
    void cmdStrobe() override;
    void cmdColour(const char* colorString) override;

private:
    
};

extern ThetisAPI api;
#endif // ThetisAPI_h