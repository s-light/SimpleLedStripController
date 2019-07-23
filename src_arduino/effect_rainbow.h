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


    CRGBArray<PIXEL_COUNT_OVERLAY> spread_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     spread.pixels_overlay[i] = CRGB::Green;
        // }
        spread.pixels_overlay(
            spread.BORDER,
            spread.BORDER_END
        ).fill_gradient(
            CHSV(50, 255, 0),
            CHSV(50, 255, 255)
        );
        return spread.pixels_overlay;
    }

    void spread_set_relative(int16_t offset) {
        double temp = spread + offset * 0.01;
        spread = clamp(temp, 0.0, 10.0);
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, double> spread = {
        "spread",
        0.0,
        10.0,
        0.5,
        std::bind(
            &EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::spread_render_overlay,
            this
        ),
        std::bind(
            &EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::spread_set_relative,
            this,
            std::placeholders::_1
        ),
    };

    // CRGBArray<PIXEL_COUNT_OVERLAY> hue_render_overlay() {
    //     // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //     //     hue.pixels_overlay[i] = CRGB::Green;
    //     // }
    //     hue.pixels_overlay(
    //         hue.BORDER,
    //         hue.BORDER_END
    //     ).fill_gradient(
    //         CHSV(0, 255, 255),
    //         CHSV(255, 255, 255),
    //         LONGEST_HUES
    //     );
    //     return hue.pixels_overlay;
    // }
    //
    // ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> hue = {
    //     "hue",
    //     0,
    //     255,
    //     120,
    //     std::bind(
    //         &EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::hue_render_overlay,
    //         this
    //     ),
    // };

    CRGBArray<PIXEL_COUNT_OVERLAY> saturation_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     saturation.pixels_overlay[i] = CRGB::Black;
        // }
        saturation.pixels_overlay(
            saturation.BORDER,
            saturation.BORDER_END
        ).fill_gradient(
            // CHSV(hue, 0, brightness),
            // CHSV(hue, 255, brightness)
            CHSV(0, 0, brightness),
            CHSV(0, 255, brightness)
        );
        return saturation.pixels_overlay;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> saturation = {
        "saturation",
        0,
        255,
        255,
        std::bind(
            &EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::saturation_render_overlay,
            this
        ),
    };

    CRGBArray<PIXEL_COUNT_OVERLAY> brightness_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     brightness.pixels_overlay[i] = CRGB::Black;
        // }
        brightness.pixels_overlay(
            brightness.BORDER,
            brightness.BORDER_END
        ).fill_gradient(
            // CHSV(hue, saturation, 0),
            // CHSV(hue, saturation, 255)
            CHSV(0, saturation, 0),
            CHSV(0, saturation, 255)
        );
        return brightness.pixels_overlay;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> brightness = {
        "brightness",
        0,
        255,
        255,
        std::bind(
            &EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::brightness_render_overlay,
            this
        ),
    };


    virtual void parameter_next() {
        if (this->parameter_current == &this->duration) {
            this->parameter_current = &spread;
        } else if (this->parameter_current == &spread) {
            this->parameter_current = &this->duration;
            // this->parameter_current = &brightness;
        // } else if (this->parameter_current == &brightness) {
        //     this->parameter_current = &hue;
        } else {
            this->parameter_current = &this->duration;
        }
    };
};  // class EffectRainbow

#endif  // effect_rainbow_H_
