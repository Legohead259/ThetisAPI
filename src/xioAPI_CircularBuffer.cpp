/******************************************************************
    @file       xioAPI_CircularBuffer.cpp
    @brief      Circular buffer for use with logging messages from the xIO API Arduino Library
    @author     Braidan Duffy
    @copyright  Please see the accompanying LICENSE file

    Code:       Braidan Duffy

    Version:    1.0.0
    Date:       02/09/23
    Modifed:    02/09/2023

    1.0.0       Original Release.                       02/09/23

    Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
            (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
            and using information in the xIMU3 User Manual 
            (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).

******************************************************************/

#include "xioAPI_CircularBuffer.h"

CircularBuffer<uint8_t*, 4096> dataBytesBuffer;
CircularBuffer<const char*, 8192> dataASCIIBuffer;

