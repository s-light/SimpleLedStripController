/******************************************************************************

    for more information have a look at effect.cpp

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



#ifndef effect_base_H_
#define effect_base_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <FastLED.h>

template <uint16_t PIXEL_COUNT>
class EffectBase {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor
    EffectBase() {};
    ~EffectBase() {};

    // basic library api
    void update() {
        fill_solid(pixels, PIXEL_COUNT, CHSV(142, 100, brightness));
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations
    CRGBArray<PIXEL_COUNT> pixels;

    // parameter handling
    enum class PARAM {
        BRIGHTNESS,
    };

    void parameter_set_current(PARAM parameter) {
        parameter_current = parameter;
    };

    void parameter_next() {
        switch (parameter_current) {
            case PARAM::BRIGHTNESS: {
                parameter_current = PARAM::BRIGHTNESS;
            } break;
        }
    };

    void change_parameter(uint8_t value) {
        switch (parameter_current) {
            case PARAM::BRIGHTNESS: {
                brightness = value;
            } break;
        }
    };

protected:
    PARAM parameter_current = PARAM::BRIGHTNESS;
    uint8_t brightness = 255;

};  // class EffectBase

#endif  // effect_base_H_
