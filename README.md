<!--lint disable list-item-indent-->
<!--lint disable list-item-bullet-indent-->
# SimpleLedStripController
Simple Controller for RGB-LED-Strips

This is a Arduino sketch for a simple LED-Strip Controller.
The user-interface is just a normal rotary encoder with pushbutton.

## usage
- push or rotate to wake up, then click (second time) to switch on
- rotate to change parameter
- click to switch to next parameter
- double click to switch between effect and global parameters
- long click (hold > 4seconds and release) to switch off

### parameter tree
- Global
    - effect
    - brightness
    - overwrite (use only part of led-strip → rest is overwritten with black)
- Effect
    - static
        - hue
        - saturation
    - rainbow
        - duration
        - spread
    - plasma
        - duration
        - spread
        - hue

## hardware
- [Adafruit ItsyBitsy M4 Express](https://learn.adafruit.com/introducing-adafruit-itsybitsy-m4)
- [Pololu Big Pushbutton Power Switch](https://www.pololu.com/product/2813)
- [SN74HCT125 as levelshifter](http://www.ti.com/lit/ds/symlink/sn74hct125.pdf)
- APA102 144Pixel/m \*3
- as heatsink for the LED-Strips: Aluminium L-Profile
- [MeanWell DC/DC converter SD-100A-5 (in: 12V; out: 5V 18A)](https://www.meanwell.com/webapp/product/search.aspx?prod=SD-100)

## build
to compile this in arduino you have to install a bunch of additionally libraries:
marked with \* are my own ones and are not jet added to the library index.
(so you have to download and install them the old manual way..)
- [slight_DebugMenu \*](https://github.com/s-light/slight_DebugMenu)
- [slight_ButtonInput \*](https://github.com/s-light/slight_ButtonInput)
- [slight_RotaryEncoder \*](https://github.com/s-light/slight_RotaryEncoder)
- [FastLED](http://fastled.io/)
- [Adafruit_DotStar](https://github.com/adafruit/Adafruit_DotStar)

## overview / concept
the first concept is written in circuit python.
but with a strip of 144*3=432 Pixels this is just to slow ;-)
i then ported this to C++. ([src_arduino](src_arduino/))

the Code is heavily structured with classes.
eventually at first this could be complicated to understand..  

the main application is split into 3 parts:
- [MyMenu](src_arduino/mymenu.h)
    a helper class for a serial debug and test menu
- [SettingsUI](src_arduino/settingsui.h)
- [MyAnimation](src_arduino/animation.h)  
    all things animation related. here the main work happens  
    this uses some sub-classes for the effects and parameters.  
    This way its easier to *automagically* handle some of the parameters:  
    there are `global_current` and `fx_current` and similar variables.
    these have the base type and so are able to point to child-classes.
    - [ParameterBase](src_arduino/parameter_base.h)
    - [EffectBase](src_arduino/effect_base.h)

it is used as interior lighting for a car - therefore a DC/DC converter is used to generate 5V from the 12V extension battery

## history
next time i will reevaluate the DC/DC converter market..  
eventually for this it would make more sens to use a [Pololu 5V, 15A Step-Down Voltage Regulator D24V150F5](https://www.pololu.com/product/2881)
the MeanWell SD-100A-5 is a great thing - but it misses a Enable Input.  
so i head to use the external Switch..  
the Pololu D24V150F5 has an Enable input - so no need for the extra switch..  
also if i had more time in the first design decision phase - and would - why ever -  
decide to use the SD-100A-5 i would reconsider just using a Mosfet as HighSide switch -  
trying out things like the [BTS441TG Smart Power High-Side-Switch](https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/automotive-smart-high-side-switch-profet/classic-profet/bts441tg/)

additionally i will try out another chip as levelshifter: [SN74LVC2T45](http://www.ti.com/product/SN74LVC2T45)  
the currently used [SN74HCT125 as levelshifter](http://www.ti.com/lit/ds/symlink/sn74hct125.pdf) maxes out at about 6MHz Clock/Data speed.  
but the LEDs could go up to 20MHz according to some references..  
this would give some more space for effect calculations...  
(its also fine as is - the classic rainbow gives us 100fps currently...)  
the other advantage of the SN74LVC2T45 is its *VCC Isolation Feature* -  
currently i have to disable the outputs of the levelshifter to High-Z manually -  
otherwise the leds try to power up from the 5V coming over the data-lines :-(  
this isolation feature just does exactly this: if on of the VCC's is off the output is HIGH-Z :-)  
one thing less to care about in software ;-)

currently the off-state current consumption is about 4mA@12V  
1,5mA are alone for the standby of the small 5V DC/DC converter that powers the uC...  
another ~1mA are going to the ItsyBitsy on-Board APA102 led...  
so there is a bunch of things that could be improved.  
but the Lights are run from the secondary Battery that is charged via solar..
