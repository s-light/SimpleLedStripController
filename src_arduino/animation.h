/******************************************************************************

    __doc__ = """
    animation.py - TLC5971 & FancyLED & 2D Array / Mapping.

    it combines the TLC5971 library with FancyLED and 2D Array / Mapping.

    Enjoy the colors :-)
    """

    libraries used:
        ~ slight_DebugMenu
        ~ slight_FaderLin
        ~ slight_TLC5957
            written by stefan krueger (s-light),
                git@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT

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



#ifndef MyAnimation_H_
#define MyAnimation_H_

// include Core Arduino functionality
#include <Arduino.h>

// #undef min
// #undef max
// #include <functional>

#include <slight_DebugMenu.h>



#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
// #define FASTLED_USE_GLOBAL_BRIGHTNESS 1
// i can't test this  - it needs the min and max functions defined by Arduino.h
// but the slight_* libs
// that are using std:function are undef them so the std:: namespace works..
#define FASTLED_USE_GLOBAL_BRIGHTNESS 1

#include <FastLED.h>

// #include "./color.h"
// ^ color.h is my own minimal FastLED compatible thing...

#include "./mapping.h"


#include "./effect_base.h"
#include "./effect_static.h"
#include "./effect_rainbow.h"
#include "./effect_plasma.h"

#include "parameter_base.h"

class MyAnimation {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor
    MyAnimation();
    ~MyAnimation();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // modes
    enum class MODE {
        OFF,
        FADE,
        RUN,
    };
    MODE mode = MODE::OFF;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // pixels
    static const uint16_t PIXEL_COUNT = 144*3;

    // Parameter overlay is 50pixel == ~35cm
    static const uint16_t PIXEL_COUNT_OVERLAY = 50;
    static const uint16_t PIXEL_OVERLAY_START = (PIXEL_COUNT / 2) - (PIXEL_COUNT_OVERLAY / 2);
    static const uint16_t PIXEL_OVERLAY_END = PIXEL_OVERLAY_START + PIXEL_COUNT_OVERLAY;

    // CRGB pixels[PIXEL_COUNT];
    CRGBArray<PIXEL_COUNT> pixels;
    // CRGBSet overlay(pixels, PIXEL_COUNT);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // FastLED helper
    void show();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations
    bool animation_run = true;


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // effects

    EffectBlack<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_black = {"black"};
    EffectStatic<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_static = {"static"};
    EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_rainbow = {"rainbow"};
    EffectPlasma<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_plasma = {"plasma"};

    EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> * fx_last = &fx_static;
    EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> * fx_current = &fx_plasma;
    EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> * fx_next = &fx_rainbow;

    void select_next_effect();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // parameters

    // global_effect
    CRGBArray<PIXEL_COUNT_OVERLAY> global_effect_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     global_effect.pixels_overlay[i] = CRGB::Black;
        // }
        uint16_t temp_count = (global_effect.BORDER_END - global_effect.BORDER);
        temp_count -= (3*global_effect.BORDER);
        temp_count = temp_count / 3;

        const uint8_t brightness_active = 255;
        const uint8_t brightness_inactive = 100;

        // static
        uint8_t temp_brightness = brightness_inactive;
        if (global_effect.value == 0) {
            temp_brightness = brightness_active;
        } else {
            temp_brightness = brightness_inactive;
        }
        uint16_t start = global_effect.BORDER;
        global_effect.pixels_overlay(start, start + temp_count).fill_gradient(
            CHSV(0, 0, temp_brightness),
            CHSV(0, 0, temp_brightness)
        );
        // rainbow
        if (global_effect.value == 1) {
            temp_brightness = brightness_active;
        } else {
            temp_brightness = brightness_inactive;
        }
        start += temp_count + global_effect.BORDER;
        global_effect.pixels_overlay(start, start + temp_count).fill_gradient(
            CHSV(0, 255, temp_brightness),
            CHSV(255, 255, temp_brightness),
            LONGEST_HUES
        );
        // plasma
        if (global_effect.value == 2) {
            temp_brightness = brightness_active;
        } else {
            temp_brightness = brightness_inactive;
        }
        start += temp_count + global_effect.BORDER;
        global_effect.pixels_overlay(start, start + temp_count).fill_gradient(
            CHSV(170, 255, temp_brightness),
            CHSV(200, 255, temp_brightness)
        );
        return global_effect.pixels_overlay;
    }

    void global_effect_set_relative(int16_t offset) {
        // ignore acceleration...
        // if (offset > 0) {
        //     global_effect = global_effect + 1;
        // } else if (offset < 0) {
        //     global_effect = global_effect - 1;
        // }
        int16_t temp = global_effect + offset;
        // wrap around
        if (temp > global_effect.value_max) {
            if (offset > 0) {
                temp = global_effect.value_min;
            } else {
                temp = global_effect.value_max;
            }
        } else if (temp < global_effect.value_min) {
            if (offset > 0) {
                temp = global_effect.value_min;
            } else {
                temp = global_effect.value_max;
            }
        }
        global_effect = temp;
    }

    uint8_t global_effect_set(uint8_t value_new) {
        // Serial.println("global_effect_set custom ");
        // Serial.print("global_effect_set custom ");
        // Serial.print(value_new);
        // Serial.println();
        switch (value_new) {
            case 0: {
                fx_current = &fx_static;
            } break;
            case 1: {
                fx_current = &fx_rainbow;
            } break;
            case 2: {
                fx_current = &fx_plasma;
            } break;
            default: {
                fx_current = &fx_static;
            }
        }
        Print &out = Serial;
        fx_current->print_name(out);
        out.print(" ");
        return value_new;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> global_effect = {
        "global_effect",
        0,
        2,
        0,
        std::bind(
            &MyAnimation::global_effect_render_overlay,
            this
        ),
        std::bind(
            &MyAnimation::global_effect_set_relative,
            this,
            std::placeholders::_1
        ),
        std::bind(
            &MyAnimation::global_effect_set,
            this,
            std::placeholders::_1
        ),
    };



    // global_brightness
    CRGBArray<PIXEL_COUNT_OVERLAY> global_brightness_render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     global_brightness.pixels_overlay[i] = CRGB::Black;
        // }
        global_brightness.pixels_overlay(
            global_brightness.BORDER,
            global_brightness.BORDER_END
        ).fill_gradient(
            CHSV(0, 0, 25),
            CHSV(0, 0, 200)
        );
        global_brightness.draw_indicator(CHSV(100, 255, 255));
        return global_brightness.pixels_overlay;
    }

    uint8_t global_brightness_set(uint8_t value_new) {
        // Serial.println("custom global_brightness_set called...");
        if (value_new > 0) {
            FastLED.setBrightness(value_new);
        } else {
            FastLED.setBrightness(1);
        }
        return value_new;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint8_t> global_brightness = {
        "global_brightness",
        0,
        255,
        1,
        std::bind(
            &MyAnimation::global_brightness_render_overlay,
            this
        ),
        nullptr,
        std::bind(
            &MyAnimation::global_brightness_set,
            this,
            std::placeholders::_1
        ),
    };



    // global_overwrite
    void overwrite_set(uint16_t start, uint16_t end);
    void overwrite_set_relative(int16_t value);
    uint16_t overwrite_start_get();
    uint16_t overwrite_end_get();

    CRGBArray<PIXEL_COUNT_OVERLAY> global_overwrite_render_overlay() {
        for (
            int i = global_overwrite.BORDER;
            i < global_overwrite.BORDER_END;
            i++
        ) {
            if (i % global_overwrite.BORDER != 0) {
                global_overwrite.pixels_overlay[i] = CRGB::Black;
            } else {
                global_overwrite.pixels_overlay[i] = CRGB::White;
            }
        }
        return global_overwrite.pixels_overlay;
    }

    virtual void global_overwrite_set_relative(int16_t offset) {
        overwrite_set_relative(offset);
        global_overwrite = overwrite_start;
    }

    ParameterTyped<PIXEL_COUNT_OVERLAY, uint16_t> global_overwrite = {
        "global_overwrite",
        0,
        PIXEL_COUNT,
        0,
        std::bind(
            &MyAnimation::global_overwrite_render_overlay,
            this
        ),
        std::bind(
            &MyAnimation::global_overwrite_set_relative,
            this,
            std::placeholders::_1
        ),
    };


    ParameterBase<PIXEL_COUNT_OVERLAY> * global_current = &global_effect;

    void global_next() {
        if (global_current == &global_effect) {
            global_current = &global_brightness;
        } else if (global_current == &global_brightness) {
            global_current = &global_overwrite;
        } else if (global_current == &global_overwrite) {
            global_current = &global_effect;
        }
    };


    ParameterBase<PIXEL_COUNT_OVERLAY> * param_for_overlay = nullptr;

    // bool render_overlay_global = false;
    // bool render_overlay_effect = false;

    // using parameter_overlay_func_t = void (*)();
    // using parameter_overlay_func_t = std::function<void()>;
    // using parameter_overlay_func_t = CRGBArray<PIXEL_COUNT_OVERLAY> (*)();
    // using parameter_overlay_func_t =
    //     void (ParameterBase<PIXEL_COUNT_OVERLAY>::*render_overlay)();
    // using parameter_overlay_func_t =
    //     std::function<void ParameterBase<PIXEL_COUNT_OVERLAY>::*render_overlay()>;
    // using parameter_overlay_func_t =
    //     std::function<CRGBArray<PIXEL_COUNT_OVERLAY> ()>;

    // using parameter_overlay_func_t =
    //     CRGBArray<PIXEL_COUNT_OVERLAY> (MyAnimation::*)();

    // parameter_overlay_func_t parameter_overlay_func = nullptr;
    // parameter_overlay_func_t parameter_overlay_func =
    //     &MyAnimation::render_overlay_EFFECT;
    // parameter_overlay_func_t parameter_overlay_func = std::bind(
    //         &MyAnimation::render_overlay_EFFECT, this);



    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // output
    void output_toggle();
    bool output_get();
    void output_on();
    void output_off();


private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    // animation
    void animation_init(Stream &out);
    void animation_update();
    void fps_update();
    void overwrite_black();

    void render_parameter_overlay();
    void parameter_activate_overlay();

    uint16_t overwrite_start = 0;
    uint16_t overwrite_end = 0;


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;
    const uint8_t leave_every_n_pixel_on = 15;

    static const uint8_t psu_off_pin = 7;
    static const uint8_t psu_on_pin = 9;
    static const uint8_t output_active_pin = 2;
    bool output_active = false;

    uint32_t fps_start = 0;
    uint32_t fps_duration = 10 * 1000;  // ms
    uint32_t fps_loopcount = 0;
};  // class MyAnimation

#endif  // MyAnimation_H_
