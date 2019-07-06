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

#include <slight_DebugMenu.h>

// #define FASTLED_USE_GLOBAL_BRIGHTNESS 1
// i can't test this  - it needs the min and max functions defined by Arduino.h
// but the slight_* libs
// that are using std:function are undef them so the std:: namespace works..

#include <FastLED.h>

// #include "./color.h"
// ^ color.h is my own minimal FastLED compatible thing...

// #include "./mapping.h"


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
    // attributes

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // pixels

    static const uint16_t PIXEL_COUNT = 144*3;
    CRGB pixels[PIXEL_COUNT];

    const CHSV warm_white = CHSV(142, 100, 240);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // menu & helper
    void menu__set_pixel(Print &out, char *command);
    void menu__test_buffer(Print &out);
    void menu__time_meassurements(Print &out);
    void print_tlc_buffer(Print &out);

    void menu__set_hue(Print &out, char *command);
    void menu__set_saturation(Print &out, char *command);
    void menu__set_brightness(Print &out, char *command);


    void fill_black();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    enum class EFFECT {
        OFF,
        STATIC,
        RAINBOW,
    };
    EFFECT effect_current = EFFECT::RAINBOW;
    void effect_print(Print &out, EFFECT fx);
    void effect_print_current(Print &out);

    bool animation_run = true;

    uint32_t effect_duration = 30 * 1000; //ms

    float hue = 0.55;
    float saturation = 1.0;
    float brightness = 0.1;

    float rainbow_spread = 0.5;
    uint8_t rainbow_brightness = 50;
    CHSV color_hsv = warm_white;
    // EffectRainbow rainbow_settings;

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
    void calculate_effect_position();

    void effect__pixel_checker();
    void effect__rainbow();
    void effect__static();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

    uint8_t psu_on_pin = 7;
    uint8_t psu_off_pin = 9;
    uint8_t output_active_pin = 2;
    bool output_active = false;

    uint32_t effect_start = 0;
    uint32_t effect_end = 0;
    float effect_position = 0.0;

    uint32_t fps_start = 0;
    uint32_t fps_duration = 10 * 1000;  // ms
    uint32_t fps_loopcount = 0;

};  // class MyAnimation

#endif  // MyAnimation_H_
