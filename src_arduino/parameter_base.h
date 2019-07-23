/******************************************************************************

    parameters...

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



#ifndef parameter_H_
#define parameter_H_

// include Core Arduino functionality
#include <Arduino.h>


#undef min
#undef max
#include <functional>

#include <FastLED.h>

#include "mapping.h"

// template <class T, T MIN_VALUE, T MAX, uint16_t PIXEL_COUNT_OVERLAY>
template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterBase {
public:

    using overlay_func_t = std::function<CRGBArray<PIXEL_COUNT_OVERLAY> ()>;
    using set_relative_func_t = std::function<void (int16_t offset)>;

    // constructor
    ParameterBase(
        char const * param_name,
        overlay_func_t overlay_customfunc = nullptr,
        set_relative_func_t set_relative_customfunc = nullptr
    ):
        param_name(param_name),
        overlay_customfunc(overlay_customfunc),
        set_relative_customfunc(set_relative_customfunc)
    {
        fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
    };
    ~ParameterBase() {};

    char const * param_name;
    virtual void print_name(Print &out) {
        out.print(this->param_name);
    }


    overlay_func_t overlay_customfunc = nullptr;

    CRGBArray<PIXEL_COUNT_OVERLAY> pixels_overlay;
    static const uint16_t BORDER = PIXEL_COUNT_OVERLAY / 10;
    static const uint16_t BORDER_END = PIXEL_COUNT_OVERLAY - BORDER;

    virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
        for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
            this->pixels_overlay[i] = CRGB::Black;
        }
        return this->pixels_overlay;
    };


    set_relative_func_t set_relative_customfunc = nullptr;

    virtual void set_relative(__attribute__((unused)) int16_t offset) {
        Serial.print("ParameterBase -- set relative called.");
        if (set_relative_customfunc != nullptr) {
            Serial.print("ParameterBase -- set_relative_customfunc called.");
        }
    }

    virtual void operator+=(int16_t offset) {
        this->set_relative(offset);
    };

    virtual void print_value(Print &out) {
        out.print("-");
    }
    // virtual std::ostream& operator<<(std::ostream& os) {
    //     return os << '-';
    // }

};  // class ParameterBase



template <uint16_t PIXEL_COUNT_OVERLAY, class T>
class ParameterTyped: public ParameterBase<PIXEL_COUNT_OVERLAY> {
public:

    // using overlay_func_t = ParameterBase<PIXEL_COUNT_OVERLAY>::overlay_func_t;
    using overlay_func_t = std::function<CRGBArray<PIXEL_COUNT_OVERLAY> ()>;
    using set_func_t = std::function<T (T value_new)>;
    using set_relative_func_t = std::function<void (int16_t offset)>;

    // constructor
    ParameterTyped(
        char const * param_name,
        T value_min,
        T value_max,
        T value_default,
        overlay_func_t overlay_customfunc = nullptr,
        set_relative_func_t set_relative_customfunc = nullptr,
        set_func_t set_customfunc = nullptr
    ):
        ParameterBase<PIXEL_COUNT_OVERLAY> (
            param_name,
            overlay_customfunc,
            set_relative_customfunc
        ),
        value_min(value_min),
        value_max(value_max),
        value_default(value_default),
        // set_relative_customfunc(set_relative_customfunc),
        set_customfunc(set_customfunc)
    {
        fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
        this->value = this->value_default;
    };


    const T value_min;
    const T value_max;
    const T value_default;
    T value;
    virtual void print_value(Print &out) {
        out.print(value);
    }
    // virtual std::ostream& operator<<(std::ostream& os) {
    //     return os << reinterpate_cast<T>(this->value);
    // }

    virtual void set_relative(int16_t offset) {
        // Serial.print("ParameterTyped - set relative called: ");
        // Serial.print(this->value);
        // Serial.print(" + '");
        // Serial.print(offset);
        // Serial.print("' -> ");
        if (this->set_relative_customfunc != nullptr) {
            // Serial.println();
            // Serial.print("  ParameterTyped - set_relative_customfunc called: ");
            this->set_relative_customfunc(offset);
        } else {
            T temp = this->value + offset;
            this->set(clamp(temp, this->value_min, this->value_max));
            // Serial.print(this->value);
        }
        // Serial.println();
    }

    virtual void operator+=(int16_t offset) {
        return this->set_relative(offset);
    };


    set_func_t set_customfunc = nullptr;

    virtual T set(T value_new) {
        // Serial.print("** set called: ");
        // Serial.print(value_new);
        // Serial.print(" -> ");
        if (set_customfunc != nullptr) {
            // Serial.print(" ** set_customfunc called.");
            value_new = set_customfunc(value_new);
        }
        this->value = clamp(value_new, this->value_min, this->value_max);
        // Serial.print(this->value);
        // Serial.println();
        return this->value;
    }

    virtual T operator=(T value_new) {
        return this->set(value_new);
    };


    virtual operator T () const {
        return this->value;
    }
};  // class ParameterTyped


#endif  // parameter_H_
