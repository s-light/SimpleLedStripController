/******************************************************************************

    src_arduino.ino
        SimpleLedStripController.
        debugout on usbserial interface: 115200baud

    hardware:
        Board:
            Arduino compatible (with serial port)
            LED on pin 13
        HW:
            APA102 (DotStar) LED-Strip
                SCK
                MOSI
            rotary encoder
                button: A3
                A: A4
                B: A5

    ToDo:
    more research on sleep-mode
    Adafruit_SleepyDog
        https://github.com/adafruit/Adafruit_SleepyDog/blob/master/examples/Sleep/Sleep.ino
    STANDBY sleep mode on SAMD51?
        https://forum.arduino.cc/index.php?topic=600359.0
    ArduinoLowPower - External wakeup
        https://www.arduino.cc/en/Tutorial/LowPowerExternalWakeup



    libraries used:
        ~ FastLED
            ! please fill in !
        ~ slight_DebugMenu
        ~ slight_ButtonInput
        ~ slight_RotaryEncoder
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Includes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// use "file.h" for files in same directory as .ino
// #include "file.h"
// use <file.h> for files in library directory
// #include <file.h>

#include <slight_RotaryEncoder_CallbackHelper.h>

#include <slight_DebugMenu.h>

#include "animation.h"
#include "settingsui.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Info
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void sketchinfo_print(Print &out) {
    out.println();
    //             "|~~~~~~~~~|~~~~~~~~~|~~~..~~~|~~~~~~~~~|~~~~~~~~~|"
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("|                       ^ ^                      |"));
    out.println(F("|                      (0,0)                     |"));
    out.println(F("|                      ( _ )                     |"));
    out.println(F("|                       \" \"                      |"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("| src_arduino.ino"));
    out.println(F("|   SimpleLedStripController."));
    out.println(F("|"));
    out.println(F("| This Sketch has a debug-menu:"));
    out.println(F("| send '?'+Return for help"));
    out.println(F("|"));
    out.println(F("| dream on & have fun :-)"));
    out.println(F("|"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("|"));
    //out.println(F("| compiled: Nov 11 2013  20:35:04"));
    out.print(F("| compiled: "));
    out.print(F(__DATE__));
    out.print(F("  "));
    out.print(F(__TIME__));
    out.println();
    out.print(F("| last changed: "));
    out.print(F(__TIMESTAMP__));
    out.println();
    //
    // out.println(F("|"));
    // out.print(F("| __FILE__: "));
    // out.print(F(__FILE__));
    // out.println();
    // out.print(F("| __BASE_FILE__"));
    // out.print(F(__BASE_FILE__));
    // out.println();
    // out.println(F("|"));
    //
    out.println(F("|"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println();

    // __DATE__ Nov 11 2013
    // __TIME__ 20:35:04
    // __TIMESTAMP__ Tue Dec 27 14:14:04 2016
    // __FILE__  /home/stefan/mydata/arduino_sketchbook/libraries/slight_TLC5957/examples/src_arduino/src_arduino.ino
    // __BASE_FILE__ /tmp/arduino_build_330237/sketch/src_arduino.ino.cpp

}


// Serial.print to Flash: Notepad++ Replace RegEx
//     Find what:        Serial.print(.*)\("(.*)"\);
//     Replace with:    Serial.print\1\(F\("\2"\)\);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// definitions (global)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug Output

boolean infoled_state = 0;
const byte infoled_pin = 13;

uint32_t debugOut_LastAction = 0;
const uint16_t debugOut_interval = 1000; //ms

boolean debugOut_Serial_Enabled = 1;
boolean debugOut_LED_Enabled = 1;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Menu

// slight_DebugMenu(Stream &in_ref, Print &out_ref, uint8_t input_length_new);
slight_DebugMenu myDebugMenu(Serial, Serial, 20);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sub-Parts

MyAnimation animation = MyAnimation();

SettingsUI settingsui = SettingsUI(42);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// debug things

void debugOut_update() {
    uint32_t duration_temp = millis() - debugOut_LastAction;
    if (duration_temp > debugOut_interval) {
        debugOut_LastAction = millis();

        if ( debugOut_Serial_Enabled ) {
            Serial.print(millis());
            Serial.print(F("ms;"));
            Serial.println();
        }

        if ( debugOut_LED_Enabled ) {
            infoled_state = ! infoled_state;
            if (infoled_state) {
                //set LED to HIGH
                digitalWrite(infoled_pin, HIGH);
            } else {
                //set LED to LOW
                digitalWrite(infoled_pin, LOW);
            }
        }

    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Menu System

void menu__print_help(Print &out) {
    // help
    out.println(F("____________________________________________________________"));
    out.println();
    out.println(F("Help for Commands:"));
    out.println();
    out.println(F("\t '?': this help"));
    out.println(F("\t '!': sketch info"));
    out.println(F("\t 'y': toggle DebugOut livesign print"));
    out.println(F("\t 'Y': toggle DebugOut livesign LED"));
    out.println(F("\t 'x': tests"));
    out.println();
    out.print(F("\t 'r': toggle animation_run 'r' ("));
    out.print(animation.animation_run);
    out.println(F(")"));
    out.print(F("\t 'd': set effect_duration 'd1000' ("));
    out.print(animation.effect_duration);
    out.println(F("ms)"));
    out.print(F("\t 'h': set hue 'h1.0' ("));
    out.print(animation.hue, 4);
    out.println(F(")"));
    out.print(F("\t 'c': set contrast 'c1.0' ("));
    out.print(animation.contrast, 4);
    out.println(F(")"));
    out.print(F("\t 'b': set brightness 'b1.0' ("));
    out.print(animation.brightness, 4);
    out.println(F(")"));
    out.println();
    // out.println(F("\t 'f': test fc 'f'"));
    out.println(F("\t 'u': tlc.show() 'u'"));
    out.print(F("\t 'e': toggle ESPWM 'e' ("));
    out.print(animation.tlc.get_fc_ESPWM());
    out.println(F(")"));
    out.print(F("\t 'g': set grayscale frequency in MHz 'g1.0' ("));
    out.print(animation.gsclock_get_frequency_MHz(), 4);
    out.println(F("MHz)"));
    out.print(F("\t 's': set spi baudrate in MHz 's1.0' ("));
    out.print(animation.tlc.spi_baudrate / (1000.0 * 1000), 4);
    out.println(F("MHz)"));
    out.println(F("\t 't': set buffer to test values 't'"));
    out.println(F("\t 'T': time_meassurements 'T'"));
    out.println(F("\t 'p': set pixel 'p0:65535'"));
    out.println(F("\t 'P': set all pixel 'P65535'"));
    out.println(F("\t 'z': set all pixel to black 'b'"));
    out.println(F("\t 'Z': set all pixel to 21845 'z'"));
    out.println(F("\t 'B': print Buffer 'B'"));
    out.println(F("\t 'F': print buffer_fc 'F'"));
    out.println();
    out.println(F("____________________________________________________________"));
}


// Main Menu
void handleMenu_Main(slight_DebugMenu *pInstance) {
    Print &out = pInstance->get_stream_out_ref();
    char *command = pInstance->get_command_current_pointer();
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
        case 'h': {
            animation.menu__set_hue(out, command);
        } break;
        case 'c': {
            animation.menu__set_contrast(out, command);
        } break;
        case 'b': {
            animation.menu__set_brightness(out, command);
        } break;
        // ---------------------
        case 'u': {
            out.println(F("write buffer to chips"));
            animation.tlc.show();
        } break;
        case 'e': {
            out.println(F("toggle ESPWM"));
            animation.tlc.set_fc_ESPWM_all(!animation.tlc.get_fc_ESPWM());
            animation.tlc.update_fc();
        } break;
        case 'g': {
            out.print(F("set grayscale frequency - new value:"));
            float value = atof(&command[1]);
            out.print(value);
            value = animation.gsclock_set_frequency_MHz(value);
            out.print(F(" → "));
            out.print(value, 4);
            out.println(F("MHz"));
        } break;
        case 's': {
            out.print(F("set spi baudrate in MHz - new value:"));
            float value = atof(&command[1]);
            out.print(value, 4);
            out.println(F("MHz"));
            animation.tlc.spi_baudrate = value * 1000 * 1000;
            // out.print(F(" → "));
            // out.print(animation.tlc.spi_baudrate);
            // out.println();
        } break;
        case 't': {
            animation.menu__test_buffer(out);
        } break;
        case 'T': {
            animation.menu__time_meassurements(out);
        } break;
        case 'p': {
            animation.menu__set_pixel(out, command);
        } break;
        case 'P': {
            out.print(F("Set all pixel to "));
            uint16_t value = atoi(&command[1]);
            animation.tlc.set_pixel_all_16bit_value(value, value, value);
            out.print(value);
            out.println();
        } break;
        case 'z': {
            out.println(F("Set all Pixel to black."));
            animation.tlc.set_pixel_all_16bit_value(0, 0, 0);
            out.println();
        } break;
        case 'Z': {
            out.println(F("Set all Pixel to 21845."));
            animation.tlc.set_pixel_all_16bit_value(21845, 21845, 21845);
            out.println();
        } break;
        case 'B': {
            out.println(F("Print Buffer:"));
            animation.print_tlc_buffer(out);
            out.println();
        } break;
        case 'F': {
            out.println(F("Print buffer_fc:"));
            animation.tlc.print_buffer_fc(out);
            out.println();
        } break;
        //---------------------------------------------------------------------
        default: {
            if(strlen(command) > 0) {
                out.print(F("command '"));
                out.print(command);
                out.println(F("' not recognized. try again."));
            }
            pInstance->get_command_input_pointer()[0] = '?';
            pInstance->set_flag_EOC(true);
        }
    } // end switch

    // end Command Parser
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Subparts things
void settingsui_encoder_ISR() {
    settingsui.myencoder.updateClassic();
    // settingsui.encoder.updateGray();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setup
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // initialise PINs

        //LiveSign
        pinMode(infoled_pin, OUTPUT);
        digitalWrite(infoled_pin, HIGH);

        // as of arduino 1.0.1 you can use INPUT_PULLUP

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // initialise serial

        // wait for arduino IDE to release all serial ports after upload.
        delay(1000);
        // initialise serial
        Serial.begin(115200);

        // Wait for Serial Connection to be Opend from Host or
        // timeout after 6second
        uint32_t timeStamp_Start = millis();
        while( (! Serial) && ( (millis() - timeStamp_Start) < 2000 ) ) {
            // nothing to do
        }

        Serial.println();
        Serial.println();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // print welcome

        sketchinfo_print(Serial);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // setup sub-Parts

        animation.begin(Serial);
        settingsui.begin(Serial, settingsui_encoder_ISR);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // show serial commands

        myDebugMenu.set_callback(handleMenu_Main);
        myDebugMenu.begin(true);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // go

        Serial.println(F("wait 1s."));
        delay(1000);
        Serial.println(F("Loop:"));

} /** setup **/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main loop
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
    myDebugMenu.update();
    animation.update();
    settingsui.update();
    debugOut_update();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
