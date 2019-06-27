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

#include <slight_DebugMenu.h>
#include <slight_ButtonInput.h>
#include <slight_RotaryEncoder.h>


class SettingsUI {
 public:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    // SettingsUI();
    SettingsUI(
        const uint8_t placeholder
    );
    ~SettingsUI();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // basic library api
    void begin(Stream &out, slight_RotaryEncoder::tCallbackFunctionISR funcISR);
    void update();
    void end();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CONST


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes & objects

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
        500,
        // const uint16_t duration_click_double_new = 250
        250
    );


    void myencoder_event(slight_RotaryEncoder *instance);

    slight_RotaryEncoder myencoder = slight_RotaryEncoder(
        // uint8_t id_new,
        1,
        // uint8_t pin_A_new,
        A4,
        // uint8_t pin_B_new,
        A5,
        // uint8_t pulse_per_step_new,
        2,
        // tCallbackFunction callbackOnEvent_new
        std::bind(&SettingsUI::myencoder_event, this, std::placeholders::_1)
    );


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // menu & helper
    void menu__test_xxx(Print &out);
    void menu__set_yyy(Print &out, char *command);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    bool ambientlight_run = true;

    uint16_t filter_duration = 20 * 1000;  // ms
    float light_event_threshold = 0.1;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

 private:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    // ambientlight sensor
    // void light_init(Stream &out);
    // void light_update();

    // button input
    void button_init(Stream &out);
    bool button_getinput(byte id, byte pin);
    void button_event(slight_ButtonInput *instance, byte event);
    // slight_ButtonInput::tCbfuncGetInput button_getinput_func;
    // slight_ButtonInput::tcbfOnEvent button_event_func;

    // button input
    // void encoder_init(Stream &out);
    // void encoder_event(slight_RotaryEncoder *instance, byte event);
    // slight_ButtonInput::tCbfuncGetInput encoder_getinput_func;


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // internal attributes
    bool ready;

    int16_t placeholder;

    int16_t counter = 0;
    int16_t counter_last = 0;

    uint8_t state = 0;
    uint8_t state_last = 0;

    // uint32_t light_start = 0;
    // uint32_t light_end = 0;
    // uint32_t light_loopcount = 0;
    // float effect_position = 0.0;

};  // class SettingsUI

#endif  // SRC_ARDUINO_SETTINGSUI_H_
