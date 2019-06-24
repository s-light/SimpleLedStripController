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
                    "hue": 0.0,
                    "satturation": 1.0,
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
                            "satturation": 1.0,
                            "value": 1.0,
                        },
                        {
                            "hue": 0.15,
                            "satturation": 1.0,
                            "value": 1.0,
                        },
                    ]
                },
            },
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

        self.last_action = time.monotonic()

    ##########################################
    # main loop

    def update(self):
        """Loop."""
        pass


##########################################
# test main loop

if __name__ == '__main__':
    # animation_helper
    temp = Animation()

    while True:
        temp.update()
