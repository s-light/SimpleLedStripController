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

// #include <FastLED.h>

#include "./color.h"


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

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // pixels

    static const uint16_t PIXEL_COUNT = 144*3:
    // CHSVArray<PIXEL_COUNT> leds;
    // CRGB pixels[PIXEL_COUNT];

    // const CHSV warm_white = CHSV(142, 100, 240);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    MyAnimation();
    ~MyAnimation();

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
    void menu__set_contrast(Print &out, char *command);
    void menu__set_brightness(Print &out, char *command);

    float gsclock_set_frequency_MHz(float frequency_MHz);
    float gsclock_get_frequency_MHz();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    bool animation_run = true;

    uint16_t effect_duration = 30 * 1000; //ms
    float hue = 0.55;
    float contrast = 1.4;
    float brightness = 0.3;

    // const float PI = 3.141592;
    // is already defined by arduino or some other defaults...

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    // animation
    void animation_init(Stream &out);
    void animation_update();
    void calculate_effect_position();

    void effect__pixel_checker();
    void effect__line();
    void effect__rainbow();

    void effect_Matrix2D();
    CHSV effect_Matrix2D_get_pixel(float col, float row, float offset);
    CHSV effect__plasma(float col, float row, float offset);
    CHSV effect__sparkle(float col, float row, float offset);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

    uint32_t effect_start = 0;
    uint32_t effect_end = 0;
    uint32_t effect_loopcount = 0;
    float effect_position = 0.0;

};  // class MyAnimation

#endif  // MyAnimation_H_
