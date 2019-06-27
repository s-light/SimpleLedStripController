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

#include <SPI.h>
#include <slight_TLC5957.h>

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
    // pixel map
    // LEDBoard_4x4_16bit mapping

    static const uint16_t LEDBOARD_COL_COUNT = 4;
    static const uint16_t LEDBOARD_ROW_COUNT = 4;
    static const uint16_t LEDBOARD_PIXEL_COUNT = (
        LEDBOARD_ROW_COUNT * LEDBOARD_COL_COUNT);

    // Layouts for LEDBoard_4x4_HD
    // https://github.com/s-light/LEDBoard_4x4_HD

    static const uint8_t LEDBOARD_SINGLE
        [LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT];


    static const uint8_t BOARDS_COL_COUNT = 2;
    static const uint8_t BOARDS_ROW_COUNT = 2;
    static const uint8_t BOARDS_COUNT = BOARDS_COL_COUNT * BOARDS_ROW_COUNT;
    static const uint8_t BOARDS_POSITIONS
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT];

    static const uint8_t MATRIX_COL_COUNT = LEDBOARD_COL_COUNT * BOARDS_COL_COUNT;
    static const uint8_t MATRIX_ROW_COUNT = LEDBOARD_ROW_COUNT * BOARDS_ROW_COUNT;
    static const uint8_t MATRIX_PIXEL_COUNT = MATRIX_COL_COUNT * MATRIX_ROW_COUNT;

    static const uint8_t LEDBOARDS_ROTATED
        [MATRIX_ROW_COUNT][MATRIX_COL_COUNT];

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TLC5957

    // possible options and defaults:
    // slight_TLC5957(
    //     uint8_t chip_count,
    //     uint8_t lat_pin = 7,
    //     uint8_t gclk_pin = 9,
    //     uint8_t sclk_pin = SCK,
    //     uint8_t sout_pin = MOSI,
    //     uint8_t sin_pin = MISO
    // );
    // use default pins
    slight_TLC5957 tlc = slight_TLC5957(MATRIX_PIXEL_COUNT);


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

    uint8_t pmap[MATRIX_ROW_COUNT][MATRIX_COL_COUNT];

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

    // others
    void pmap_init();

    uint8_t mymap_LEDBoard_4x4_HD(uint8_t col, uint8_t row);
    uint8_t mymap_LEDBoard_4x4_HD_CrystalLightGuide(uint8_t col, uint8_t row);

    void tlc_init(Stream &out);

    // grayscale clock things
    void gsclock_init(Print &out);

    void setup_GenericClock7();

    void setup_D9_10MHz();
    void set_D9_period_reg(uint8_t period_reg);
    uint8_t get_D9_period_reg();

    void setup_D2_10MHz();
    void set_D2_period_reg(uint8_t period_reg);
    uint8_t get_D2_period_reg();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

    uint32_t effect_start = 0;
    uint32_t effect_end = 0;
    uint32_t effect_loopcount = 0;
    float effect_position = 0.0;

};  // class MyAnimation

#endif  // MyAnimation_H_
