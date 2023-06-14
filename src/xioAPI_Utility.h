/******************************************************************
    @file       xioAPI_Utility.h
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

#ifndef XIOAPI_UTILITY_H
#define XIOAPI_UTILITY_H

/**
 * @brief Converts a string to an unsigned long via the DJB2 hash function
 * @note The hashing function changes depending on architecture.
 * Ensure that similar architectures are used to generate the keys being checked against.
 * 
 * @param str The string to be converted
 * 
 * @return The numerical representation of the given string as an unsigned long
*/
inline unsigned long hash(const char *str) {
    //  djb2 Hash Function
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

#endif // XIOAPI_UTILITY_H