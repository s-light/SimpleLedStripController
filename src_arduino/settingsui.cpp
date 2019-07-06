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

#include "./mapping.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

// SettingsUI::SettingsUI(button):
// ) {
// SettingsUI::SettingsUI() {
SettingsUI::SettingsUI(
    MyAnimation &animation
):
    animation(animation),
    //       ^ '(' needed. its a bug in gcc..
    // https://stackoverflow.com/questions/10509603/why-cant-i-initialize-a-reference-in-an-initializer-list-with-uniform-initializ
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

        // if (counter != counter_last) {
        //     counter_last = counter;
        //     Serial.print("counter changed: ");
        //     Serial.print(counter);
        //     Serial.println();
        // }



        active_update();
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// active handling

void SettingsUI::active_update() {
    if (flag_active) {
        if (millis() - active_last >= active_timeout) {
            active_leave();
        }
    }
}

void SettingsUI::active_activate() {
    active_last = millis();
    if (!flag_active) {
        flag_active = true;
        Serial.println("flag_active = true");
    } else {
        // nothing to do..
    }
}

void SettingsUI::active_leave() {
    Serial.println("active_leave()");

    Print &out = Serial;
    animation.effect_print_current(out);
    out.print(F(" - "));
    print_param(out);
    out.println();

    flag_active = false;
    active_last = millis();
    if (flag_dirty) {
        // save changes
        Serial.println("TODO: Save changes...");
        flag_dirty = false;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~

void SettingsUI::switch_mode() {
    switch (animation.effect_current) {
        case MyAnimation::EFFECT::OFF: {
            // currently not in use.
            animation.effect_current = MyAnimation::EFFECT::STATIC;
        } break;
        case MyAnimation::EFFECT::STATIC: {
            animation.effect_current = MyAnimation::EFFECT::RAINBOW;
        } break;
        case MyAnimation::EFFECT::RAINBOW: {
            animation.effect_current = MyAnimation::EFFECT::STATIC;
            // animation.effect_current = EFFECT::OFF;
        } break;
    }
    Print &out = Serial;
    animation.effect_print_current(out);
    out.println();
}

void SettingsUI::change_param(int16_t value) {
    Print &out = Serial;
    out.print(F("Param: "));
    print_param(out);
    out.print(F(" = "));
    switch (animation.effect_current) {
        case MyAnimation::EFFECT::OFF: {
            // Nothing to do..
        } break;
        case MyAnimation::EFFECT::STATIC: {
            switch (static_current) {
                case STATIC_PARAM::HUE: {
                    animation.color_hsv.hue += value;
                    out.print(animation.color_hsv.hue);
                } break;
                case STATIC_PARAM::SATURATION: {
                    animation.color_hsv.saturation += value;
                    out.print(animation.color_hsv.saturation);
                } break;
                case STATIC_PARAM::VALUE: {
                    animation.color_hsv.value += value;
                    out.print(animation.color_hsv.value);
                } break;
            }
        } break;
        case MyAnimation::EFFECT::RAINBOW: {
            switch (rainbow_current) {
                case RAINBOW_PARAM::DURATION: {
                    uint32_t factor = duration_factor_map.mapit(
                        animation.effect_duration);
                    int32_t value_wf = (value * factor);
                    uint32_t temp = animation.effect_duration + value_wf;
                    // constrain to full step value..
                    temp = (temp / factor) * factor;
                    // Serial.print("temp ");
                    // Serial.println(temp);
                    // Serial.print("factor ");
                    // Serial.println(factor);
                    // Serial.print("temp / factor ");
                    // Serial.println(temp / factor);
                    // Serial.print("(temp / factor) * factor ");
                    // Serial.println((temp / factor) * factor);
                    if (temp > duration_max) {
                        if (value_wf > 0) {
                            temp = 300;
                        } else {
                            temp = duration_max;
                        }
                    } else if (temp < duration_min) {
                        if (value_wf > 0) {
                            temp = 300;
                        } else {
                            temp = duration_max;
                        }
                    }
                    animation.effect_duration = temp;
                    out.print(animation.effect_duration);
                } break;
                case RAINBOW_PARAM::BRIGHTNESS: {
                    animation.rainbow_brightness += value;
                    out.print(animation.rainbow_brightness);
                } break;
                case RAINBOW_PARAM::SPREAD: {
                    animation.rainbow_spread += value * 0.01;
                    out.print(animation.rainbow_spread);
                } break;
            }
        } break;
    }
    out.println();
}

void SettingsUI::print_param(Print &out) {
    switch (animation.effect_current) {
        case MyAnimation::EFFECT::OFF: {
            out.print(F("-"));
        } break;
        case MyAnimation::EFFECT::STATIC: {
            switch (static_current) {
                case STATIC_PARAM::HUE: {
                    out.print(F("HUE"));
                } break;
                case STATIC_PARAM::SATURATION: {
                    out.print(F("SATURATION"));
                } break;
                case STATIC_PARAM::VALUE: {
                    out.print(F("VALUE"));
                } break;
            }
        } break;
        case MyAnimation::EFFECT::RAINBOW: {
            switch (rainbow_current) {
                case RAINBOW_PARAM::DURATION: {
                    out.print(F("DURATION"));
                } break;
                case RAINBOW_PARAM::BRIGHTNESS: {
                    out.print(F("BRIGHTNESS"));
                } break;
                case RAINBOW_PARAM::SPREAD: {
                    out.print(F("SPREAD"));
                } break;
            }
        } break;
    }
}

void SettingsUI::switch_param() {
    switch (animation.effect_current) {
        case MyAnimation::EFFECT::OFF: {
            // Nothing to do..
        } break;
        case MyAnimation::EFFECT::STATIC: {
            switch (static_current) {
                case STATIC_PARAM::HUE: {
                    static_current = STATIC_PARAM::SATURATION;
                } break;
                case STATIC_PARAM::SATURATION: {
                    static_current = STATIC_PARAM::VALUE;
                } break;
                case STATIC_PARAM::VALUE: {
                    static_current = STATIC_PARAM::HUE;
                } break;
            }
        } break;
        case MyAnimation::EFFECT::RAINBOW: {
            switch (rainbow_current) {
                case RAINBOW_PARAM::DURATION: {
                    rainbow_current = RAINBOW_PARAM::BRIGHTNESS;
                } break;
                case RAINBOW_PARAM::BRIGHTNESS: {
                    rainbow_current = RAINBOW_PARAM::SPREAD;
                } break;
                case RAINBOW_PARAM::SPREAD: {
                    rainbow_current = RAINBOW_PARAM::DURATION;
                } break;
            }
        } break;
    }
    Print &out = Serial;
    print_param(out);
    out.println();
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

    active_activate();

    // react on event
    switch ((*instance).getEventLast()) {
        case slight_ButtonInput::event_down : {
            // Serial.println(F("down"));
        } break;
        case slight_ButtonInput::event_holddown : {
            Serial.print(F("duration active: "));
            Serial.println((*instance).getDurationActive());
            if ((*instance).getDurationActive() <= 5000) {
                animation.output_off();
            }
        } break;
        case slight_ButtonInput::event_up : {
            // Serial.println(F("up"));
        } break;
        case slight_ButtonInput::event_click : {
            Serial.println(F("click"));
            if (animation.output_get()) {
                switch_param();
            } else {
                animation.output_on();
            }
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long "));
            Serial.println((*instance).getDurationActive());
            animation.output_off();
        } break;
        case slight_ButtonInput::event_click_double : {
            Serial.println(F("click double"));
            switch_mode();
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
            // int16_t temp_steps = (*instance).getSteps();
            int16_t temp_stepsAccel = (*instance).getStepsAccelerated();
            // clear data
            (*instance).clearSteps();

            // Serial.print(F("  steps: "));
            // Serial.print(temp_steps);
            // Serial.print(F(" -> "));
            // Serial.println(temp_stepsAccel);
            // counter += temp_stepsAccel;

            active_activate();
            change_param(temp_stepsAccel);
        } break;
        // currently there are no other events fired.
    }  // end switch
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
