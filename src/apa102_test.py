#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
CircuitPython APA102 (DotStar) test.

inspired by / based on
https://learn.adafruit.com/circuitpython-essentials/circuitpython-dotstar
"""

import time

import board
import adafruit_dotstar

import adafruit_fancyled.adafruit_fancyled as fancyled

num_pixels = 144*3
pixels = adafruit_dotstar.DotStar(
    board.SCK,
    board.MOSI,
    num_pixels,
    # brightness=0.1,
    auto_write=False)
# no brightness gives a speedup for pixels.show():
# for 144*3 pixels: 73ms to 4ms per call!!


def wheel(pos):
    """Colorwheel."""
    # Input a value 0 to 255 to get a color value.
    # The colours are a transition r - g - b - back to r.
    if pos < 0 or pos > 255:
        return (0, 0, 0)
    if pos < 85:
        return (255 - pos * 3, pos * 3, 0)
    if pos < 170:
        pos -= 85
        return (0, 255 - pos * 3, pos * 3)
    pos -= 170
    return (pos * 3, 0, 255 - pos * 3)


def rainbow_cycle__wheel():
    """Render Rainbow Cycle."""
    for j in range(255):
        for i in range(num_pixels):
            rc_index = (i * 256 // num_pixels) + j
            pixels[i] = wheel(rc_index & 255)
        pixels.show()


# def normalize_to_01(x, in_max):
#     """Map value to 0..1 range."""
#     return x / in_max


def rainbow_cycle__hsv():
    """Render Rainbow Cycle."""
    for offset in range(255):
        for pixel_index in range(num_pixels):
            rc_index = (pixel_index * 256 // num_pixels) + offset
            hue = rc_index & 255
            pixels[pixel_index] = fancyled.CHSV(hue, 255, 255).pack()
        pixels.show()


##########################################

start = time.monotonic()
end = time.monotonic()
duration = end-start


print("set every pixel once:")
start = time.monotonic()
for i in range(num_pixels):
    pixels[i] = (50, 255, 0)
end = time.monotonic()
duration = end-start
print(
    "  took {}seconds for {}pixels\n"
    "  {}ms/pixel"
    "".format(duration, num_pixels, (duration/num_pixels)*1000)
)


print("set pixel hsv:")
start = time.monotonic()
for i in range(num_pixels):
    pixels[i] = fancyled.CHSV(0.08, 1.0, 1.0).pack()
end = time.monotonic()
duration = end-start
print(
    "  took {}seconds for {}pixels\n"
    "  {}ms/pixel"
    "".format(duration, num_pixels, (duration/num_pixels)*1000)
)


print("pixels.show():")
calls = 100
start = time.monotonic()
for i in range(calls):
    pixels.show()
end = time.monotonic()
duration = end-start
print(
    "  took {}seconds for {}calls\n"
    "  {}ms/call"
    "".format(duration, calls, (duration/calls)*1000)
)


while True:
    print("rainbow cycle")
    start = time.monotonic()
    # rainbow_cycle__wheel()
    rainbow_cycle__hsv()
    end = time.monotonic()
    duration = end-start
    print(
        "  took {}seconds\n"
        "  {}s/loop"
        "".format(duration, duration/255)
    )
