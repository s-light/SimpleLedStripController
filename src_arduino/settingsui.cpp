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
        sleepmode_init(out);
        out.println("  board_dotstar");
        board_dotstar.begin();
        board_dotstar.setPixelColor(0, board_dotstar_active_color);
        board_dotstar.show();
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

        handle_power_mode();

        if (power_mode == POWER_MODE::ON) {
            active_update();
        }
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
    parameter_activate_overlay();
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
    animation.fx_current->print_name(out);
    out.print(F(" - "));
    print_param(out);
    out.println();

    flag_active = false;
    // animation.parameter_overlay_func = nullptr;
    animation.render_overlay_global = false;
    animation.render_overlay_effect = false;
    active_last = millis();
    if (flag_dirty) {
        // save changes
        Serial.println("TODO: Save changes...");
        flag_dirty = false;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~
// parameter handling

void SettingsUI::switch_mode() {
    switch (settings_mode) {
        case SETTINGS_MODE::EFFECT: {
            settings_mode = SETTINGS_MODE::GLOBAL;
        } break;
        case SETTINGS_MODE::GLOBAL: {
            settings_mode = SETTINGS_MODE::EFFECT;
        } break;
    }
    Print &out = Serial;
    print_settings_mode(out);
    out.println();
}

void SettingsUI::change_param(int16_t value) {
    Print &out = Serial;
    switch (settings_mode) {
        case SETTINGS_MODE::EFFECT: {
            animation.fx_current->print_name(out);
            out.print(" - ");
            animation.fx_current->parameter_print_name(out);
            out.print(": ");
            animation.fx_current->change_parameter(value);
            animation.fx_current->parameter_print_value(out);
            out.print("; ");
            // Nothing to do..
        } break;
        case SETTINGS_MODE::GLOBAL: {
            switch (global_current) {
                case GLOBAL_PARAM::EFFECT: {
                    animation.select_next_effect();
                    animation.fx_current->print_name(out);
                } break;
                case GLOBAL_PARAM::BRIGHTNESS: {
                    // animation.rainbow_brightness += value;
                    // out.print(animation.rainbow_brightness);
                    uint8_t temp = animation.getBrightness() + value;
                    animation.setBrightness(temp);
                    out.print(animation.getBrightness());
                } break;
                case GLOBAL_PARAM::OVERWRITE: {
                    // out.printf(
                    //     "%+3d (%3d, %3d) --> ",
                    //     value,
                    //     animation.overwrite_start_get(),
                    //     animation.overwrite_end_get());
                    animation.overwrite_set_relative(value);
                    out.printf(
                        "(%3d, %3d)",
                        animation.overwrite_start_get(),
                        animation.overwrite_end_get());
                } break;
            }
        } break;
    }
    out.println();
}

void SettingsUI::print_settings_mode(Print &out) {
    switch (settings_mode) {
        case SETTINGS_MODE::EFFECT: {
            out.print(F("EFFECT"));
        } break;
        case SETTINGS_MODE::GLOBAL: {
            out.print(F("GLOBAL"));
        } break;
    }
}

void SettingsUI::print_param(Print &out) {
    switch (settings_mode) {
        case SETTINGS_MODE::EFFECT: {
            animation.fx_current->print_name(out);
            out.print(": ");
            animation.fx_current->parameter_print_name(out);
        } break;
        case SETTINGS_MODE::GLOBAL: {
            switch (global_current) {
                case GLOBAL_PARAM::EFFECT: {
                    out.print(F("EFFECT"));
                } break;
                case GLOBAL_PARAM::BRIGHTNESS: {
                    out.print(F("BRIGHTNESS"));
                } break;
                case GLOBAL_PARAM::OVERWRITE: {
                    out.print(F("OVERWRITE"));
                } break;
            }
        } break;
    }
}

void SettingsUI::switch_param() {
    switch (settings_mode) {
        case SETTINGS_MODE::EFFECT: {
            animation.fx_current->parameter_next();
        } break;
        case SETTINGS_MODE::GLOBAL: {
            switch (global_current) {
                case GLOBAL_PARAM::EFFECT: {
                    global_current = GLOBAL_PARAM::BRIGHTNESS;
                } break;
                case GLOBAL_PARAM::BRIGHTNESS: {
                    global_current = GLOBAL_PARAM::OVERWRITE;
                } break;
                case GLOBAL_PARAM::OVERWRITE: {
                    global_current = GLOBAL_PARAM::EFFECT;
                } break;
            }
        } break;
    }
    Print &out = Serial;
    print_param(out);
    out.println();
}

void SettingsUI::parameter_activate_overlay() {
    switch (settings_mode) {
        case SETTINGS_MODE::EFFECT: {
            animation.render_overlay_global = true;
            animation.render_overlay_effect = false;
            // animation.parameter_overlay_func =
            //     animation.fx_current->render_overlay;
        } break;
        case SETTINGS_MODE::GLOBAL: {
            animation.render_overlay_global = false;
            animation.render_overlay_effect = true;
            switch (global_current) {
                case GLOBAL_PARAM::EFFECT: {
                    // animation.parameter_overlay_func =
                    //     animation.render_overlay_EFFECT;
                    animation.param_global_current =
                        &animation.param_effect;
                } break;
                case GLOBAL_PARAM::BRIGHTNESS: {
                    // animation.parameter_overlay_func =
                    //     animation.render_overlay_BRIGHTNESS;
                    animation.param_global_current =
                        &animation.param_brightness;
                } break;
                case GLOBAL_PARAM::OVERWRITE: {
                    // animation.parameter_overlay_func =
                    //     animation.render_overlay_OVERWRITE;
                    animation.param_global_current =
                        &animation.param_overwrite;
                } break;
            }
        } break;
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// power modes

// based on MartinL post at
// https://forum.arduino.cc/index.php?topic=600359.msg4079526#msg4079526

void SettingsUI::sleepmode_init(Stream &out) {
    out.println(F("prepare sleepmode:"));
    // Set up SAMD51 to enter low power STANDBY mode
    PM->SLEEPCFG.bit.SLEEPMODE = 0x4;
    while (PM->SLEEPCFG.bit.SLEEPMODE != 0x4) {}
    out.println(F("  finished."));
}

void wakeup_isr() {
    // nothing to do here..
}

void SettingsUI::go_to_sleep() {
    USBDevice.detach();
    USBDevice.end();
    USBDevice.standby();

    attachInterrupt(mybutton.pin, wakeup_isr, LOW);

    // Complete data memory operations
    __DSB();
    // Put the SAMDx1 into deep sleep Zzzzzzz....
    __WFI();

    // HERE THE CODE RETURNS AFTER SLEEP!

    detachInterrupt(mybutton.pin);

    USBDevice.init();
    USBDevice.attach();
}


void SettingsUI::system_power_off() {
    power_mode = POWER_MODE::WAIT_FOR_POWER_OFF;
    board_dotstar.setPixelColor(0, board_dotstar_wait_color);
    board_dotstar.show();
    animation.output_off();
}

void SettingsUI::system_power_on() {
    power_mode = POWER_MODE::WAIT_FOR_POWER_ON;
    animation.output_on();
    board_dotstar.setPixelColor(0, board_dotstar_wait_color);
    board_dotstar.show();
}

void SettingsUI::handle_power_mode() {
    switch (power_mode) {
        case POWER_MODE::UNDEFINED: {
            system_power_off();
        } break;
        case POWER_MODE::WAIT_FOR_POWER_OFF: {
            if (animation.mode == MyAnimation::MODE::OFF) {
                power_mode = POWER_MODE::OFF;
            }
        } break;
        case POWER_MODE::OFF: {
            board_dotstar.setPixelColor(0, board_dotstar_standby_color);
            board_dotstar.show();
            power_mode = POWER_MODE::OFF;

            go_to_sleep();

            // welcomem back.
            power_mode = POWER_MODE::WAIT_FOR_POWER_ON_CLICK;
            active_last = millis();
            // animation.output_on();
            board_dotstar.setPixelColor(0, board_dotstar_wait_color);
            board_dotstar.show();
        } break;
        case POWER_MODE::WAIT_FOR_POWER_ON_CLICK: {
            if (millis() - active_last > auto_power_off_wait) {
                system_power_off();
            }
        } break;
        case POWER_MODE::WAIT_FOR_POWER_ON: {
            if (animation.mode == MyAnimation::MODE::RUN) {
                power_mode = POWER_MODE::ON;
                board_dotstar.setPixelColor(0, board_dotstar_active_color);
                board_dotstar.show();
            }
        } break;
        case POWER_MODE::ON: {
            // nothing to do here..
        } break;
    }
}

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
            // if ((*instance).getDurationActive() <= 5000) {
            //     animation.output_off();
            //     board_dotstar.setPixelColor(0, 50, 50, 0);
            //     board_dotstar.show();
            // }
        } break;
        case slight_ButtonInput::event_up : {
            Serial.println(F("up"));
        } break;
        case slight_ButtonInput::event_click : {
            Serial.println(F("click"));
            if (power_mode == POWER_MODE::WAIT_FOR_POWER_ON_CLICK) {
                system_power_on();
            } else {
                switch_param();
            }
            // if (animation.output_get()) {
            //     switch_param();
            // } else {
            //     animation.output_on();
            //     board_dotstar.setPixelColor(0, board_dotstar_active_color);
            //     board_dotstar.show();
            // }
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long "));
            Serial.println((*instance).getDurationActive());
            system_power_off();
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

            // if (power_mode == POWER_MODE::WAIT_FOR_POWER_ON_CLICK) {
            //     system_power_on();
            // } else {
            //     active_activate();
            //     change_param(temp_stepsAccel);
            // }
            active_activate();
            change_param(temp_stepsAccel);
        } break;
        // currently there are no other events fired.
    }  // end switch
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
