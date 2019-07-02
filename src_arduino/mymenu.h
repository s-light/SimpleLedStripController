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



#ifndef SRC_ARDUINO_MYMENU_H_
#define SRC_ARDUINO_MYMENU_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>
#include <slight_RotaryEncoder.h>

#include "./animation.h"


class MyMenu {
 public:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // defines

    using sketchinfo_func = void (*)(Print &out);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    // MyMenu();
    MyMenu(
        MyAnimation &animation,
        const sketchinfo_func sketchinfo_print
    );
    ~MyMenu();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Debug Output

    boolean infoled_state = 0;
    const byte infoled_pin = 13;

    uint32_t debugOut_LastAction = 0;
    const uint16_t debugOut_interval = 1000; //ms

    boolean debugOut_Serial_Enabled = 0;
    boolean debugOut_LED_Enabled = 1;
    void debugOut_update();


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes & objects

    // serial menu
    void menu__print_help(Print &out);
    void handleMenu_Main(slight_DebugMenu *instance);

    // slight_DebugMenu(Stream &in_ref, Print &out_ref, uint8_t input_length_new);
    slight_DebugMenu myDebugMenu;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // menu & helper
    void menu__test_xxx(Print &out);
    void menu__set_yyy(Print &out, char *command);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

 private:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    MyAnimation &animation;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // internal attributes
    bool ready;
    const sketchinfo_func sketchinfo_print;

};  // class MyMenu

#endif  // SRC_ARDUINO_MYMENU_H_
