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



#ifndef effect_plasma_H_
#define effect_plasma_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <FastLED.h>

#include "./mapping.h"

#include "effect_base.h"

template <uint16_t PIXEL_COUNT>
class EffectPlasma: public EffectBase<PIXEL_COUNT> {
public:
    // constructor
    EffectPlasma() {
        this->duration_min = 10000;
        this->duration = 60000;
    };
    // ~EffectPlasma() {};

    virtual void print_name(Print &out) {
        out.print("Plasma");
    }

    // basic library api
    void update() {
        // double offset = map_range_01_to(this->position, 0.0, (PI * 30.0));
        double offset = map_range_01_to(
            static_cast<double>(this->position), 0.0, (PI * 30.0));

        double row = 0.0;
        double cy = row + 0.5 * cos(offset / 3);
        double cy2 = cy * cy;

        for (size_t pixel_i = 0; pixel_i < PIXEL_COUNT; pixel_i++) {
            double pixel_offset = pixel_i * 1.0 / PIXEL_COUNT;
            // double offset = this->position + (pixel_offset * spread);

            double col = pixel_offset;
            // double row = 0.0;

            // double col = 0.0;
            // double row = pixel_offset;

            // calculate plasma
            // mostly inspired by
            // https://www.bidouille.org/prog/plasma
            // moving rings
            double cx = col + 0.5 * sin(offset / 5);
            double cx2 = cx * cx;
            // double cy = row + 0.5 * cos(offset / 3);
            // double cy2 = cy * cy;
            double xy_value = sin(
                sqrt(100 * (cx2 + cy2) + 1)
                + offset);
            // mapping
            double pixel_hue = map_range_11_to(
                xy_value,
                // self._hue_min, self._hue_max
                // 0.0, 0.08
                hue - spread, hue + spread);
            // double pixel_hue = map_range(
            //     xy_value,
            //     -1.0, 1.0,
            //     // self._hue_min, self._hue_max
            //     // 0.0, 0.08
            //     hue - 0.1, hue + 0.1);
            // double pixel_saturation = map_range(
            //     xy_value,
            //     -1.0, 1.0,
            //     1.0, 1.0);
            // double pixel_value = map_range(
            //     xy_value,
            //     1.0, -1.0,
            //     // self._contrast_min, self._contrast_max
            //     // -0.005, 1.0
            //     1.0 - contrast, 1.0);
            // map to color
            // CHSV pixel_hsv = CHSV(pixel_hue, pixel_saturation, pixel_value);

            // uint8_t offset_int = offset * 255;
            uint8_t hue_int = pixel_hue * 255;
            // uint8_t saturation_int = pixel_saturation * 255;
            // uint8_t value_int = pixel_value * 255;
            uint8_t saturation_int = 255;
            uint8_t value_int = 255;
            this->pixels[pixel_i] = CHSV(hue_int, saturation_int, value_int);

        }
        // fill_rainbow(pixels, PIXEL_COUNT, position_int);
    }

    // parameter handling
    enum class PARAM {
        DURATION,
        SPREAD,
        // CONTRAST,
        HUE,
        // SATURATION,
        // BRIGHTNESS,
    };

    void parameter_next() {
        switch (this->parameter_current) {
            case PARAM::DURATION: {
                this->parameter_current = PARAM::SPREAD;
            } break;
            case PARAM::SPREAD: {
                // this->parameter_current = PARAM::CONTRAST;
                this->parameter_current = PARAM::HUE;
                // this->parameter_current = PARAM::BRIGHTNESS;
            } break;
            // case PARAM::CONTRAST: {
            //     this->parameter_current = PARAM::HUE;
            //     // this->parameter_current = PARAM::BRIGHTNESS;
            // } break;
            case PARAM::HUE: {
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
                this->parameter_set_duration_relative(value);
            } break;
            case PARAM::SPREAD: {
                double temp = spread + value * 0.01;
                temp = clamp(temp, 0.0, 1.0);
                spread = temp;
            } break;
            // case PARAM::CONTRAST: {
            //     double temp = contrast + value * 0.01;
            //     temp = clamp(temp, 0.0, 2.0);
            //     contrast = temp;
            // } break;
            case PARAM::HUE: {
                double temp = hue + value * 0.01;
                temp = clamp(temp, 0.0, 1.0);
                hue = temp;
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
            // case PARAM::CONTRAST: {
            //     out.print(F("CONTRAST"));
            // } break;
            case PARAM::HUE: {
                out.print(F("HUE"));
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
            // case PARAM::CONTRAST: {
            //     out.print(contrast);
            // } break;
            case PARAM::HUE: {
                out.print(hue);
            } break;
            // case PARAM::SATURATION: {
            //     out.print(saturation);
            // } break;
            // case PARAM::BRIGHTNESS: {
            //     out.print(brightness);
            // } break;
        }
    };

    // configurations
    double spread = 0.13;
    // double contrast = 0.5;
    double hue = 0.5;
    // uint8_t brightness = 255;

private:
    PARAM parameter_current;
};  // class EffectPlasma

#endif  // effect_plasma_H_
