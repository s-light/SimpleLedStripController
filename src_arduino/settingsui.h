/******************************************************************************

    __doc__ = """
    settings handling
    """

    libraries used:
        ~ slight_ButtonInput
        ~ slight_RotaryEncoder
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



#ifndef SRC_ARDUINO_SETTINGSUI_H_
#define SRC_ARDUINO_SETTINGSUI_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_ButtonInput.h>
#include <slight_RotaryEncoder.h>

#include <Adafruit_DotStar.h>

#include "./animation.h"
#include "./mapping.h"

#define debug_FaderLin

class SettingsUI {
 public:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // defines

    enum class SETTINGS_MODE {
        EFFECT,
        GLOBAL,
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    // SettingsUI();
    SettingsUI(
        MyAnimation &animation
    );
    ~SettingsUI();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // basic library api
    void begin(Stream &out, slight_RotaryEncoder::tCallbackFunctionISR funcISR);
    void update();
    void end();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes & objects

    // OnBoardDotstar
    Adafruit_DotStar board_dotstar = Adafruit_DotStar(1, 8, 6, DOTSTAR_BGR);

    // button
    boolean mybutton_get_input(slight_ButtonInput *instance);
    void mybutton_event(slight_ButtonInput *instance);

    slight_ButtonInput mybutton = slight_ButtonInput(
        // uint8_t id_new
        1,
        // uint8_t pin_new,
        A3,
        // tCallbackFunctionGetInput callbackGetInput_new,
        std::bind(&SettingsUI::mybutton_get_input, this, std::placeholders::_1),
        // tCallbackFunction callbackOnEvent_new,
        std::bind(&SettingsUI::mybutton_event, this, std::placeholders::_1),
        // const uint16_t duration_debounce_new = 20,
        10,
        // const uint16_t duration_holddown_new = 1000,
        1000,
        // const uint16_t duration_click_long_new =   3000,
        2000,
        // const uint16_t duration_click_double_new = 250
        250
    );

    // encoder
    void myencoder_event(slight_RotaryEncoder *instance);

    slight_RotaryEncoder myencoder = slight_RotaryEncoder(
        // uint8_t id_new,
        1,
        // uint8_t pin_A_new,
        A5,
        // uint8_t pin_B_new,
        A4,
        // uint8_t pulse_per_step_new,
        1,
        // tCallbackFunction callbackOnEvent_new
        std::bind(&SettingsUI::myencoder_event, this, std::placeholders::_1)
    );


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // power modes

    enum class POWER_MODE {
        UNDEFINED,
        WAIT_FOR_POWER_OFF,
        OFF,
        WAIT_FOR_POWER_ON_CLICK,
        WAIT_FOR_POWER_ON,
        ON,
    };

    void system_power_off();
    void system_power_on();
    const uint32_t auto_power_off_wait = 5 * 1000;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

 private:
    MyAnimation &animation;

    // ambientlight sensor
    // void light_init(Stream &out);
    // void light_update();

    // button input
    void button_init(Stream &out);

    bool ready;

    // int16_t counter = 0;
    // int16_t counter_last = 0;

    // active handling
    bool flag_dirty = false;
    bool flag_active = false;
    uint32_t active_last = 0;
    uint32_t active_timeout = 5000;
    void active_update();
    void active_activate();
    void active_leave();
    void parameter_activate_overlay();

    void switch_mode();
    SETTINGS_MODE settings_mode = SETTINGS_MODE::GLOBAL;
    void print_settings_mode(Print &out);
    void print_param(Print &out);
    void switch_param();
    void change_param(int16_t value);

    // uint32_t light_end = 0;
    // uint32_t light_loopcount = 0;
    // float effect_position = 0.0;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // power modes
    uint32_t board_dotstar_standby_color = Adafruit_DotStar::Color(0, 0, 10);
    uint32_t board_dotstar_wait_color = Adafruit_DotStar::Color(100, 100, 0);
    uint32_t board_dotstar_active_color = Adafruit_DotStar::Color(0, 40, 0);

    POWER_MODE power_mode = POWER_MODE::UNDEFINED;

    void sleepmode_init(Stream &out);
    void go_to_sleep();
    void handle_power_mode();
};  // class SettingsUI

#endif  // SRC_ARDUINO_SETTINGSUI_H_
