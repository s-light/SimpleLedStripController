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

#undef min
#undef max
#include <functional>

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









template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterGlobal: public ParameterBase<PIXEL_COUNT_OVERLAY> {
public:
    ParameterGlobal() {
        uint16_t temp = this->BORDER/2;
        this->pixels_overlay[temp] = CHSV(200, 255, 200);
        this->pixels_overlay[this->BORDER_END - temp] = CHSV(200, 255, 200);
    };
};  // class ParameterGlobal


template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterGlobalBRIGHTNESS: public ParameterGlobal<PIXEL_COUNT_OVERLAY> {
public:
    // virtual void render_overlay() {
    virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     this->pixels_overlay[i] = CRGB::Black;
        // }
        this->pixels_overlay(this->BORDER, this->BORDER_END).fill_gradient(
            CHSV(0, 0, 0),
            CHSV(0, 0, 255)
        );
        // this->pixels_overlay[]
        return this->pixels_overlay;
    };
};  // class ParameterGlobalBRIGHTNESS


template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterGlobalOVERWRITE: public ParameterGlobal<PIXEL_COUNT_OVERLAY> {
public:
    // virtual void render_overlay() {
    virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
        for (int i = this->BORDER; i < this->BORDER_END; i++) {
            if (i % this->BORDER != 0) {
                this->pixels_overlay[i] = CRGB::Black;
            } else {
                this->pixels_overlay[i] = CRGB::White;
            }
        }
        return this->pixels_overlay;
    };
};  // class ParameterGlobalOVERWRITE


template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterGlobalEFFECT: public ParameterGlobal<PIXEL_COUNT_OVERLAY> {
public:
    // virtual void render_overlay() {
    virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
        // for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
        //     this->pixels_overlay[i] = CRGB::Black;
        // }
        uint16_t temp_count = (this->BORDER_END - this->BORDER);
        temp_count -= (3*this->BORDER);
        temp_count = temp_count / 3;
        // static
        uint16_t start = this->BORDER;
        this->pixels_overlay(start, start + temp_count).fill_gradient(
            CHSV(0, 0, 0),
            CHSV(0, 0, 255)
        );
        // rainbo
        start += temp_count + this->BORDER;
        this->pixels_overlay(start, start + temp_count).fill_gradient(
            CHSV(0, 255, 255),
            CHSV(255, 255, 255),
            LONGEST_HUES
        );
        // plasma
        start += temp_count + this->BORDER;
        this->pixels_overlay(start, start + temp_count).fill_gradient(
            CHSV(150, 255, 255),
            CHSV(200, 255, 255)
        );
        return this->pixels_overlay;
    };
};  // class ParameterGlobalEFFECT














class MyAnimation {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // structs

    // typedef void (* tAnimationUpdatePixel) ();
    // typedef void (* tAnimationUpdatePosition) ();
    //
    // struct animation_t {
    //   const tAnimationUpdatePixel update_pixel;
    //   const tAnimationUpdatePosition update_position;
    //   const uint16_t mask;
    //   const uint16_t defaultv;
    // };

    // class EffectRainbow {
    //     uint8_t brightness = 255;
    //     uint32_t duration = 10 * 1000;  // ms
    //     float spread = 1.0;
    // }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    MyAnimation();
    ~MyAnimation();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // modes

    // parameter handling
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

    // CRGBArray<PIXEL_COUNT_OVERLAY> pixels_overlay;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    void setBrightness(uint8_t value);
    uint8_t getBrightness();

    // FastLED helper
    void show();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations
    bool animation_run = true;

    // float hue = 0.55;
    // float saturation = 1.0;
    // float brightness = 1.0;

    void overwrite_set(uint16_t start, uint16_t end);
    void overwrite_set_relative(int16_t value);
    uint16_t overwrite_start_get();
    uint16_t overwrite_end_get();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // effects

    // EffectStatic<PIXEL_COUNT> fx_static = EffectStatic<PIXEL_COUNT>();
    EffectBlack<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_black {};
    EffectStatic<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_static {};
    EffectRainbow<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_rainbow {};
    EffectPlasma<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> fx_plasma {};


    EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> * fx_last = &fx_static;
    EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> * fx_current = &fx_static;
    EffectBase<PIXEL_COUNT, PIXEL_COUNT_OVERLAY> * fx_next = &fx_rainbow;

    void select_next_effect();


    // using parameter_overlay_func_t = void (*)();
    // using parameter_overlay_func_t = std::function<void()>;
    // using parameter_overlay_func_t = CRGBArray<PIXEL_COUNT_OVERLAY> (*)();
    // using parameter_overlay_func_t = std::function<CRGBArray<PIXEL_COUNT_OVERLAY> ()>;
    // using parameter_overlay_func_t =
    //     void (ParameterBase<PIXEL_COUNT_OVERLAY>::*render_overlay)();
    // using parameter_overlay_func_t =
    //     std::function<void ParameterBase<PIXEL_COUNT_OVERLAY>::*render_overlay()>;
    // parameter_overlay_func_t parameter_overlay_func = nullptr;

    ParameterGlobalEFFECT<PIXEL_COUNT_OVERLAY>  param_effect {};
    ParameterGlobalBRIGHTNESS<PIXEL_COUNT_OVERLAY>  param_brightness {};
    ParameterGlobalOVERWRITE<PIXEL_COUNT_OVERLAY>  param_overwrite {};

    ParameterBase<PIXEL_COUNT_OVERLAY> * param_global_current = &param_effect;

    bool render_overlay_global = false;
    bool render_overlay_effect = false;

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

    // CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay_EFFECT() {
    //     for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //         pixels_overlay[i] = CRGB::Black;
    //     }
    //     return pixels_overlay;
    // };
    // CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay_BRIGHTNESS() {
    //     for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //         pixels_overlay[i] = CRGB::Black;
    //     }
    //     return pixels_overlay;
    // };
    // CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay_OVERWRITE() {
    //     for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //         pixels_overlay[i] = CRGB::Black;
    //     }
    //     return pixels_overlay;
    // };

    // // template <uint16_t PIXEL_COUNT_OVERLAY>
    // CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay_EFFECT();
    // CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay_BRIGHTNESS();
    // CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay_OVERWRITE();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

    uint8_t global_brightness = 255;
    const uint8_t leave_every_n_pixel_on = 15;

    uint8_t psu_off_pin = 7;
    uint8_t psu_on_pin = 9;
    uint8_t output_active_pin = 2;
    bool output_active = false;

    uint32_t fps_start = 0;
    uint32_t fps_duration = 10 * 1000;  // ms
    uint32_t fps_loopcount = 0;

    uint16_t overwrite_start = 0;
    uint16_t overwrite_end = 0;

};  // class MyAnimation

#endif  // MyAnimation_H_
