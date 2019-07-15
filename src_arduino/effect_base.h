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

    virtual void print_name(Print &out) {
        out.print("BASE");
    }

    // basic library api
    virtual void update() {
        fill_solid(pixels, PIXEL_COUNT, CHSV(142, 100, 240));
    }

    virtual void calculate_effect_position() {
        // effect_position = normalize_to_01(millis(), effect_start, effect_end);
        end = start + duration;
        position = (
            ((millis() - start) * 1.0) / (end - start)
        );  // NOLINT(whitespace/parens)
        if (position >  1.0) {
            position = 0;
            start = millis();
        }
    }


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations
    CRGBArray<PIXEL_COUNT> pixels;

    uint32_t duration = 30 * 1000; //ms
    uint32_t start = 0;
    uint32_t end = 0;
    float position = 0.0;

    // parameter handling
    // enum class PARAM {
    //     // HUE,
    //     // SATURATION,
    //     // BRIGHTNESS,
    //     // DURATION,
    //     // DURATION,
    // };

    // virtual void parameter_set_current(PARAM parameter) {
    //     parameter_current = parameter;
    // };

    virtual void parameter_next() {
        // switch (parameter_current) {
        //     case PARAM::BRIGHTNESS: {
        //         parameter_current = PARAM::BRIGHTNESS;
        //     } break;
        // }
    };

    virtual void change_parameter(__attribute__((unused)) int16_t value) {
        // switch (parameter_current) {
        //     case PARAM::BRIGHTNESS: {
        //         brightness = value;
        //     } break;
        // }
    };

    // virtual void parameter_print(Print &out, PARAM parameter) {
    //     switch (parameter) {
    //         // case STATIC_PARAM::BRIGHTNESS: {
    //         //     out.print(F("BRIGHTNESS"));
    //         // } break;
    //     }
    // };

    // virtual void parameter_current_print(Print &out) {
    // NOLINTNEXTLINE(unused-parameter)
    virtual void parameter_print_name(__attribute__((unused)) Print &out) {
        // this->parameter_print(out, parameter_current);
    };

protected:
    // PARAM parameter_current;

};  // class EffectBase

#endif  // effect_base_H_
