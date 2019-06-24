#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""SettingsUI."""

__doc__ = """
settingsui.py - RotaryInput UserInterface

change all animation settings
"""

import time
# import math

import configdict

##########################################
if __name__ == '__main__':
    print()
    print(42 * '*')
    print(__doc__)
    print(42 * '*')
    print()


##########################################

class SettingsUI(object):
    """SettingsUI."""

    config_defaults = {
        "settingsui": {
            "timeout_duration": 5,
        },
    }

    ##########################################
    # constructor

    def __init__(self, config):
        """Init."""
        super(SettingsUI, self).__init__()

        # extend config with defaults
        self.config = config
        configdict.extend_deep(self.config, self.config_defaults.copy())

        self.mode_active = False
        self.last_action = time.monotonic()
        self.timeout_duration = self.config["settingsui"]["timeout_duration"]

        self.on_settings_done = None

    ##########################################
    # main loop

    def update(self):
        """Loop."""
        if (time.monotonic() - self.last_action) >= self.timeout_duration:
            if self.mode_active:
                self.mode_active = False
                self.leave_settings_mode()

    ##########################################
    # others

    def check_mode(self):
        """Check Settings Mode."""
        # reste timer
        self.last_action = time.monotonic()
        # check
        if not self.mode_active:
            self.mode_active = True
            self.activate_settings_mode()

    def activate_settings_mode(self):
        """Activate Settings Mode."""
        print("Activate Settings Mode")

    def leave_settings_mode(self):
        """Leave Settings Mode."""
        print("Leave Settings Mode")
        if self.on_settings_done:
            staticmethod(self.on_settings_done())

    ##########################################
    # handlers

    def handle_inc(self):
        """Print inc."""
        self.check_mode()
        print("inc")

    def handle_dec(self):
        """Print dec."""
        self.check_mode()
        print("dec")

    def handle_click(self):
        """Print click."""
        self.check_mode()
        print("click")

    def handle_doubleclick(self):
        """Print doubleclick."""
        self.check_mode()
        print("doubleclick")

##########################################
# test main loop

if __name__ == '__main__':
    # animation_helper
    temp = SettingsUI()

    while True:
        temp.update()
