/******************************************************************************

    minimal float version of FastLED / FancyLED pixel types and HSV2RGB

    Enjoy the colors :-)

    heavily based on work from:
    - https://github.com/FastLED/FastLED/
    - https://github.com/adafruit/Adafruit_CircuitPython_FancyLED

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/

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



#ifndef color_H_
#define color_H_

// include Core Arduino functionality
#include <Arduino.h>

struct CHSV {
    // based on / copied partly from:
    // https://github.com/FastLED/FastLED/blob/master/pixeltypes.h
    union {
        float hue;
        float h;
    };
    union {
        float saturation;
        float sat;
        float s;
    };
    union {
        float value;
        float val;
        float v;
    };

    // default values are UNITIALIZED
    inline CHSV() __attribute__((always_inline)) {}

    // allow construction from H, S, V
    inline CHSV( float ih, float is, float iv) __attribute__((always_inline))
        : h(ih), s(is), v(iv) {}

};

struct CRGB {
    // based on / copied partly from:
    // https://github.com/FastLED/FastLED/blob/master/pixeltypes.h
    union {
        float r;
        float red;
    };
    union {
        float g;
        float green;
    };
    union {
        float b;
        float blue;
    };

    // default values are UNINITIALIZED
	inline CRGB() __attribute__((always_inline)) {}

    // allow construction from R, G, B
    inline CRGB(float ir, float ig, float ib)  __attribute__((always_inline))
        : r(ir), g(ig), b(ib) {}
};

CRGB hsv2rgb(CHSV hsv);

// TODO(s-light): port hsv2rgb_rainbow
// https://github.com/FastLED/FastLED/blob/master/hsv2rgb.cpp#L278


// TODO(s-light): port gamma_adjust from
// https://github.com/adafruit/Adafruit_CircuitPython_FancyLED/blob/master/adafruit_fancyled/adafruit_fancyled.py#L331
// or
// https://github.com/FastLED/FastLED/blob/master/colorutils.h#L1679
// https://github.com/FastLED/FastLED/blob/master/colorutils.cpp#L1140-L1151

#endif  // color_H_
