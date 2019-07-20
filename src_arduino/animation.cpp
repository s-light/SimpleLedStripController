// NOLINT(legal/copyright)

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

//
// // include Core Arduino functionality
// #include <Arduino.h>
//
// #include <slight_DebugMenu.h>
// #include <slight_FaderLin.h>

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./animation.h"

// #include "./color.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions


MyAnimation::MyAnimation() {
    ready = false;
}

MyAnimation::~MyAnimation() {
    end();
}

void MyAnimation::begin(Stream &out) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        out.println(F("setup control pins:"));
        out.println(F("  output_active_pin"));
        pinMode(output_active_pin, OUTPUT);
        out.println(F("  psu_off_pin"));
        pinMode(psu_off_pin, OUTPUT);
        out.println(F("  psu_on_pin"));
        pinMode(psu_on_pin, OUTPUT);
        out.println(F("  output_on()"));
        output_on();

        // setup
        out.println(F("setup pixels:"));
        // FastLED.addLeds<APA102>(pixels, PIXEL_COUNT);
        // https://github.com/FastLED/FastLED/wiki/Frequently-Asked-Questions#5-with-apa102-leds-my-wiring-is-right-but-my-leds-are-flickering--or-my-leds-start-flickering-somewhere-down-the-line
        const uint8_t DATA_RATE_MHZ  = 2;
        out.print(F("  DATA_RATE "));
        out.print(DATA_RATE_MHZ);
        out.println(F(" MHZ"));
        FastLED.addLeds<APA102, MOSI, SCK, RGB,
            DATA_RATE_MHZ(DATA_RATE_MHZ)>(pixels, PIXEL_COUNT);
        // out.println(F("  setDither(0)"));
        // FastLED.setDither(0);
        out.println(F("  setBrightness(2)."));
        FastLED.setBrightness(2);
        out.println(F("  finished."));

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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// output

bool MyAnimation::output_get() {
    return output_active;
}

void MyAnimation::output_toggle() {
    if (output_active) {
        output_off();
    } else {
        output_on();
    }
}

void MyAnimation::output_off() {
    fx_last = fx_current;
    fx_current = &fx_black;
    update();
    animation_run = false;
    // deactivate level-shifter output
    digitalWrite(output_active_pin, HIGH);
    // deactivate power supply
    digitalWrite(psu_off_pin, HIGH);
    delay(1);
    digitalWrite(psu_off_pin, LOW);
    output_active = false;
}

void MyAnimation::output_on() {
    if (!output_active) {
        // activate power supply
        digitalWrite(psu_on_pin, HIGH);
        delay(1);
        digitalWrite(psu_on_pin, LOW);
        // activate level-shifter output
        digitalWrite(output_active_pin, LOW);
        // reactivate fx that was active before black
        fx_current = fx_last;
        animation_run = true;
        output_active = true;
    }
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
        // tlc.set_pixel_all_16bit_value(100, 0, 100);
        // tlc.show();

        effect_start = millis();
        effect_end = millis() + effect_duration;
    }
    out.println(F("  finished."));
}

void MyAnimation::animation_update() {
    fps_update();
    if (animation_run) {
        fx_current->calculate_effect_position();
        fx_current->update();
        // for now just copy pixel data from effect to master
        pixels = fx_current->pixels;
        overwrite_black();
    }
    // write data to chips
    FastLED.show();
}

void MyAnimation::fps_update() {
    fps_loopcount++;
    if ((millis() - fps_start) >=  fps_duration) {
        float duration_seconds = (millis() - fps_start) / 1000.0;
        float fps = fps_loopcount / duration_seconds;
        fps_loopcount = 0;
        fps_start = millis();
        if (animation_run) {
            Serial.print("FPS: ");
            Serial.print(fps);
            // Serial.print("");
            Serial.println();
        }
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FastLED helper

void MyAnimation::show() {
    // write data to chips
    FastLED.show();
}

void MyAnimation::setBrightness(uint8_t value) {
    global_brightness = value;
    if (global_brightness > 0) {
        FastLED.setBrightness(value);
    } else {
        FastLED.setBrightness(1);
    }
}

uint8_t MyAnimation::getBrightness() {
    return global_brightness;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// overwrite

void MyAnimation::overwrite_black() {
    if (overwrite_end - overwrite_start > 0) {
        pixels(overwrite_start, overwrite_end) = CRGB::Black;
    }
    if (global_brightness == 0) {
        // leave only every nth pixel on
        for (int i = 0; i < PIXEL_COUNT; i++) {
            if (i % leave_every_n_pixel_on != 0) {
                pixels[i] = CRGB::Black;
            }
        }
    }
}

void MyAnimation::overwrite_set(uint16_t start, uint16_t end) {
    overwrite_start = limit(start, PIXEL_COUNT);
    overwrite_end = clamp(end, overwrite_start, PIXEL_COUNT);
}

void MyAnimation::overwrite_set_relative(int16_t value) {
    int16_t start_new = overwrite_start + value;
    int16_t end_new = overwrite_end + value;

    if (overwrite_start == 0) {
        // Serial.printf(" <E%+3d", end_new);
        if (end_new >= 0 && end_new <= PIXEL_COUNT-1) {
            overwrite_end = end_new;
        } else {
            if (end_new > 0) {
                // Serial.printf(" |+ %+4d", end_new - (PIXEL_COUNT-1));
                overwrite_start = end_new - (PIXEL_COUNT-1);
                overwrite_end = PIXEL_COUNT-1;
            } else {
                // we need + (because end_new is already negative!!)
                // Serial.printf(" |- %+4d", (PIXEL_COUNT-1) + end_new);
                overwrite_start = (PIXEL_COUNT-1) + end_new;
                overwrite_end = PIXEL_COUNT-1;
            }
        }
        // Serial.print("> ");
    } else if (overwrite_end == PIXEL_COUNT-1) {
        // Serial.printf(" <S%+3d", start_new);
        if (start_new >= 0 && start_new <= PIXEL_COUNT-1) {
            overwrite_start = start_new;
        } else {
            if (start_new > 0) {
                // Serial.printf(" |+ %+4d", start_new - (PIXEL_COUNT-1));
                overwrite_start = 0;
                overwrite_end = start_new - (PIXEL_COUNT-1);
            } else {
                // Serial.printf(" |- %+4d", start_new + (PIXEL_COUNT-1));
                overwrite_start = 0;
                overwrite_end = start_new + (PIXEL_COUNT-1);
            }
        }
        // Serial.print("> ");
        // overwrite_start = limit(
            //     static_cast<uint16_t>(start_new), PIXEL_COUNT);
    } else {
        // Serial.println("UPS don't know what to do!!!");
        Serial.printf(
            "\n\r"
            "UPS don't know what to do!!! "
            "\n\r"
            "value: %+4d\n\r"
            "PIXEL_COUNT: %+4d\n\r"
            "",
            value,
            PIXEL_COUNT);
        Serial.printf(
            "start: %+4d\n\r"
            "end: %+4d\n\r"
            "",
            overwrite_start,
            overwrite_end);
        Serial.printf(
            "start_new: %+4d\n\r"
            "end_new: %+4d\n\r"
            "\n\r",
            start_new,
            end_new);
    }
}

uint16_t MyAnimation::overwrite_start_get() {
    return overwrite_start;
}

uint16_t MyAnimation::overwrite_end_get() {
    return overwrite_end;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// effects

void MyAnimation::select_next_effect() {
    if (fx_current == &fx_black) {
        fx_current = &fx_static;
    } else if (fx_current == &fx_static) {
        fx_current = &fx_rainbow;
    } else if (fx_current == &fx_rainbow) {
        fx_current = &fx_plasma;
    } else if (fx_current == &fx_plasma) {
        fx_current = &fx_static;
    } else {
        fx_current = &fx_static;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// xx


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
