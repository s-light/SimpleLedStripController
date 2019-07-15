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

// https://stackoverflow.com/a/8941363/574981
// https://www.arduino.cc/reference/en/language/functions/math/constrain/
template<class T>
const T& clamp(const T& x, const T& a, const T& b) {
    if(x < a) {
        return a;
    } else if(b < x) {
        return b;
    } else {
        return x;
    }
}

template<class T>
const T& limit(const T& x, const T& top) {
    if(x > top) {
        return top;
    } else {
        return x;
    }
}

// MultiMap
// http://arduino.cc/playground/Main/MultiMap
template<class T, uint8_t N>
T multiMap(T val, std::array<T, N> _in, std::array<T, N> _out) {
    // take care the value is within range
    // val = constrain(val, _in[0], _in[N-1]);
    if (val <= _in[0]) {
        return _out[0];
    }
    if (val >= _in[N-1]) {
        return _out[N-1];
    }

    // search right interval
    uint8_t pos = 1;  // _in[0] allready tested
    while (val > _in[pos]) {
        pos++;
    }

    // this will handle all exact "points" in the _in array
    if (val == _in[pos]) {
        return _out[pos];
    }

    // interpolate in the right segment for the rest
    return map(val, _in[pos-1], _in[pos], _out[pos-1], _out[pos]);
}

template<class T, uint8_t N>
class MultiMap {
 public:
    MultiMap(
        const std::array<T, N> in,
        const std::array<T, N> out
    ):
        in(in),
        out(out)
    {}

    T mapit(T val) {
        // take care the value is within range
        // val = std::clamp(val, _in[0], _in[N-1]);
        if (val <= in[0]) {
            return out[0];
        }
        if (val >= in[N-1]) {
            return out[N-1];
        }

        // search right interval
        uint8_t pos = 1;  // _in[0] allready tested
        while (val > in[pos]) {
            pos++;
        }

        // this will handle all exact "points" in the _in array
        if (val == in[pos]) {
            return out[pos];
        }

        // interpolate in the right segment for the rest
        return map(val, in[pos-1], in[pos], out[pos-1], out[pos]);
    }
 private:
    // std:array<T, N> in;
    // std:array<T, N> out;
    const std::array<T, N> in;
    const std::array<T, N> out;
};




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

template<class T>
T map_range(T x, T in_min, T in_max, T out_min, T out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template<class T>
T map_range_11_to(T x, T out_min, T out_max) {
    return (x - -1.0) * (out_max - out_min) / (1.0 - -1.0) + out_min;
}

// double map_range__double(
//     double x, double in_min, double in_max, double out_min, double out_max
// ) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }
//
//
// float map_range__int2float(
//     int32_t x, int32_t in_min, int32_t in_max, float out_min, float out_max
// ) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }
//
// float map_range__uint2float(
//     uint32_t x, uint32_t in_min, uint32_t in_max, float out_min, float out_max
// ) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// map_range_01_to

template<class T>
T map_range_01_to(T x, T out_min, T out_max) {
    return x * (out_max - out_min) / 1.0 + out_min;
}

// double map_range_01_to__double(
//     double x, double out_min, double out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }


// int8_t map_range_01_to__int8(
//     float x, int8_t out_min, int8_t out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }
//
// uint8_t map_range_01_to__uint8(
//     float x, uint8_t out_min, uint8_t out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }
//
// int16_t map_range_01_to__int16(
//     float x, int16_t out_min, int16_t out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }
//
// uint16_t map_range_01_to__uint16(
//     float x, uint16_t out_min, uint16_t out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }
//
// int32_t map_range_01_to__int32(
//     float x, int32_t out_min, int32_t out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }
//
// uint32_t map_range_01_to__uint32(
//     float x, uint32_t out_min, uint32_t out_max
// ) {
//     return x * (out_max - out_min) / 1.0 + out_min;
// }



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// normalize_to_01

// float normalize_to_01(
//     int8_t x, int8_t in_min, int8_t in_max
// ) {
//     // """Map value to 0..1 range."""
//     return ((x - in_min) * 1.0) / (in_max - in_min);
// }
//
// float normalize_to_01(
//     uint8_t x, uint8_t in_min, uint8_t in_max
// ) {
//     // """Map value to 0..1 range."""
//     return ((x - in_min) * 1.0) / (in_max - in_min);
// }
//
// float normalize_to_01(
//     int16_t x, int16_t in_min, int16_t in_max
// ) {
//     // """Map value to 0..1 range."""
//     return ((x - in_min) * 1.0) / (in_max - in_min);
// }
//
// float normalize_to_01(
//     uint16_t x, uint16_t in_min, uint16_t in_max
// ) {
//     // """Map value to 0..1 range."""
//     return ((x - in_min) * 1.0) / (in_max - in_min);
// }
//
// float normalize_to_01(
//     int32_t x, int32_t in_min, int32_t in_max
// ) {
//     // """Map value to 0..1 range."""
//     return ((x - in_min) * 1.0) / (in_max - in_min);
// }
//
// float normalize_to_01(
//     uint32_t x, uint32_t in_min, uint32_t in_max
// ) {
//     // """Map value to 0..1 range."""
//     return ((x - in_min) * 1.0) / (in_max - in_min);
// }


#endif  // special_mapping_H_
