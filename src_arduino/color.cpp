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


// include Core Arduino functionality
#include <Arduino.h>

#include "./color.h"


CRGB hsv2rgb(CHSV hsv) {
    // based on :
    // https://github.com/adafruit/Adafruit_CircuitPython_FancyLED/blob/master/adafruit_fancyled/adafruit_fancyled.py#L72

    // Hue circle = 0.0 to 6.0
    float hue = hsv.hue * 6.0;
    // Sextant index is next-lower integer of hue
    float sxt = floor(hue);
    // Fraction-within-sextant is 0.0 to <1.0
    float frac = hue - sxt;
    // mod6 the sextant so it's always 0 to 5
    uint8_t sxt_int = static_cast<int>(sxt) % 6;

    float r = 0.0;
    float g = 0.0;
    float b = 0.0;

    switch (sxt_int) {
        case 0: {
            // Red to <yellow
            r = 1.0;
            g = frac;
            b = 0.0;
        } break;
        case 1: {
            // Yellow to <green
            r = 1.0 - frac;
            g = 1.0;
            b = 0.0;
        } break;
        case 2: {
            // Green to <cyan
            r = 0.0;
            g = 1.0;
            b = frac;
        } break;
        case 3: {
            // Cyan to <blue
            r = 0.0;
            g = 1.0 - frac;
            b = 1.0;
        } break;
        case 4: {
            // Blue to <magenta
            r = frac;
            g = 0.0;
            b = 1.0;
        } break;
        case 5: {
            // Magenta to <red
            r = 1.0;
            g = 0.0;
            b = 1.0 - frac;
        } break;
    }

    // Inverse-of-saturation
    float invsat = 1.0 - hsv.saturation;

    CRGB result_rgb = CRGB();
    result_rgb.red = ((r * hsv.saturation) + invsat) * hsv.value;
    result_rgb.green = ((g * hsv.saturation) + invsat) * hsv.value;
    result_rgb.blue = ((b * hsv.saturation) + invsat) * hsv.value;
    return result_rgb;
}

// TODO(s-light): port hsv2rgb_rainbow
// https://github.com/FastLED/FastLED/blob/master/hsv2rgb.cpp#L278


// TODO(s-light): port gamma_adjust from
// https://github.com/adafruit/Adafruit_CircuitPython_FancyLED/blob/master/adafruit_fancyled/adafruit_fancyled.py#L331
// or
// https://github.com/FastLED/FastLED/blob/master/colorutils.h#L1679
// https://github.com/FastLED/FastLED/blob/master/colorutils.cpp#L1140-L1151
