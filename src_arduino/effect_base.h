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


// template <class T, T MIN_VALUE, T MAX, uint16_t PIXEL_COUNT_OVERLAY>
template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterBase {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor
    ParameterBase() {
        fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
    };
    ~ParameterBase() {};

    // virtual void print_name(Print &out) {
    //     out.print("ParameterBase");
    // }

    CRGBArray<PIXEL_COUNT_OVERLAY> pixels_overlay;
    static const uint16_t BORDER = PIXEL_COUNT_OVERLAY / 10;
    static const uint16_t BORDER_END = PIXEL_COUNT_OVERLAY - BORDER;

    // virtual void render_overlay() {
    virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
        for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
            this->pixels_overlay[i] = CRGB::Black;
        }
        return this->pixels_overlay;
    };

};  // class ParameterBase


template <uint16_t PIXEL_COUNT, uint16_t PIXEL_COUNT_OVERLAY>
class EffectBase: public ParameterBase<PIXEL_COUNT_OVERLAY> {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor
    EffectBase() {
        fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
    };
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
    CRGBArray<PIXEL_COUNT_OVERLAY> pixels_overlay;


    uint32_t duration_min =     300;
    const uint32_t duration_max = 1800000;
    // duration mapping:
    // ms               = step_size (factor)
    //      0..    500    =     1 (0ms - 500ms = 1ms)
    //    501..   1000    =    10 (500ms - 1s = 10ms)
    //   1001..  10000    =   100 (1s - 10s = 100ms)
    //  10001..  60000    =  1000 (10s - 1min = 1s)
    //  60001.. 300000    = 10000 (1min - 5min = 10s)
    // 300001..1800000    = 60000 (5min - 30min = 1min)
    MultiMap<uint32_t, 12> duration_factor_map{
        std::array<uint32_t, 12>{
                 0,     500,
               501,    1000,
              1001,   10000,
             10001,   60000,
             60001,  300000,
            300001, duration_max,
        },
        std::array<uint32_t, 12>{
                 1,       1,
                10,      10,
               100,     100,
              1000,    1000,
             10000,   10000,
             60000,   60000,
        }
    };

    uint32_t duration = 30 * 1000; //ms
    uint32_t start = 0;
    uint32_t end = 0;
    float position = 0.0;

    virtual void parameter_next() {
        // switch (parameter_current) {
        //     case PARAM::BRIGHTNESS: {
        //         parameter_current = PARAM::BRIGHTNESS;
        //     } break;
        // }
    };

    virtual void change_parameter(__attribute__((unused)) int16_t value) {
        // out.print(F("Param: "));
        // this->parameter_print_name(out);
        // out.print(F(" = "));
        // switch (parameter_current) {
        //     case PARAM::BRIGHTNESS: {
        //         brightness = value;
        //     } break;
        // }
    };

    virtual void parameter_print_name(__attribute__((unused)) Print &out) {
        // this->parameter_print_name(out, parameter_current);
    };

    virtual void parameter_print_value(__attribute__((unused)) Print &out) {
        // switch (this->parameter_current) {
        //     case PARAM::DURATION: {
        //         out.print(this->duration);
        //     } break;
        // }
    };

    // virtual void render_overlay() {
    virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
        for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
            this->pixels_overlay[i] = CRGB::Black;
        }
        return this->pixels_overlay;
    };

    virtual void parameter_set_duration_relative(int16_t value) {
        uint32_t factor = duration_factor_map.mapit(duration);
        int32_t value_wf = (value * factor);
        uint32_t temp = duration + value_wf;
        // constrain to full step value..
        temp = (temp / factor) * factor;
        // Serial.print("temp ");
        // Serial.println(temp);
        // Serial.print("factor ");
        // Serial.println(factor);
        // Serial.print("temp / factor ");
        // Serial.println(temp / factor);
        // Serial.print("(temp / factor) * factor ");
        // Serial.println((temp / factor) * factor);
        if (temp > duration_max) {
            if (value_wf > 0) {
                temp = duration_min;
            } else {
                temp = duration_max;
            }
        } else if (temp < duration_min) {
            if (value_wf > 0) {
                temp = duration_min;
            } else {
                temp = duration_max;
            }
        }
        duration = temp;
    }

protected:
    // PARAM parameter_current;

};  // class EffectBase

#endif  // effect_base_H_
