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
#include "./settingsui.h"

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>
#include <slight_ButtonInput.h>
#include <slight_RotaryEncoder.h>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

// SettingsUI::SettingsUI(button):
// ) {
// SettingsUI::SettingsUI() {
SettingsUI::SettingsUI(
    const MyAnimation animation
):
    animation{animation},
    ready{false}
// NOLINTNEXTLINE(whitespace/braces)
{
    // nothing to do right now..
}

SettingsUI::~SettingsUI() {
    end();
}

void SettingsUI::begin(
    Stream &out,
    slight_RotaryEncoder::tCallbackFunctionISR funcISR
) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup
        out.println("SettingsUI begin:");
        // light_init(out);
        button_init(out);
        out.println("  myencoder.begin");
        myencoder.begin(funcISR);
        out.println("done:");
        ready = true;
    }
}


void SettingsUI::end() {
    if (ready) {
        ready = false;
    }
}

void SettingsUI::update() {
    if (ready) {
        // light_update();
        mybutton.update();
        myencoder.update();

        if (state != state_last) {
            state_last = state;
            Serial.println("state changed!");
        }
        if (counter != counter_last) {
            counter_last = counter;
            Serial.print("counter changed: ");
            Serial.print(counter);
            Serial.println();
        }
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ambientlight sensor

// void SettingsUI::light_init(Stream &out) {
//     out.println(F("setup light sensor:"));
//     // TODO(s-light): implement
//     out.println(F("  finished."));
// }
//
// void SettingsUI::light_update() {
//     // TODO(s-light): implement
// }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// button

void SettingsUI::button_init(Stream &out) {
    out.println(F("setup button input:"));

    out.println(F("  set pinMode INPUT_PULLUP"));
    pinMode(mybutton.pin, INPUT_PULLUP);
    out.println(F("  mybutton.begin()"));
    mybutton.begin();
    out.println(F("  flag_filter_multi_click_events true"));
    mybutton.flag_filter_multi_click_events = true;

    out.println(F("  finished."));
}

boolean SettingsUI::mybutton_get_input(slight_ButtonInput *instance) {
    // read input + invert: button closes to GND.
    return !digitalRead((*instance).pin);
}

void SettingsUI::mybutton_event(slight_ButtonInput *instance) {
    // Serial.print(F("instance:"));
    // Serial.print((*instance).id);
    // Serial.print(F(" - event: "));
    // (*instance).printEventLast(Serial);
    // Serial.println();

    // react on event
    switch ((*instance).getEventLast()) {
        case slight_ButtonInput::event_down : {
            // Serial.println(F("down"));
        } break;
        case slight_ButtonInput::event_holddown : {
            Serial.print(F("duration active: "));
            Serial.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_up : {
            // Serial.println(F("up"));
        } break;
        case slight_ButtonInput::event_click : {
            Serial.println(F("click"));
            state += 1;
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long "));
            Serial.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_click_double : {
            Serial.println(F("click double"));
        } break;
        case slight_ButtonInput::event_click_triple : {
            // Serial.println(F("click triple"));
        } break;
        case slight_ButtonInput::event_click_multi : {
            Serial.print(F("click multi - count: "));
            Serial.println((*instance).getClickCount());
        } break;
    }  // end switch
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encoder

void SettingsUI::myencoder_event(slight_RotaryEncoder *instance) {
    // Serial.print(F("instance:"));
    // Serial.print((*instance).id);
    // Serial.print(F(" - event: "));
    // (*instance).printEventLast(Serial);
    // Serial.println();

    // react on event
    switch ((*instance).getEventLast()) {
        // case slight_RotaryEncoder::event_StateChanged : {
        //     Serial.print(F("\t state: "));
        //     (*instance).printState(Serial);
        //     Serial.println();
        // } break;
        // rotation
        case slight_RotaryEncoder::event_Rotated : {
            // get current data
            int16_t temp_steps = (*instance).getSteps();
            int16_t temp_stepsAccel = (*instance).getStepsAccelerated();
            // clear data
            (*instance).clearSteps();

            Serial.print(F("  steps: "));
            Serial.print(temp_steps);
            Serial.print(F(" -> "));
            Serial.println(temp_stepsAccel);
            counter += temp_stepsAccel;
        } break;
        // currently there are no other events fired.
    }  // end switch
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
