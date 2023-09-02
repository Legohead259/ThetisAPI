/******************************************************************
    @file       xioAPI_CircularBuffer.h
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

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <Arduino.h>

template <typename T, size_t Size>
class CircularBuffer {
public:
    CircularBuffer() { clear(); }
    
    void write(T data, size_t dataSize) {
        // Write data to the buffer with wrap-around
        while (dataSize--) {
            buffer[writeIndex] = data++;
            writeIndex = (writeIndex + 1) % Size;  // Wrap around if necessary
            if (writeIndex == readIndex) {
                // Buffer is full, move readIndex to prevent data overwrite
                readIndex = (readIndex + 1) % Size;
            }
        }
    }

    void readAll(T outBuf) {
        while (readIndex != writeIndex) {
            outBuf++ = buffer[readIndex];
            readIndex = (readIndex + 1) % Size;  // Wrap around if necessary
        }
    }

    bool isEmpty() { return (readIndex == writeIndex); }
    void clear() { readIndex = writeIndex = 0; }

private:
    T buffer[Size];
    int writeIndex = 0;  // Index to write data into the buffer
    int readIndex = 0;   // Index to read data from the buffer
};

extern CircularBuffer<uint8_t*, 4096> dataBytesBuffer;
extern CircularBuffer<const char*, 8192> dataASCIIBuffer;

#endif