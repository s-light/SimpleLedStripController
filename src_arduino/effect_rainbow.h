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



#ifndef effect_rainbow_H_
#define effect_rainbow_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <FastLED.h>

#include "./mapping.h"

#include "effect_base.h"

template <uint16_t PIXEL_COUNT, uint16_t PIXEL_COUNT_OVERLAY>
class EffectRainbow: public EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> {
public:
    // constructor
    EffectRainbow(char const * effect_name):
        EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> (effect_name)
    {
        // nothing to do.
    };
    // ~EffectRainbow() {};

    // basic library api
    void update() {
        for (size_t pixel_i = 0; pixel_i < PIXEL_COUNT; pixel_i++) {
            float pixel_offset = pixel_i * 1.0 / PIXEL_COUNT;
            float offset = this->position + (pixel_offset * spread);
            uint8_t offset_int = offset * 255;
            uint8_t hue = offset_int;
            this->pixels[pixel_i] = CHSV(hue, saturation, brightness);
        }
        // fill_rainbow(pixels, PIXEL_COUNT, position_int);
    }


    // parameter handling
    enum class PARAM {
        DURATION,
        SPREAD,
        // SATURATION,
        // BRIGHTNESS,
    };

    void parameter_next() {
        switch (this->parameter_current) {
            case PARAM::DURATION: {
                this->parameter_current = PARAM::SPREAD;
            } break;
            case PARAM::SPREAD: {
                this->parameter_current = PARAM::DURATION;
                // this->parameter_current = PARAM::BRIGHTNESS;
            } break;
            // case PARAM::BRIGHTNESS: {
            //     this->parameter_current = PARAM::DURATION;
            // } break;
        }
    };

    virtual void change_parameter(int16_t value) {
        // EffectBase<PIXEL_COUNT>::change_parameter(value);
        switch (this->parameter_current) {
            case PARAM::DURATION: {
                // this->parameter_set_duration_relative(value);
            } break;
            case PARAM::SPREAD: {
                double temp = spread + value * 0.01;
                temp = clamp(temp, 0.0, 10.0);
                spread = temp;
            } break;
            // case PARAM::SATURATION: {
            //     color_hsv.saturation = value;
            // } break;
            // case PARAM::BRIGHTNESS: {
            //     color_hsv.value = value;
            // } break;
        }
    };

    void parameter_print_name(Print &out) {
        switch (this->parameter_current) {
            case PARAM::DURATION: {
                out.print(F("DURATION"));
            } break;
            case PARAM::SPREAD: {
                out.print(F("SPREAD"));
            } break;
            // case PARAM::SATURATION: {
            //     out.print(F("SATURATION"));
            // } break;
            // case PARAM::BRIGHTNESS: {
            //     out.print(F("BRIGHTNESS"));
            // } break;
        }
    };

    void parameter_print_value(Print &out) {
        switch (this->parameter_current) {
            case PARAM::DURATION: {
                out.print(this->duration);
            } break;
            case PARAM::SPREAD: {
                out.print(spread);
            } break;
            // case PARAM::SATURATION: {
            //     out.print(saturation);
            // } break;
            // case PARAM::BRIGHTNESS: {
            //     out.print(brightness);
            // } break;
        }
    };

    // // virtual void render_overlay() {
    // virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
    //     switch (this->parameter_current) {
    //         case PARAM::DURATION: {
    //             for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //                 this->pixels_overlay[i] = CRGB::Black;
    //             }
    //         } break;
    //         case PARAM::SPREAD: {
    //             for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //                 this->pixels_overlay[i] = CRGB::Black;
    //             }
    //         } break;
    //         // case PARAM::SATURATION: {
    //         //     for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //         //         this->pixels_overlay[i] = CRGB::Black;
    //         //     }
    //         // } break;
    //         // case PARAM::BRIGHTNESS: {
    //         //     for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //         //         this->pixels_overlay[i] = CRGB::Black;
    //         //     }
    //         // } break;
    //     }
    //     return this->pixels_overlay;
    // };

    // configurations
    float spread = 0.5;
    uint8_t saturation = 255;
    uint8_t brightness = 255;

private:
    PARAM parameter_current;
};  // class EffectRainbow

#endif  // effect_rainbow_H_
