#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Animation."""

__doc__ = """
animation.py - render nice things..

animation..
"""

import time
# import math

import configdict

import board
import adafruit_dotstar
import adafruit_fancyled.adafruit_fancyled as fancyled

##########################################
if __name__ == '__main__':
    print()
    print(42 * '*')
    print(__doc__)
    print(42 * '*')
    print()

##########################################

class Animation(object):
    """Animation."""

    config_defaults = {
        "animation": {
            "mode": "rainbow",
            "mode_settings": {
                "static": {
                    "hue": 0.08,
                    "saturation": 1.0,
                    "value": 1.0,
                },
                "rainbow": {
                    "duration": 10.0,
                    "spread": 0.5,
                    "brightness": 1.0,
                },
                "gradient": {
                    "duration": 10.0,
                    "stops": [
                        {
                            "hue": 0.0,
                            "saturation": 1.0,
                            "value": 1.0,
                        },
                        {
                            "hue": 0.15,
                            "saturation": 1.0,
                            "value": 1.0,
                        },
                    ]
                },
            },
            "pixel_count": 144*1,
            "offset": 0,
        },
    }

    ##########################################
    # constructor

    def __init__(self, config):
        """Init."""
        super(Animation, self).__init__()

        # extend config with defaults
        self.config = config
        configdict.extend_deep(self.config, self.config_defaults.copy())

        self.mode = self.config["animation"]["mode"]
        self.pixel_count = self.config["animation"]["pixel_count"]
        self.offset = self.config["animation"]["offset"]

        self.pixels = adafruit_dotstar.DotStar(
            board.SCK,
            board.MOSI,
            self.pixel_count,
            brightness=0.1,
            auto_write=False)

        self.last_action = time.monotonic()

    ##########################################
    # effects

    def render_static(self):
        """Render Static Color."""
        # pixels.fill(fancyled.CHSV(0.08, 1.0, 1.0))
        self.pixels.fill(fancyled.CHSV(
            self.config["animation"]["mode_settings"]["static"]["hue"],
            self.config["animation"]["mode_settings"]["static"]["saturation"],
            self.config["animation"]["mode_settings"]["static"]["value"]))

    def render_rainbow(self):
        """Render Rainbow."""
        for pixel in self.pixels:
            pixel = fancyled.CHSV(0.09, 1.0, 0.5)

    ##########################################
    # main loop

    def update(self):
        """Loop."""
        if self.mode is "static":
            self.render_static()
        elif self.mode is "rainbow":
            self.render_rainbow()
        self.pixels.show()


##########################################
# test main loop

if __name__ == '__main__':
    # animation_helper
    temp = Animation()

    while True:
        temp.update()
