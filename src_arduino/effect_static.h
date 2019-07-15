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



#ifndef effect_static_H_
#define effect_static_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <FastLED.h>

#include "effect_base.h"

template <uint16_t PIXEL_COUNT>
class EffectStatic: public EffectBase<PIXEL_COUNT> {
public:
    // constructor
    // EffectStatic() {};
    // ~EffectStatic() {};

    virtual void print_name(Print &out) {
        out.print("Static");
    }

    // basic library api
    void update() {
        fill_solid(this->pixels, PIXEL_COUNT, color_hsv);
    }


    // parameter handling
    enum class PARAM {
        HUE,
        SATURATION,
        // BRIGHTNESS,
    };

    void parameter_next() {
        switch (this->parameter_current) {
            case PARAM::HUE: {
                this->parameter_current = PARAM::SATURATION;
            } break;
            case PARAM::SATURATION: {
                this->parameter_current = PARAM::HUE;
                // parameter_current = PARAM::BRIGHTNESS;
            } break;
            // case PARAM::BRIGHTNESS: {
            //     parameter_current = PARAM::HUE;
            // } break;
        }
    };

    virtual void change_parameter(int16_t value) {
        switch (this->parameter_current) {
            case PARAM::HUE: {
                this->color_hsv.hue = value;
            } break;
            case PARAM::SATURATION: {
                this->color_hsv.saturation = value;
            } break;
            // case PARAM::BRIGHTNESS: {
            //     this->color_hsv.value = value;
            // } break;
        }
    };

    void parameter_print(Print &out) {
        switch (this->parameter_current) {
            case PARAM::HUE: {
                out.print(F("HUE"));
            } break;
            case PARAM::SATURATION: {
                out.print(F("SATURATION"));
            } break;
            // case PARAM::BRIGHTNESS: {
            //     out.print(F("BRIGHTNESS"));
            // } break;
        }
    };


    // configurations
    // const CHSV warm_white = CHSV(142, 100, 255);
    const CHSV warm_white = CHSV(142, 100, 240);
    CHSV color_hsv = warm_white;

private:
    PARAM parameter_current;

};  // class EffectStatic

#endif  // effect_static_H_
