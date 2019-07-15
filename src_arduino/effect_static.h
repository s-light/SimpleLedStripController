/******************************************************************************

    for more information have a look at effect.cpp

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



#ifndef effect_static_H_
#define effect_static_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <FastLED.h>

#include "effect_base.h"

template <uint16_t PIXEL_COUNT>
class EffectStatic: public EffectBase<PIXEL_COUNT> {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor
    EffectStatic();
    ~EffectStatic();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // basic library api
    // void begin(Stream &out);
    void update();
    // void end();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations
    const CHSV warm_white = CHSV(142, 100, 240);
    CHSV color_hsv = warm_white;

private:

};  // class EffectStatic


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// implementation

template <uint16_t PIXEL_COUNT>
EffectStatic<PIXEL_COUNT>::EffectStatic() {
    // ready = false;
}

template <uint16_t PIXEL_COUNT>
EffectStatic<PIXEL_COUNT>::~EffectStatic() {
    // end();
}

// template <uint16_t PIXEL_COUNT>
// void EffectStatic<PIXEL_COUNT>::begin(Stream &out) {
//     // clean up..
//     end();
//     // start up...
//     if (ready == false) {
//         // setup
//
//         // enable
//         ready = true;
//     }
// }

// template <uint16_t PIXEL_COUNT>
// void EffectStatic<PIXEL_COUNT>::end() {
//     if (ready) {
//         // nothing to do..
//     }
// }

template <uint16_t PIXEL_COUNT>
void EffectStatic<PIXEL_COUNT>::update() {
    Serial.println(test);
    // Serial.println(pixels);
    Serial.println(PIXEL_COUNT);
    Serial.println(color_hsv.hue);
    // fill_solid(pixels, PIXEL_COUNT, color_hsv);
}

#endif  // effect_static_H_
