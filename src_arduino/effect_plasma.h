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

template <uint16_t PIXEL_COUNT, uint16_t PIXEL_COUNT_OVERLAY>
class EffectPlasma: public EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> {
public:
    // constructor
    EffectPlasma(char const * effect_name):
        EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> (effect_name)
    {
        this->duration_min = 10000;
    };
    // ~EffectPlasma() {};

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


    // configurations

    CRGBArray<PIXEL_COUNT_OVERLAY> spread_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     spread.pixels_overlay[i] = CRGB::Green;
        // }
        spread.pixels_overlay(
            spread.BORDER,
            spread.BORDER_END
        ).fill_gradient(
            CHSV(50, 255, 25),
            CHSV(50, 255, 180)
        );
        spread.draw_indicator(CHSV(50, 0, 255));
        return spread.pixels_overlay;
    }

    void spread_set_relative(int16_t offset) {
        double temp = spread + offset * 0.01;
        spread = clamp(temp, spread.value_min, spread.value_max);
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, double> spread = {
        "spread",
        0.0,
        1.0,
        0.13,
        std::bind(
            &EffectPlasma<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::spread_render_overlay,
            this
        ),
        std::bind(
            &EffectPlasma<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::spread_set_relative,
            this,
            std::placeholders::_1
        ),
    };

    CRGBArray<PIXEL_COUNT_OVERLAY> hue_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     hue.pixels_overlay[i] = CRGB::Green;
        // }
        hue.pixels_overlay(
            hue.BORDER,
            hue.BORDER_END
        ).fill_gradient(
            CHSV(0, 255, 180),
            CHSV(255, 255, 180),
            LONGEST_HUES
        );
        hue.draw_indicator(CHSV(100, 0, 255));
        return hue.pixels_overlay;
    }

    void hue_set_relative(int16_t offset) {
        double temp = hue + offset * 0.01;
        hue = clamp(temp, hue.value_min, hue.value_max);
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, double> hue = {
        "hue",
        0.0,
        1.0,
        0.5,
        std::bind(
            &EffectPlasma<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::hue_render_overlay,
            this
        ),
        std::bind(
            &EffectPlasma<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::hue_set_relative,
            this,
            std::placeholders::_1
        ),
    };

    virtual void parameter_next() {
        if (this->parameter_current == &this->duration) {
            this->parameter_current = &spread;
        } else if (this->parameter_current == &spread) {
            this->parameter_current = &hue;
        } else if (this->parameter_current == &hue) {
            this->parameter_current = &this->duration;
        } else {
            this->parameter_current = &this->duration;
        }
    };
};  // class EffectPlasma

#endif  // effect_plasma_H_
