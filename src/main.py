#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Simple Led Strip Controller."""

__doc__ = """
Simple Led Strip Controller

Enjoy the colors :-)
"""


from configdict import ConfigDict

from rotaryinput import RotaryInput
from settingsui import SettingsUI
from animation import Animation


##########################################
print(
    "\n" +
    (42 * '*') + "\n" +
    __doc__ + "\n" +
    (42 * '*') + "\n" +
    "\n"
)

##########################################
# helper function


##########################################
# function


##########################################
# main class

class MyMain(object):
    """MyMainClass."""

    config_defaults = {
        'rotaryinput': {},
        'settingsui': {},
        'animation': {},
    }

    def __init__(self):
        """Init."""
        super(MyMain, self).__init__()
        # self.arg = arg

        self.init_config()

        # create Sub-Objects
        self.rotaryinput = RotaryInput(self.config)
        self.settingsui = SettingsUI(self.config)
        self.animation = Animation(self.config)

        self.prepare_callbacks()

    def prepare_callbacks(self):
        """Prepare callbacks."""
        # call class methode from static callback
        temp = self.settingsui

        def handle_inc():
            temp.handle_inc()

        def handle_dec():
            temp.handle_dec()

        def handle_click():
            temp.handle_click()

        def handle_doubleclick():
            temp.handle_doubleclick()

        self.rotaryinput.on_inc = handle_inc
        self.rotaryinput.on_dec = handle_dec
        self.rotaryinput.on_click = handle_click
        self.rotaryinput.on_doubleclick = handle_doubleclick

        def settings_done():
            print("save configuration..")
            try:
                self.my_config.write_to_file()
            except OSError as e:
                # OSError: [Errno 30] Read-only filesystem
                if e.errno is 30:
                    print(e)
                else:
                    raise e

        self.settingsui.on_settings_done = settings_done

    def init_config(self):
        """Read and parse configuration."""
        filename = "config.json"

        # setup config file:
        self.my_config = ConfigDict(self.config_defaults, filename)
        # print("my_config.config: {}".format(self.my_config.config))
        self.config = self.my_config.config
        # print("config: {}".format(self.config))

    def setup(self):
        """Setup."""
        print(42 * '*')
        # time.sleep(0.5)

    def loop(self):
        """Loop."""
        self.rotaryinput.update()
        self.settingsui.update()
        self.animation.update()
        # time.sleep(0.1)


##########################################
# main

def main():
    """Main."""
    my_main = MyMain()
    # print(42 * '*')
    print("setup")
    my_main.setup()
    print(42 * '*')
    print("loop")
    try:
        while True:
            my_main.loop()
    except KeyboardInterrupt:
        print("exit.")

    # try to save configuration to disc
    try:
        my_main.my_config.write_to_file()
    except Exception as e:
        print(e)


if __name__ == '__main__':
    main()
