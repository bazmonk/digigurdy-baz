# digigurdy-baz
Digigurdy Teensy code with a new tuning setup

This is a personal fork of the digigurdy code (https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy).

The latest version (v116/7 is the basis of this fork) offers four base tuning options (C/G with G or C drones, and G/D with D or G drones).  These can each be taken up or down an octave, and capo'ed up one whole tone.

This fork replaces that with:
* Two base options: C/G or G/D
* Three chanter options (normal is the 3/4 octave, high is 4/5, low is 2/3)
* TEN drone/trompette options for each base option.
* Capo one whole tone up on drone/trompette for any option.

This provides 120 tuning options.  While some of the octave combinations may be rare on a real gurdy, they should all be "compatible" options that could concievably work.

...Ok, some of them may be pretty strange.  But a volume knob should be able to go "too high" in order to guarantee it goes to "loud enough".  You may stick to just a few of the options, but the options you want should be in there :-)

The goal is to replicate the two-string drone combinations that can be done with a 6-string Nerdy Gurdy, significantly expand the tuning options, and find a way to do it that doesn't overwhelm the user with options if they don't wish to be.  The basic, "standard" tunings should be easily available.

Future plans are exploring using two additional channels to support a third or perhaps fourth droning string as well, essentially making it a six "string" gurdy.

## Installation

Your digigurdy may have an SSD1306 display (the "white OLED"), or the SH1106 display (the "blue OLED").  This code needs to be compiled for the correct display.

Near the very top of the code are two `#define` statements.  Uncomment the appropriate line to build for either display type.  The version on this repository is set to white (because that's the one I have).

For white OLEDs:

```
#define WHITE_OLED
//#define BLUE_OLED
```

For blue OLEDs:

```
//#define WHITE_OLED
#define BLUE_OLED
```
