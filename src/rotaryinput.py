#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""RotaryInput."""

__doc__ = """
input_rotary.py - Rotary Encoder User-Interface

handels all things rotary input...
"""

# import time
# import math

import configdict

import board
import digitalio
import rotaryio


##########################################
if __name__ == '__main__':
    print()
    print(42 * '*')
    print(__doc__)
    print(42 * '*')
    print()


##########################################

class RotaryInput(object):
    """RotaryInput."""

    config_defaults = {
        "rotaryinput": {
            "doubleclick_duration": 100,
        },
    }

    ##########################################
    # constructor

    def __init__(self, config):
        """Init."""
        super(RotaryInput, self).__init__()

        # extend config with defaults
        self.config = config
        configdict.extend_deep(self.config, self.config_defaults.copy())
        self.doubleclick_duration = \
            self.config["rotaryinput"]["doubleclick_duration"]

        button_pin = board.A3
        encoder_in_A_pin = board.A4
        encoder_in_B_pin = board.A5

        self.button = digitalio.DigitalInOut(button_pin)
        self.button.direction = digitalio.Direction.INPUT
        self.button.pull = digitalio.Pull.UP

        self.encoder = rotaryio.IncrementalEncoder(
            encoder_in_A_pin, encoder_in_B_pin)

        self._button_state = None
        self._last_position = self.encoder.position

        self.on_click = None
        self.on_doubleclick = None
        self.on_inc = None
        self.on_dec = None

    ##########################################
    # main loop

    def update(self):
        """Loop."""
        # handle encoder
        current_position = self.encoder.position
        position_change = current_position - self._last_position
        if position_change > 0:
            for _ in range(position_change):
                # print("inc")
                if self.on_inc:
                    staticmethod(self.on_inc())
            # print(current_position)
        elif position_change < 0:
            for _ in range(-position_change):
                # print("dec")
                if self.on_dec:
                    staticmethod(self.on_dec())
            # print(current_position)
        self._last_position = current_position

        # handle button
        if not self.button.value and self._button_state is None:
            self._button_state = "pressed"
        if self.button.value and self._button_state == "pressed":
            # print("Button pressed.")
            if self.on_click:
                staticmethod(self.on_click())
            self._button_state = None


##########################################
# test main loop

if __name__ == '__main__':
    # animation_helper
    my_rotary_input = RotaryInput()

    # some debug callbacks
    def print_inc():
        """Print inc."""
        print("inc")

    def print_dec():
        """Print dec."""
        print("dec")

    def print_click():
        """Print click."""
        print("click")

    def print_doubleclick():
        """Print doubleclick."""
        print("doubleclick")

    my_rotary_input.on_inc = print_inc
    my_rotary_input.on_dec = print_dec
    my_rotary_input.on_click = print_click
    my_rotary_input.on_doubleclick = print_doubleclick

    while True:
        my_rotary_input.update()
