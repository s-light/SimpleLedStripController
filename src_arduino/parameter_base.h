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

// template <class T, T MIN_VALUE, T MAX, uint16_t PIXEL_COUNT_OVERLAY>
template <uint16_t PIXEL_COUNT_OVERLAY>
class ParameterBase {
public:

    using overlay_func_t = std::function<CRGBArray<PIXEL_COUNT_OVERLAY> ()>;

    // constructor
    ParameterBase(
        char * param_name,
        overlay_func_t overlay_customfunc = nullptr
    ):
        param_name(param_name),
        overlay_customfunc(overlay_customfunc)
    {
        fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
    };
    ~ParameterBase() {};


    char * param_name = "BASE";
    virtual void print_name(Print &out) {
        out.print(this->param_name);
    }


    // using set_func_t = std::function<uint16_t (uint16_t value)>;
    // set_func_t set_customfunc_t = nullptr;
    //
    // virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
    //     for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
    //         this->pixels_overlay[i] = CRGB::Black;
    //     }
    //     return this->pixels_overlay;
    // };

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

};  // class ParameterBase



// template <uint16_t PIXEL_COUNT_OVERLAY, class T>
// class ParameterTyped {
// public:
//
//     using set_func_t = std::function<T (T value_new)>;
//
//     // constructor
//     ParameterTyped(
//         char * param_name,
//         T min,
//         T max,
//         overlay_func_t overlay_customfunc = nullptr,
//         set_func_t set_customfunc = nullptr
//     ):
//         param_name(param_name),
//         min(min),
//         max(max),
//         overlay_customfunc(overlay_customfunc),
//         set_customfunc(set_customfunc)
//     {
//         fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
//     };
//
//     const T min;
//     const T max;
//
//     set_func_t set_customfunc = nullptr;
//
//     virtual T operator=(T value_new) {
//         if (set_customfunc != nullptr) {
//             value_new = set_customfunc(value_new);
//         }
//         this->value = clamp(value_new, this->min, this->max);
//         return this->value;
//     }
//
//     virtual T set(T value_new) {
//         return this->set(value_new);
//     };
//
//     virtual operator T () const {
//         return this->value;
//     }
//
// protected:
//     T value;
// };  // class ParameterTyped


// class ParameterInt16 {
// public:
//     // constructor
//     ParameterInt16(
//         char * param_name,
//         int16_t min,
//         int16_t max,
//         set_func_t custom_set_function = nullptr,
//         set_func_t set_customfunc = nullptr
//     ):
//         param_name(param_name),
//         custom_set_function(custom_set_function),
//         set_customfunc(set_customfunc),
//     {
//         fill_solid(this->pixels_overlay, PIXEL_COUNT_OVERLAY, CHSV(0, 255, 0));
//     };
//
//
//     using set_func_t = std::function<uint16_t (uint16_t value)>;
//     set_func_t set_customfunc = nullptr;
//
//     virtual CRGBArray<PIXEL_COUNT_OVERLAY> render_overlay() {
//         for (int i = 0; i < PIXEL_COUNT_OVERLAY; i++) {
//             this->pixels_overlay[i] = CRGB::Black;
//         }
//         return this->pixels_overlay;
//     };
//
// };  // class ParameterInt16


#endif  // parameter_H_
