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

template <uint16_t PIXEL_COUNT, uint16_t PIXEL_COUNT_OVERLAY>
class EffectStatic: public EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> {
public:
    // constructor
    EffectStatic(char const * effect_name):
        EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> (effect_name)
    {
        this->parameter_current = &hue;
    };
    // ~EffectStatic() {};

    // const CHSV warm_white = CHSV(142, 100, 255);
    const CHSV warm_white = CHSV(142, 100, 240);

    // parameter handling

    CRGBArray<PIXEL_COUNT_OVERLAY> hue_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     hue.pixels_overlay[i] = CRGB::Black;
        // }
        hue.pixels_overlay(
            hue.BORDER,
            hue.BORDER_END
        ).fill_gradient(
            CHSV(0, saturation, limit(brightness, 180)),
            CHSV(255, saturation, limit(brightness, 180)),
            LONGEST_HUES
        );
        // position indicator
        uint16_t pixel_indicator = map_range<uint32_t>(
            hue.value,
            hue.value_min, hue.value_max,
            hue.BORDER, hue.BORDER_END);
        hue.pixels_overlay[pixel_indicator] = CHSV(100, 0, 255);
        return hue.pixels_overlay;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> hue = {
        "hue",
        0,
        255,
        warm_white.hue,
        std::bind(
            &EffectStatic<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::hue_render_overlay,
            this
        ),
    };

    CRGBArray<PIXEL_COUNT_OVERLAY> saturation_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     saturation.pixels_overlay[i] = CRGB::Black;
        // }
        saturation.pixels_overlay(
            saturation.BORDER,
            saturation.BORDER_END
        ).fill_gradient(
            CHSV(hue, 0, limit(brightness, 180)),
            CHSV(hue, 255, limit(brightness, 180))
        );
        // position indicator
        uint16_t pixel_indicator = map_range<uint32_t>(
            saturation.value,
            saturation.value_min, saturation.value_max,
            saturation.BORDER, saturation.BORDER_END);
        saturation.pixels_overlay[pixel_indicator] = CHSV(100, 0, 255);
        return saturation.pixels_overlay;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> saturation = {
        "saturation",
        0,
        255,
        warm_white.saturation,
        std::bind(
            &EffectStatic<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::saturation_render_overlay,
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
            CHSV(hue, saturation, 25),
            CHSV(hue, saturation, 200)
        );
        // position indicator
        uint16_t pixel_indicator = map_range<uint32_t>(
            brightness.value,
            brightness.value_min, brightness.value_max,
            brightness.BORDER, brightness.BORDER_END);
        brightness.pixels_overlay[pixel_indicator] = CHSV(100, 255, 255);
        return brightness.pixels_overlay;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> brightness = {
        "brightness",
        0,
        255,
        warm_white.value,
        std::bind(
            &EffectStatic<PIXEL_COUNT, PIXEL_COUNT_OVERLAY>::brightness_render_overlay,
            this
        ),
    };


    virtual void parameter_next() {
        if (this->parameter_current == &this->duration) {
            this->parameter_current = &saturation;
        } else if (this->parameter_current == &hue) {
            this->parameter_current = &saturation;
        } else if (this->parameter_current == &saturation) {
            this->parameter_current = &hue;
            // this->parameter_current = &brightness;
        } else if (this->parameter_current == &brightness) {
            this->parameter_current = &hue;
        } else {
            this->parameter_current = &hue;
        }
    };


    // basic library api
    void update() {
        fill_solid(this->pixels, PIXEL_COUNT, CHSV(hue, saturation, brightness));
        // fill_solid(this->pixels, PIXEL_COUNT, CHSV(hue, 255, 240));
    }


};  // class EffectStatic


template <uint16_t PIXEL_COUNT, uint16_t PIXEL_COUNT_OVERLAY>
class EffectBlack: public EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> {
public:
    // constructor
    EffectBlack(char const * effect_name):
        EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> (effect_name)
    {
        // nothing to do.
    };
    // ~EffectBlack() {};

    // basic library api
    void update() {
        fill_solid(this->pixels, PIXEL_COUNT, CHSV(0, 255, 0));
    }
};  // class EffectBlack

#endif  // effect_static_H_
