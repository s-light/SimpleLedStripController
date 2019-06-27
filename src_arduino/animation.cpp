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


// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>
#include <slight_FaderLin.h>

#include <SPI.h>
#include <slight_TLC5957.h>

// include own headerfile
#include "./animation.h"

#include "./color.h"
#include "./mapping.h"

// namespace MCAnim = MyAnimation;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions


MyAnimation::MyAnimation() {
    ready = false;
    pmap_init();
}

MyAnimation::~MyAnimation() {
    end();
}

void MyAnimation::begin(Stream &out) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup
        tlc_init(out);
        gsclock_init(out);
        animation_init(out);

        // enable
        ready = true;
    }
}

void MyAnimation::end() {
    if (ready) {
        // nothing to do..
    }
}

void MyAnimation::update() {
    if (ready) {
        animation_update();
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu

void MyAnimation::menu__test_buffer(Print &out) {
    out.println(F("SetBuffer:"));
    out.println(F("--- old"));
    print_tlc_buffer(out);
    // tlc.set_pixel_all_16bit_value(
    //     255, 1, 1);
    //     // 0b11000110, 0b11000110, 0b11000110); // 198
    //     // 0x0055, 0x0055, 0x0055);
    //     // 0b01010101, 0b10101010, 0b10011001);
    //     // 0x0055, 0x00AA, 0x0099);
    //     // 85, 170, 153);
    // out.println(F("--- new"));
    // print_tlc_buffer(out);

    out.println(F("--- red"));
    tlc.set_pixel_all_16bit_value(1, 0, 0);
    print_tlc_buffer(out);
    tlc.show();
    delay(1000);
    out.println(F("--- green"));
    tlc.set_pixel_all_16bit_value(0, 1, 0);
    print_tlc_buffer(out);
    tlc.show();
    delay(1000);
    out.println(F("--- blue"));
    tlc.set_pixel_all_16bit_value(0, 0, 1);
    print_tlc_buffer(out);
    tlc.show();
    delay(1000);
    out.println(F("--- red full"));
    tlc.set_pixel_all_16bit_value(65535, 0, 0);
    print_tlc_buffer(out);
    tlc.show();
    delay(100);
    out.println(F("--- white"));
    tlc.set_pixel_all_16bit_value(1, 1, 1);
    print_tlc_buffer(out);
    tlc.show();

    out.println();
}

void MyAnimation::menu__set_pixel(Print &out, char *command) {
    out.print(F("Set pixel "));
    uint8_t command_offset = 1;
    uint8_t index = atoi(&command[command_offset]);
    // a better way than this would be to search for the ':'
    // i have used this a long time ago for MAC address format parsing
    // was something with 'tokenize' or similar..
    command_offset = 3;
    if (index > 9) {
        command_offset = command_offset +1;
    }
    out.print(index);
    out.print(F(" to "));
    uint16_t value = atoi(&command[command_offset]);
    out.print(value);
    tlc.set_pixel_16bit_value(index, value, value, value);
    out.println();
}

void MyAnimation::menu__time_meassurements(Print &out) {
    out.println(F("time_meassurements:"));

    uint32_t tm_start = 0;
    uint32_t tm_end = 0;
    uint32_t tm_duration = 0;
    uint32_t tm_loop_count = 10;

    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = millis();
        effect_Matrix2D();
        tlc.show();
        tm_end = millis();
        tm_duration += (tm_end - tm_start);
    }

    out.print(tm_duration / static_cast<float>(tm_loop_count));
    out.print(F("ms / call"));
    out.println();
}


void MyAnimation::menu__set_hue(Print &out, char *command) {
    out.print(F("Set hue "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value);
    hue = value;
    out.println();
}

void MyAnimation::menu__set_contrast(Print &out, char *command) {
    out.print(F("Set contrast "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value);
    contrast = value;
    out.println();
}

void MyAnimation::menu__set_brightness(Print &out, char *command) {
    out.print(F("Set brightness "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value);
    brightness = value;
    out.println();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TLC5957 / LED-Driver

void MyAnimation::tlc_init(Stream &out) {
    out.println(F("setup tlc:"));

    tlc.print_buffer_fc(out);

    out.println(F("  tlc.begin()"));
    tlc.begin();

    tlc.print_buffer_fc(out);

    out.println(F("  set spi_baudrate"));
    // 2MHz
    tlc.spi_baudrate = 2.0 * 1000 * 1000;
    // 0.001MHz = 1000kHz
    // tlc.spi_baudrate = 0.001 * 1000 * 1000;

    out.println(F("  set function configurations"));
    tlc.set_fc_CC_all(0x1FF, 0x1FF, 0x0FF);
    tlc.set_fc_BC_all(0x4);
    tlc.set_fc_ESPWM_all(true);
    tlc.update_fc();

    tlc.print_buffer_fc(out);

    out.print(F("  tlc.pixel_count: "));
    out.print(tlc.pixel_count);
    out.println();
    out.print(F("  tlc.chip_count: "));
    out.print(tlc.chip_count);
    out.println();
    out.print(F("  tlc.buffer_byte_count: "));
    out.print(tlc.buffer_byte_count);
    out.println();
    out.print(F("  tlc.spi_baudrate: "));
    out.print(tlc.spi_baudrate);
    out.print(F("Hz"));
    out.print(F("  = "));
    out.print(tlc.spi_baudrate / 1000.0, 4);
    out.print(F("kHz"));
    out.print(F("  = "));
    out.print(tlc.spi_baudrate / (1000.0 * 1000.0), 4);
    out.println(F("MHz"));

    out.print(F("  tlc.get_fc_ESPWM(): "));
    out.print(tlc.get_fc_ESPWM());
    out.println();
    out.println(F("  finished."));
}

void MyAnimation::print_tlc_buffer(Print &out) {
    uint8_t *buffer = tlc.buffer;
    // uint16_t *buffer16 = reinterpret_cast<uint16_t *>(tlc.buffer);
    char color_names[][6] = {
        "index",
        "blue ",
        "green",
        "red  ",
    };
    // print pixel index
    out.print(color_names[0]);
    out.print(F(" "));
    uint8_t index = 0;
    slight_DebugMenu::print_uint16_align_right(out, index);
    for (index = 1; index < tlc.pixel_count; index++) {
        out.print(F(", "));
        slight_DebugMenu::print_uint16_align_right(out, index);
    }
    out.println();
    // print data
    for (size_t color_i = 0; color_i < tlc.COLORS_PER_PIXEL; color_i++) {
        out.print(color_names[color_i + 1]);
        out.print(F(" "));
        index = color_i * tlc.BUFFER_BYTES_PER_COLOR;
        slight_DebugMenu::print_uint16_align_right(
            // out, buffer16[index]);
            out, ((buffer[index + 1] << 0) | (buffer[index + 0] << 8)) );
            // out, index);
        for (
            index += tlc.BUFFER_BYTES_PER_PIXEL;
            index < tlc.buffer_byte_count;
            index += tlc.BUFFER_BYTES_PER_PIXEL
        ) {
            out.print(F(", "));
            slight_DebugMenu::print_uint16_align_right(
                // out, buffer16[index]);
                out, ((buffer[index + 1] << 0) | (buffer[index + 0] << 8)) );
                // out, index);
        }
        out.println();
    }
    out.println(F("raw: "));
    const uint8_t line_length = 8*2*3;
    out.print(F(" p1 "));
    slight_DebugMenu::print_uint8_array(out, tlc.buffer, line_length);
    out.println();
    out.print(F(" p2 "));
    slight_DebugMenu::print_uint8_array(
        out, tlc.buffer + line_length, line_length);
    out.println();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// gsclock

void MyAnimation::gsclock_init(Print &out) {
    out.println(F("init gsclock:")); {
        out.println(F("  init gsclock timer."));
        setup_GenericClock7();
        // setup_D9_10MHz();
        setup_D2_10MHz();
        // out.println(F("  set gsclock to 3MHz."));
        // gsclock_set_frequency_MHz(3.0);
        out.println(F("  set gsclock to 30MHz."));
        gsclock_set_frequency_MHz(30.0);
    }
    out.println(F("  finished."));
}


void MyAnimation::setup_GenericClock7() {
    // Set up the generic clock (GCLK7)
    GCLK->GENCTRL[7].reg =
        // Divide clock source by divisor 1
        GCLK_GENCTRL_DIV(1) |
        // Set the duty cycle to 50/50 HIGH/LOW
        GCLK_GENCTRL_IDC |
        // Enable GCLK7
        GCLK_GENCTRL_GENEN |
        // // Select 48MHz DFLL clock source
        // GCLK_GENCTRL_SRC_DFLL;
        // // Select 100MHz DPLL clock source
        // GCLK_GENCTRL_SRC_DPLL1;
        // Select 120MHz DPLL clock source
        GCLK_GENCTRL_SRC_DPLL0;
    // Wait for synchronization
    while (GCLK->SYNCBUSY.bit.GENCTRL7) {}
}


void MyAnimation::setup_D9_10MHz() {
    // Activate timer TC3
    // check for correct mask at MCLK – Main Clock - 15.7 Register Summary
    // page178f
    // http://ww1.microchip.com/downloads/en/DeviceDoc/60001507C.pdf#page=178&zoom=page-width,-8,539
    // CLK_TC3_APB
    MCLK->APBBMASK.reg |= MCLK_APBBMASK_TC3;

    // for PCHCTRL numbers have a look at
    // Table 14-9. PCHCTRLm Mapping page168ff
    // http://ww1.microchip.com/downloads/en/DeviceDoc/60001507C.pdf#page=169&zoom=page-width,-8,696
    // GCLK->PCHCTRL[GCLK_TC3].reg =
    GCLK->PCHCTRL[26].reg =
        // Enable the peripheral channel
        GCLK_PCHCTRL_CHEN |
        // Connect generic clock 7
        GCLK_PCHCTRL_GEN_GCLK7;

    // Enable the peripheral multiplexer on pin D9
    PORT->Group[g_APinDescription[9].ulPort].
        PINCFG[g_APinDescription[9].ulPin].bit.PMUXEN = 1;

    // Set the D9 (PORT_PA19) peripheral multiplexer to
    // peripheral (odd port number) E(6): TC3, Channel 1
    // check if you need even or odd PMUX!!!
    // http://forum.arduino.cc/index.php?topic=589655.msg4064311#msg4064311
    PORT->Group[g_APinDescription[9].ulPort].
        PMUX[g_APinDescription[9].ulPin >> 1].reg |= PORT_PMUX_PMUXO(4);

    TC3->COUNT8.CTRLA.reg =
        // Set prescaler to 2
        // 120MHz/2 = 60MHz
        TC_CTRLA_PRESCALER_DIV2 |
        // Set the reset/reload to trigger on prescaler clock
        TC_CTRLA_PRESCSYNC_PRESC;

    // Set-up TC3 timer for
    // Match Frequency Generation (MFRQ)
    // the period time (T) is controlled by the CC0 register.
    // (instead of PER or MAX)
    // WO[0] toggles on each Update condition.
    TC3->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
    // Wait for synchronization
    // while (TC3->COUNT8.SYNCBUSY.bit.WAVE)

    // Set-up the CC (counter compare), channel 0 register
    // this sets the period
    //
    // (clockfreq / 2) / (CC0 + 1)  = outfreq  | * (CC0 + 1)
    // (clockfreq / 2) = outfreq * (CC0 + 1)   | / outfreq
    // (clockfreq / 2) / outfreq  = CC0 + 1    | -1
    // ((clockfreq / 2) / outfreq) -1  = CC0
    //
    // ((60 / 2) / 2) -1  = CC0
    //
    // MAX: (60MHz / 2) / (0 + 1)  = 30MHz
    // MIN: (60MHz / 2) / (255 + 1)  = 0,117MHz = 117kHz
    //
    //       60.0MHz
    //   0 = 30.0MHz
    //   1 = 15.0MHz
    //   2 = 10.0MHz
    //   3 =  7.5MHz
    //   4 =  6.0MHz
    //   5 =  5.0MHz
    //   9 =  3.0MHz
    //  14 =  2.0MHz
    //  29 =  1.0MHz
    //  59 =  0.5MHz
    //  74 =  0.4MHz
    //  99 =  0.3MHz
    // 149 =  0.2MHz
    // 255 =  0.11MHz
    // start with 10MHz
    TC3->COUNT8.CC[0].reg = 2;
    // Wait for synchronization
    while (TC3->COUNT8.SYNCBUSY.bit.CC1) {}

    // Enable timer TC3
    TC3->COUNT8.CTRLA.bit.ENABLE = 1;
    // Wait for synchronization
    while (TC3->COUNT8.SYNCBUSY.bit.ENABLE) {}
}

void MyAnimation::set_D9_period_reg(uint8_t period_reg) {
    TC3->COUNT8.CC[0].reg = period_reg;
    while (TC3->COUNT8.SYNCBUSY.bit.CC1) {}
}

uint8_t MyAnimation::get_D9_period_reg() {
    return TC3->COUNT8.CC[0].reg;
}

void MyAnimation::setup_D2_10MHz() {
    // Activate timer TC1
    // check for correct mask at MCLK – Main Clock - 15.7 Register Summary
    // page178f
    // http://ww1.microchip.com/downloads/en/DeviceDoc/60001507C.pdf#page=178&zoom=page-width,-8,539
    // CLK_TC1_APB
    MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC1;

    // for PCHCTRL numbers have a look at
    // Table 14-9. PCHCTRLm Mapping page168ff
    // http://ww1.microchip.com/downloads/en/DeviceDoc/60001507C.pdf#page=169&zoom=page-width,-8,696
    // GCLK->PCHCTRL[GCLK_TC1].reg =
    GCLK->PCHCTRL[9].reg =
        // Enable the peripheral channel
        GCLK_PCHCTRL_CHEN |
        // Connect generic clock 7
        GCLK_PCHCTRL_GEN_GCLK7;

    // Enable the peripheral multiplexer on pin D2
    PORT->Group[g_APinDescription[2].ulPort].
        PINCFG[g_APinDescription[2].ulPin].bit.PMUXEN = 1;

    // Set the D2 (PORT_PA07) peripheral multiplexer to
    // peripheral (odd port number) E(6): TC1, Channel 1
    // check if you need even or odd PMUX!!!
    // http://forum.arduino.cc/index.php?topic=589655.msg4064311#msg4064311
    PORT->Group[g_APinDescription[2].ulPort].
        PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXO(4);

    TC1->COUNT8.CTRLA.reg =
        // Set prescaler to 2
        // 120MHz/2 = 60MHz
        TC_CTRLA_PRESCALER_DIV2 |
        // Set the reset/reload to trigger on prescaler clock
        TC_CTRLA_PRESCSYNC_PRESC;

    // Set-up TC1 timer for
    // Match Frequency Generation (MFRQ)
    // the period time (T) is controlled by the CC0 register.
    // (instead of PER or MAX)
    // WO[0] toggles on each Update condition.
    TC1->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
    // Wait for synchronization
    // while (TC1->COUNT8.SYNCBUSY.bit.WAVE)

    // Set-up the CC (counter compare), channel 0 register
    // this sets the period
    //
    // (clockfreq / 2) / (CC0 + 1)  = outfreq  | * (CC0 + 1)
    // (clockfreq / 2) = outfreq * (CC0 + 1)   | / outfreq
    // (clockfreq / 2) / outfreq  = CC0 + 1    | -1
    // ((clockfreq / 2) / outfreq) -1  = CC0
    //
    // ((60 / 2) / 2) -1  = CC0
    //
    // MAX: (60MHz / 2) / (0 + 1)  = 30MHz
    // MIN: (60MHz / 2) / (255 + 1)  = 0,117MHz = 117kHz
    //
    //       60.0MHz
    //   0 = 30.0MHz
    //   1 = 15.0MHz
    //   2 = 10.0MHz
    //   3 =  7.5MHz
    //   4 =  6.0MHz
    //   5 =  5.0MHz
    //   9 =  3.0MHz
    //  14 =  2.0MHz
    //  29 =  1.0MHz
    //  59 =  0.5MHz
    //  74 =  0.4MHz
    //  99 =  0.3MHz
    // 149 =  0.2MHz
    // 255 =  0.11MHz
    // start with 10MHz
    TC1->COUNT8.CC[0].reg = 2;
    // Wait for synchronization
    while (TC1->COUNT8.SYNCBUSY.bit.CC1) {}

    // Enable timer TC1
    TC1->COUNT8.CTRLA.bit.ENABLE = 1;
    // Wait for synchronization
    while (TC1->COUNT8.SYNCBUSY.bit.ENABLE) {}
}

void MyAnimation::set_D2_period_reg(uint8_t period_reg) {
    TC1->COUNT8.CC[0].reg = period_reg;
    while (TC1->COUNT8.SYNCBUSY.bit.CC1) {}
}

uint8_t MyAnimation::get_D2_period_reg() {
    return TC1->COUNT8.CC[0].reg;
}



float MyAnimation::gsclock_set_frequency_MHz(float frequency_MHz) {
    const float frequency_MHz_min = 0.117;
    const float frequency_MHz_max = 30.0;
    if (frequency_MHz < frequency_MHz_min) {
        frequency_MHz = frequency_MHz_min;
    }
    if (frequency_MHz > frequency_MHz_max) {
        frequency_MHz = frequency_MHz_max;
    }
    float frequency_MHz_result = -1;
    // initialise to 1MHz
    uint8_t period_reg = 29;
    float req_raw = ((60 / 2) / frequency_MHz) -1;
    period_reg = static_cast<int>(req_raw);
    // set_D9_period_reg(period_reg);
    set_D2_period_reg(period_reg);
    // calculate actual used frequency
    frequency_MHz_result = (60.0 / 2) / (period_reg + 1);
    return frequency_MHz_result;
}

float MyAnimation::gsclock_get_frequency_MHz() {
    // uint8_t period_reg = get_D9_period_reg();
    uint8_t period_reg = get_D2_period_reg();
    float frequency_MHz_result = (60.0 / 2) / (period_reg + 1);
    return frequency_MHz_result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// pixel map

// Horizontal
// const uint8_t LEDBOARD_SINGLE
//         [LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT] = {
//     {15, 14, 13, 12},
//     {11, 10, 9, 8},
//     {7, 6, 5, 4},
//     {3, 2, 1, 0},
// }

// Vertical
const uint8_t MyAnimation::LEDBOARD_SINGLE
        [LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT] = {
    {3, 7, 11, 15},
    {2, 6, 10, 14},
    {1, 5, 9, 13},
    {0, 4, 8, 12},
};

const uint8_t MyAnimation::BOARDS_POSITIONS
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {2, 3},
    {1, 0},
    // ------
    // {1, 0},
    // {0},
};

const uint8_t MyAnimation::LEDBOARDS_ROTATED
        [MATRIX_ROW_COUNT][MATRIX_COL_COUNT] = {
    {15, 14, 13, 12,    3,  7, 11, 15},
    {11, 10,  9,  8,    2,  6, 10, 14},
    { 7,  6,  5,  4,    1,  5,  9, 13},
    { 3,  2,  1,  0,    0,  4,  8, 12},

    {12,  8,  4,  0,    0,  1,  2,  3},
    {13,  9,  5,  1,    4,  5,  6,  7},
    {14, 10,  6,  2,    8,  9, 10, 11},
    {15, 11,  7,  3,   12, 13, 14, 15},
};

void MyAnimation::pmap_init() {
    // """Prepare Static Map."""
    for (size_t row_index = 0; row_index < MATRIX_ROW_COUNT; row_index++) {
        for (size_t col_index = 0; col_index < MATRIX_COL_COUNT; col_index++) {
            pmap[row_index][col_index] =
                // mymap_LEDBoard_4x4_HD(
                mymap_LEDBoard_4x4_HD_CrystalLightGuide(
                    col_index, row_index);
        }
    }
}

uint8_t MyAnimation::mymap_LEDBoard_4x4_HD(
    uint8_t col, uint8_t row
) {
    // """Map row and col to pixel_index."""
    // get Board position
    uint8_t board_col = col / LEDBOARD_COL_COUNT;
    uint8_t board_row = row / LEDBOARD_ROW_COUNT;
    uint8_t board_sub_col = col % LEDBOARD_COL_COUNT;
    uint8_t board_sub_row = row % LEDBOARD_ROW_COUNT;

    uint8_t board_offset = BOARDS_POSITIONS[board_row][board_col];
    uint8_t pixel_offset = LEDBOARD_SINGLE[board_sub_row][board_sub_col];

    uint8_t pixel_index = (pixel_offset * BOARDS_COUNT) + board_offset;

    return pixel_index;
}

uint8_t MyAnimation::mymap_LEDBoard_4x4_HD_CrystalLightGuide(
    uint8_t col, uint8_t row
) {
    // """Map row and col to pixel_index."""
    // get Board position
    uint8_t board_col = col / LEDBOARD_COL_COUNT;
    uint8_t board_row = row / LEDBOARD_ROW_COUNT;
    // uint8_t board_sub_col = col % LEDBOARD_COL_COUNT;
    // uint8_t board_sub_row = row % LEDBOARD_ROW_COUNT;

    uint8_t board_offset = BOARDS_POSITIONS[board_row][board_col];
    // pixel_offset = LEDBOARD_SINGLE[board_sub_row][board_sub_col];

    uint8_t pixel_offset = LEDBOARDS_ROTATED[row][col];
    // print("{:>3} {:>3}: {:>3}".format(col, row, pixel_offset));
    uint8_t pixel_index = (pixel_offset * BOARDS_COUNT) + board_offset;

    return pixel_index;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation

void MyAnimation::animation_init(Stream &out) {
    out.println(F("init animation:")); {
        out.print(F("  effect_duration: "));
        out.print(effect_duration);
        out.println(F("ms"));

        // out.println(F("  Set all Pixel to 21845."));
        // tlc.set_pixel_all_16bit_value(21845, 21845, 21845);
        out.println(F("  Set all Pixel to red=blue=100."));
        tlc.set_pixel_all_16bit_value(100, 0, 100);
        tlc.show();

        effect_start = millis();
        effect_end = millis() + effect_duration;
    }
    out.println(F("  finished."));
}

void MyAnimation::animation_update() {
    calculate_effect_position();
    if (animation_run) {
        // effect__pixel_checker();
        // effect__line();
        // effect__rainbow();

        effect_Matrix2D();

        // write data to chips
        tlc.show();
    }
}

void MyAnimation::calculate_effect_position() {
    effect_position = normalize_to_01(millis(), effect_start, effect_end);
    effect_loopcount++;
    if (effect_position >  1.0) {
        effect_position = 0;
        float duration_seconds = (millis() - effect_start) / 1000.0;
        float fps = effect_loopcount / duration_seconds;
        effect_loopcount = 0;
        effect_start = millis();
        effect_end = millis() + effect_duration;
        if (animation_run) {
            Serial.print("effect_position loop restart. (");
            Serial.print(fps);
            Serial.print("FPS)");
            Serial.println();
        }
    }
}



void MyAnimation::effect__pixel_checker() {
    uint8_t step = map_range_01_to__uint8(
        effect_position, 0, MATRIX_PIXEL_COUNT);
    tlc.set_pixel_all_16bit_value(0, 0, 0);
    tlc.set_pixel_16bit_value(step, 0, 0, 500);
}

void MyAnimation::effect__line() {
    uint8_t step = map_range_01_to__uint8(effect_position, 0, MATRIX_COL_COUNT);
    tlc.set_pixel_all_16bit_value(0, 0, 0);
    for (size_t row_index = 0; row_index < MATRIX_ROW_COUNT; row_index++) {
        tlc.set_pixel_16bit_value(pmap[row_index][step], 0, 0, 500);
    }
}

void MyAnimation::effect__rainbow() {
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // full rainbow
            CHSV color_hsv = CHSV(effect_position, 1.0, brightness);
            CRGB color_rgb = hsv2rgb(color_hsv);
            tlc.set_pixel_float_value(
                pmap[row_i][col_i],
                color_rgb.r, color_rgb.g, color_rgb.b);
            // tlc.set_pixel_16bit_value(
            //     pmap[row_i][col_i],
            //     0, col_i * step * 10 , row_i * 100);
        }
    }
}



CHSV MyAnimation::effect__plasma(
    float col, float row, float offset
) {
    // calculate plasma
    // mostly inspired by
    // https://www.bidouille.org/prog/plasma
    // moving rings
    float cx = col + 0.5 * sin(offset / 5);
    float cy = row + 0.5 * cos(offset / 3);
    float xy_value = sin(
        sqrt(100 * (cx*cx + cy*cy) + 1)
        + offset
    );
    // mapping
    float pixel_hue = map_range(
        xy_value,
        -1.0, 1.0,
        // self._hue_min, self._hue_max
        // 0.0, 0.08
        hue - 0.05, hue + 0.05
    );
    float pixel_saturation = map_range(
        xy_value,
        -1.0, 1.0,
        1.0, 1.0
    );
    float pixel_value = map_range(
        xy_value,
        1.0, -1.0,
        // self._contrast_min, self._contrast_max
        // -0.005, 1.0
        1.0 - contrast, 1.0
    );
    // map to color
    CHSV pixel_hsv = CHSV(pixel_hue, pixel_saturation, pixel_value);
    return pixel_hsv;
}

CHSV MyAnimation::effect__sparkle(
    float col, float row, float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 1.0, 0.0);
    return pixel_hsv;
}


CHSV MyAnimation::effect_Matrix2D_get_pixel(
    float col, float row, float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 0.0, 1.0);

    // plasma
    CHSV plasma = effect__plasma(col, row, offset);
    pixel_hsv = plasma;

    // sparkle
    CHSV sparkle = effect__sparkle(
        col, row, offset);
    // not used currently..

    // TODO(s-light): develop 'layer' / 'multiplyer' system...

    return pixel_hsv;
}

void MyAnimation::effect_Matrix2D() {
    float offset = map_range_01_to(effect_position, 0.0, (PI * 30));
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        // normalize row
        float row = map_range(
            row_i,
            0, MATRIX_ROW_COUNT-1,
            -0.5, 0.5);
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // normalize col
            float col = map_range(
                col_i,
                0, MATRIX_COL_COUNT-1,
                -0.5, 0.5);

            // ------------------------------------------
            CHSV pixel_hsv = effect_Matrix2D_get_pixel(col, row, offset);

            // ------------------------------------------
            // final conversions
            // global brightness
            pixel_hsv.value *= brightness;
            // convert to rgb
            CRGB pixel_rgb = hsv2rgb(pixel_hsv);
            // gamma & global brightness
            // fancyled.gamma_adjust(brightness=self.brightness);
            tlc.set_pixel_float_value(
                pmap[row_i][col_i],
                pixel_rgb.r, pixel_rgb.g, pixel_rgb.b);
        }
    }
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
