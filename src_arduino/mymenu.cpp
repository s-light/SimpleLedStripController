// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    sensor input and pre-conditioning
    """

    libraries used:
        ~ Adafruit_Sensor
        ~ Adafruit_TSL2591
            written by Adafruit
        ~ slight_ButtonInput
        ~ slight_DebugMenu
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


// cpplint
// disable linting for next line:
// NOLINTNEXTLINE(readability/nolint)
// https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./mymenu.h"

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

MyMenu::MyMenu(
    MyAnimation &animation,
    SettingsUI &settingsui,
    const sketchinfo_func sketchinfo_print
):
    myDebugMenu{slight_DebugMenu(Serial, Serial, 20)},
    animation(animation),
    settingsui(settingsui),
    //      ^ '(' needed. its a kind of bug in gcc..
    // https://stackoverflow.com/questions/10509603/why-cant-i-initialize-a-reference-in-an-initializer-list-with-uniform-initializ
    ready{false},
    sketchinfo_print{sketchinfo_print}
// NOLINTNEXTLINE(whitespace/braces)
{
    // nothing to do right now..
}

MyMenu::~MyMenu() {
    end();
}

void MyMenu::begin(Stream &out) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup
        out.println("MyMenu begin:");

        // LiveSign
        pinMode(infoled_pin, OUTPUT);
        digitalWrite(infoled_pin, LOW);
        // as of arduino 1.0.1 you can use INPUT_PULLUP

        // out.println("  mydebugmenu.begin");
        myDebugMenu.set_callback(std::bind(
            &MyMenu::handleMenu_Main, this, std::placeholders::_1));
        myDebugMenu.begin(true);

        out.println("done:");
        // enable
        ready = true;
    }
}


void MyMenu::end() {
    if (ready) {
        ready = false;
    }
}

void MyMenu::update() {
    if (ready) {
        myDebugMenu.update();
        debugOut_update();
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// debug things

void MyMenu::debugOut_update() {
    uint32_t duration_temp = millis() - debugOut_LastAction;
    if (duration_temp > debugOut_interval) {
        debugOut_LastAction = millis();

        if ( debugOut_Serial_Enabled ) {
            Serial.print(millis());
            Serial.print(F("ms;"));
            Serial.println();
        }

        if ( debugOut_LED_Enabled ) {
            infoled_state = !infoled_state;
            if (infoled_state) {
                digitalWrite(infoled_pin, HIGH);
            } else {
                digitalWrite(infoled_pin, LOW);
            }
        }
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu

void MyMenu::menu__test_xxx(Print &out) {
    out.println(F("TODO"));
    out.println();
}

void MyMenu::menu__set_yyy(Print &out, char *command) {
    out.println(F("Set yyy "));
    out.println(F("TODO"));

    out.print(F("Set yyy "));
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



void MyMenu::menu__set_pixel(Print &out, char *command) {
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
    // animation.pixels.set_pixel_16bit_value(index, value, value, value);
    out.println();
}

void MyMenu::menu__set_board_dotstar(Print &out, char *command) {
    out.print(F("Set board_dotstar: "));
    char *command_offset = command;
    uint8_t red = atoi(command_offset);
    command_offset = strchr(command_offset, ',');
    uint8_t green = atoi(command_offset+1);
    command_offset = strchr(command_offset+1, ',');
    uint8_t blue = atoi(command_offset+1);
    // out.print(F(" red:"));
    // out.print(red);
    // out.print(F(" green:"));
    // out.print(green);
    // out.print(F(" blue:"));
    // out.print(blue);
    out.printf("red %03d, green %03d, blue %03d", red, green, blue);
    settingsui.board_dotstar.setPixelColor(0, red, green, blue);
    settingsui.board_dotstar.show();
    out.println();
}

void MyMenu::menu__time_meassurements(Print &out) {
    out.println(F("time_meassurements:"));

    uint32_t tm_start = 0;
    uint32_t tm_end = 0;
    uint32_t tm_duration = 0;
    uint32_t tm_loop_count = 10;

    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = millis();
        animation.effect__rainbow();
        animation.show();
        tm_end = millis();
        tm_duration += (tm_end - tm_start);
    }

    out.print(tm_duration / static_cast<float>(tm_loop_count));
    out.print(F("ms / call"));
    out.println();
}


void MyMenu::menu__set_overwrite_black(Print &out, char *command) {
    out.print(F("set overwrite_black: "));
    char *command_offset = command +1;
    uint16_t start = atoi(command_offset);
    command_offset = strchr(command_offset, ',');
    uint16_t end = atoi(command_offset+1);
    out.printf("(%3d, %3d) >", start, end);
    animation.overwrite_set(start, end);
    out.printf(
        " (%3d, %3d)",
        animation.overwrite_start_get(),
        animation.overwrite_end_get());
    out.println();
    out.println();
}

void MyMenu::menu__set_overwrite_black_relative(Print &out, char *command) {
    out.print(F("set overwrite_black_relative: "));
    char *command_offset = command +1;
    int16_t value = atoi(command_offset);
    out.print(value);
    // out.println();
    out.printf(
        " (%3d, %3d) --> ",
        animation.overwrite_start_get(),
        animation.overwrite_end_get());
    animation.overwrite_set_relative(value);
    out.printf(
        " (%3d, %3d)",
        animation.overwrite_start_get(),
        animation.overwrite_end_get());
    out.println();
    out.println();
}


//
// void MyMenu::menu__set_hue(Print &out, char *command) {
//     out.print(F("Set hue "));
//     uint8_t command_offset = 1;
//     float value = atof(&command[command_offset]);
//     out.print(value);
//     animation.hue = value;
//     out.println();
// }
//
// void MyMenu::menu__set_saturation(Print &out, char *command) {
//     out.print(F("Set saturation "));
//     uint8_t command_offset = 1;
//     float value = atof(&command[command_offset]);
//     out.print(value);
//     animation.saturation = value;
//     out.println();
// }
//
// void MyMenu::menu__set_brightness(Print &out, char *command) {
//     out.print(F("Set brightness "));
//     uint8_t command_offset = 1;
//     float value = atof(&command[command_offset]);
//     out.print(value);
//     animation.brightness = value;
//     out.println();
// }






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Menu System


void MyMenu::menu__print_help(Print &out) {
    // help
    out.println(F("__________________________________________________"));
    out.println();
    out.println(F("Help for Commands:"));
    out.println();
    out.println(F("\t '?': this help"));
    out.println(F("\t '!': sketch info"));
    out.println(F("\t 'y': toggle DebugOut livesign print"));
    out.println(F("\t 'Y': toggle DebugOut livesign LED"));
    out.println(F("\t 'x': tests"));
    out.println();
    out.println(F("\t 'Q': system power off 'Q'"));
    out.print(F("\t 'o': toggle animation.output 'o' ("));
    out.print(animation.output_get());
    out.println(F(")"));
    out.print(F("\t 'r': toggle animation_run 'r' ("));
    out.print(animation.animation_run);
    out.println(F(")"));
    out.print(F("\t 'd': set effect_duration 'd1000' ("));
    out.print(animation.effect_duration);
    out.println(F("ms)"));
    // out.print(F("\t 'h': set hue 'h1.0' ("));
    // out.print(animation.hue, 4);
    // out.println(F(")"));
    // out.print(F("\t 'c': set saturation 'c1.0' ("));
    // out.print(animation.saturation, 4);
    // out.println(F(")"));
    // out.print(F("\t 'b': set brightness 'b1.0' ("));
    // out.print(animation.brightness, 4);
    // out.println(F(")"));
    out.print(F("\t 'b': set brightness 'b255' ("));
    out.print(animation.getBrightness(), 4);
    out.println(F(")"));
    out.println();
    out.print(F("\t 'O': set overwrite 'O432,432'"));
    out.printf(
        " (%3d, %3d)",
        animation.overwrite_start_get(),
        animation.overwrite_end_get());
    out.println();
    out.println(F("\t 'q': set overwrite relative 'q-5'"));
    // // out.println(F("\t 'f': test fc 'f'"));
    // out.println(F("\t 'u': tlc.show() 'u'"));
    // out.print(F("\t 'e': toggle ESPWM 'e' ("));
    // out.print(animation.tlc.get_fc_ESPWM());
    // out.println(F(")"));
    // out.print(F("\t 'g': set grayscale frequency in MHz 'g1.0' ("));
    // out.print(animation.gsclock_get_frequency_MHz(), 4);
    // out.println(F("MHz)"));
    // out.print(F("\t 's': set spi baudrate in MHz 's1.0' ("));
    // out.print(animation.tlc.spi_baudrate / (1000.0 * 1000), 4);
    // out.println(F("MHz)"));
    // out.println(F("\t 't': set buffer to test values 't'"));
    out.println(F("\t 'T': time_meassurements 'T'"));
    out.println(F("\t 'p': set pixel 'p0:65535'"));
    out.println(F("\t 'P': set board_dotstar 'P255,255,255'"));
    // out.println(F("\t 'P': set all pixel 'P65535'"));
    out.println(F("\t 'z': set all pixel to black 'z'"));
    // out.println(F("\t 'Z': set all pixel to 21845 'z'"));
    // out.println(F("\t 'B': print Buffer 'B'"));
    // out.println(F("\t 'F': print buffer_fc 'F'"));
    out.println();
    out.println(F("__________________________________________________"));
}

void MyMenu::handleMenu_Main(slight_DebugMenu *instance) {
    Print &out = instance->get_stream_out_ref();
    char *command = instance->get_command_current_pointer();
    // out.print("command: '");
    // out.print(command);
    // out.println("'");
    switch (command[0]) {
        // case 'h':
        // case 'H':
        case '?': {
            menu__print_help(out);
        } break;
        case '!': {
            sketchinfo_print(out);
        } break;
        case 'y': {
            out.println(F("\t toggle DebugOut livesign Serial:"));
            debugOut_Serial_Enabled = !debugOut_Serial_Enabled;
            out.print(F("\t debugOut_Serial_Enabled:"));
            out.println(debugOut_Serial_Enabled);
        } break;
        case 'Y': {
            out.println(F("\t toggle DebugOut livesign LED:"));
            debugOut_LED_Enabled = !debugOut_LED_Enabled;
            out.print(F("\t debugOut_LED_Enabled:"));
            out.println(debugOut_LED_Enabled);
        } break;
        case 'x': {
            // get state
            out.println(F("__________"));
            out.println(F("Tests:"));
            out.println(F("nothing to do."));
            out.println(F("__________"));
        } break;
        // ---------------------
        case 'Q': {
            out.println(F("system power off"));
            out.println(F(
                " --> this currently detaches the USB-Serial connection."));
            out.println(F(
                "     but it does not reattach correctly. "
                "so after the sleep this debug menu "
                "is not available anymore."));
            out.println(F(
                "     you can reconnect only by hitting the reset button."));
            out.flush();
            settingsui.system_power_off();
        } break;
        case 'o': {
            out.println(F("toggle animation.output"));
            animation.output_toggle();
        } break;
        case 'r': {
            out.println(F("toggle animation_run"));
            animation.animation_run = !animation.animation_run;
        } break;
        case 'd': {
            out.println(F("set effect_duration:"));
            uint16_t value = atoi(&command[1]);
            out.print(value);
            animation.effect_duration = value;
            out.println();
        } break;
        // case 'h': {
        //     menu__set_hue(out, command);
        // } break;
        // case 'c': {
        //     menu__set_saturation(out, command);
        // } break;
        // case 'b': {
        //     menu__set_brightness(out, command);
        // } break;
        case 'b': {
            out.print(F("Set brightness "));
            uint8_t command_offset = 1;
            uint8_t value = atoi(&command[command_offset]);
            out.print(value);
            animation.setBrightness(value);
            out.println();
        } break;
        case 'O': {
            menu__set_overwrite_black(out, command);
        } break;
        case 'q': {
            menu__set_overwrite_black_relative(out, command);
        } break;
        // ---------------------
        // case 'u': {
        //     out.println(F("write buffer to chips"));
        //     animation.tlc.show();
        // } break;
        // case 'e': {
        //     out.println(F("toggle ESPWM"));
        //     animation.tlc.set_fc_ESPWM_all(!animation.tlc.get_fc_ESPWM());
        //     animation.tlc.update_fc();
        // } break;
        // case 'g': {
        //     out.print(F("set grayscale frequency - new value:"));
        //     float value = atof(&command[1]);
        //     out.print(value);
        //     value = animation.gsclock_set_frequency_MHz(value);
        //     out.print(F(" → "));
        //     out.print(value, 4);
        //     out.println(F("MHz"));
        // } break;
        // case 's': {
        //     out.print(F("set spi baudrate in MHz - new value:"));
        //     float value = atof(&command[1]);
        //     out.print(value, 4);
        //     out.println(F("MHz"));
        //     animation.tlc.spi_baudrate = value * 1000 * 1000;
        //     // out.print(F(" → "));
        //     // out.print(animation.tlc.spi_baudrate);
        //     // out.println();
        // } break;
        // case 't': {
        //     animation.menu__test_buffer(out);
        // } break;
        case 'T': {
            menu__time_meassurements(out);
        } break;
        case 'p': {
            menu__set_pixel(out, command);
        } break;
        case 'P': {
            menu__set_board_dotstar(out, command);
        } break;
        // case 'P': {
        //     out.print(F("Set all pixel to "));
        //     uint16_t value = atoi(&command[1]);
        //     animation.tlc.set_pixel_all_16bit_value(value, value, value);
        //     out.print(value);
        //     out.println();
        // } break;
        case 'z': {
            out.println(F("Set all Pixel to black."));
            animation.fill_black();
            out.println();
        } break;
        // case 'Z': {
        //     out.println(F("Set all Pixel to 21845."));
        //     animation.tlc.set_pixel_all_16bit_value(21845, 21845, 21845);
        //     out.println();
        // } break;
        // case 'B': {
        //     out.println(F("Print Buffer:"));
        //     animation.print_tlc_buffer(out);
        //     out.println();
        // } break;
        // case 'F': {
        //     out.println(F("Print buffer_fc:"));
        //     animation.tlc.print_buffer_fc(out);
        //     out.println();
        // } break;
        //---------------------------------------------------------------------
        default: {
            if (strlen(command) > 0) {
                out.print(F("command '"));
                out.print(command);
                out.println(F("' not recognized. try again."));
            }
            instance->get_command_input_pointer()[0] = '?';
            instance->set_flag_EOC(true);
        }
    }  // end switch

    // end Command Parser
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
