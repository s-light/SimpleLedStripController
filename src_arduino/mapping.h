/******************************************************************************

    some mapping helpers..

    written by stefan krueger (s-light),
        github@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
The MIT License (MIT)

Copyright (c) 2019 Stefan Krüger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/



#ifndef special_mapping_H_
#define special_mapping_H_

// include Core Arduino functionality
#include <Arduino.h>


// float map_range(
//     float x, float in_min, float in_max, float out_min, float out_max);
// float map_range(
//     int32_t x, int32_t in_min, int32_t in_max, float out_min, float out_max);
//
// float map_range_01_to(float x, float out_min, float out_max);
// int8_t map_range_01_to(float x, int8_t out_min, int8_t out_max);
// uint8_t map_range_01_to(float x, uint8_t out_min, uint8_t out_max);
// in16_t map_range_01_to(float x, in16_t out_min, in16_t out_max);
// uin16_t map_range_01_to(float x, uin16_t out_min, uin16_t out_max);
// int32_t map_range_01_to(float x, int32_t out_min, int32_t out_max);
// uint32_t map_range_01_to(float x, uint32_t out_min, uint32_t out_max);
//
// float normalize_to_01(int8_t x, int8_t in_min, int8_t in_max);
// float normalize_to_01(uint8_t x, uint8_t in_min, uint8_t in_max);
// float normalize_to_01(int16_t x, int16_t in_min, int16_t in_max);
// float normalize_to_01(uint16_t x, uint16_t in_min, uint16_t in_max);
// float normalize_to_01(int32_t x, int32_t in_min, int32_t in_max);
// float normalize_to_01(uint32_t x, uint32_t in_min, uint32_t in_max);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// map_range

float map_range(
    float x, float in_min, float in_max, float out_min, float out_max
) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double map_range__double(
    double x, double in_min, double in_max, double out_min, double out_max
) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float map_range__int2float(
    int32_t x, int32_t in_min, int32_t in_max, float out_min, float out_max
) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float map_range__uint2float(
    uint32_t x, uint32_t in_min, uint32_t in_max, float out_min, float out_max
) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// map_range_01_to


float map_range_01_to(
    float x, float out_min, float out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

// double map_range_01_to__double(
//     double x, double out_min, double out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }


int8_t map_range_01_to__int8(
    float x, int8_t out_min, int8_t out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

uint8_t map_range_01_to__uint8(
    float x, uint8_t out_min, uint8_t out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

int16_t map_range_01_to__int16(
    float x, int16_t out_min, int16_t out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

uint16_t map_range_01_to__uint16(
    float x, uint16_t out_min, uint16_t out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

int32_t map_range_01_to__int32(
    float x, int32_t out_min, int32_t out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

uint32_t map_range_01_to__uint32(
    float x, uint32_t out_min, uint32_t out_max
) {
    return x * (out_max - out_min) / 1.0 + out_min;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// normalize_to_01

float normalize_to_01(
    int8_t x, int8_t in_min, int8_t in_max
) {
    // """Map value to 0..1 range."""
    return ((x - in_min) * 1.0) / (in_max - in_min);
}

float normalize_to_01(
    uint8_t x, uint8_t in_min, uint8_t in_max
) {
    // """Map value to 0..1 range."""
    return ((x - in_min) * 1.0) / (in_max - in_min);
}

float normalize_to_01(
    int16_t x, int16_t in_min, int16_t in_max
) {
    // """Map value to 0..1 range."""
    return ((x - in_min) * 1.0) / (in_max - in_min);
}

float normalize_to_01(
    uint16_t x, uint16_t in_min, uint16_t in_max
) {
    // """Map value to 0..1 range."""
    return ((x - in_min) * 1.0) / (in_max - in_min);
}

float normalize_to_01(
    int32_t x, int32_t in_min, int32_t in_max
) {
    // """Map value to 0..1 range."""
    return ((x - in_min) * 1.0) / (in_max - in_min);
}

float normalize_to_01(
    uint32_t x, uint32_t in_min, uint32_t in_max
) {
    // """Map value to 0..1 range."""
    return ((x - in_min) * 1.0) / (in_max - in_min);
}


#endif  // special_mapping_H_
