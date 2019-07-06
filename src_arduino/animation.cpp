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
        out.println(F("  output_off()"));
        output_off();

        // setup
        out.println(F("setup pixels:"));
        // FastLED.addLeds<APA102>(pixels, PIXEL_COUNT);
        // https://github.com/FastLED/FastLED/wiki/Frequently-Asked-Questions#5-with-apa102-leds-my-wiring-is-right-but-my-leds-are-flickering--or-my-leds-start-flickering-somewhere-down-the-line
        const uint8_t DATA_RATE_MHZ  = 6;
        out.print(F("  DATA_RATE "));
        out.print(DATA_RATE_MHZ);
        out.println(F(" MHZ"));
        FastLED.addLeds<APA102, MOSI, SCK, RGB, DATA_RATE_MHZ(DATA_RATE_MHZ)>(
            pixels, PIXEL_COUNT);
        // out.println(F("  setDither(0)"));
        // FastLED.setDither(0);
        // out.println(F("  setBrightness(50)."));
        // FastLED.setBrightness(50);
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
    if (output_active) {
        animation_run = false;
        fill_black();
        FastLED.show();
        // deactivate level-shifter output
        digitalWrite(output_active_pin, HIGH);
        // deactivate power supply
        digitalWrite(psu_off_pin, HIGH);
        delay(1);
        digitalWrite(psu_off_pin, LOW);
        output_active = false;
    }
}

void MyAnimation::output_on() {
    if (!output_active) {
        // activate power supply
        digitalWrite(psu_on_pin, HIGH);
        delay(1);
        digitalWrite(psu_on_pin, LOW);
        // activate level-shifter output
        digitalWrite(output_active_pin, LOW);
        animation_run = true;
        output_active = true;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation


void MyAnimation::effect_print(Print &out, EFFECT fx) {
    switch (fx) {
        case EFFECT::OFF: {
            out.print(F("OFF"));
        } break;
        case EFFECT::STATIC: {
            out.print(F("STATIC"));
        } break;
        case EFFECT::RAINBOW: {
            out.print(F("RAINBOW"));
        } break;
    }
}

void MyAnimation::effect_print_current(Print &out) {
    effect_print(out, effect_current);
}


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
        calculate_effect_position();
        switch (effect_current) {
            case EFFECT::OFF: {
                fill_black();
            } break;
            // case EFFECT::PIXEL_CHECKER: {
            //     effect__pixel_checker();
            // } break;
            case EFFECT::STATIC: {
                effect__static();
            } break;
            case EFFECT::RAINBOW: {
                effect__rainbow();
            } break;
        }
    }
    // write data to chips
    FastLED.show();
}

void MyAnimation::calculate_effect_position() {
    // effect_position = normalize_to_01(millis(), effect_start, effect_end);
    effect_end = effect_start + effect_duration;
    effect_position = (
        ((millis() - effect_start) * 1.0) / (effect_end - effect_start)
    );  // NOLINT(whitespace/parens)
    if (effect_position >  1.0) {
        effect_position = 0;
        effect_start = millis();
        if (animation_run) {
            // Serial.println("effect_position loop restart.");
        }
    }
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
// effects

void MyAnimation::fill_black() {
    fill_solid(pixels, PIXEL_COUNT, CHSV(0, 255, 0));
}

void MyAnimation::effect__pixel_checker() {
    // uint8_t step = map_range_01_to__uint8(
    //     effect_position, 0, PIXEL_COUNT);
    // tlc.set_pixel_all_16bit_value(0, 0, 0);
    // tlc.set_pixel_16bit_value(step, 0, 0, 500);
    // pixels[step]
}

void MyAnimation::effect__rainbow() {
    for (size_t pixel_i = 0; pixel_i < PIXEL_COUNT; pixel_i++) {
        float pixel_offset = pixel_i * 1.0 / PIXEL_COUNT;
        float offset = effect_position + (pixel_offset * rainbow_spread);
        uint8_t offset_int = offset * 255;
        uint8_t hue = offset_int;
        pixels[pixel_i] = CHSV(hue, 255, rainbow_brightness);
    }
    // fill_rainbow(pixels, PIXEL_COUNT, effect_position_int);
}

void MyAnimation::effect__static() {
    fill_solid(pixels, PIXEL_COUNT, color_hsv);
}





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu

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
    // tlc.set_pixel_16bit_value(index, value, value, value);
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
        effect__rainbow();
        // tlc.show();
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

void MyAnimation::menu__set_saturation(Print &out, char *command) {
    out.print(F("Set saturation "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value);
    saturation = value;
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
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
