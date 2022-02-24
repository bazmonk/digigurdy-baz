# digigurdy-baz
Digigurdy Teensy code with a new tuning setup

This is a personal fork of the digigurdy code (https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy).

The current version (v116 as of this fork) offers four base tuning options (C/G with G or C drones, and G/D with D or G drones).  These can each be taken up or down an octave, and capo'ed up one whole tone.

My plan is to modify this to expand this into:
* Two base options: C/G or G/D
* Four drone/trompette options with each: e.g. for C/G, the options witll be C-C, C-G, G-C, G-G
* Octave up, down or normal
* Capo up 1 tone, up 2 tones, down 1 tone, down two tones, or normal

The goal is to replicate the two-string drone combinations that can be done with a 6-string Nerdy Gurdy, significantly expand the tuning options, and find a way to do it that doesn't overwhelm the user with options if they don't wish to be.  The basic, "standard" tunings should be easily available.

Future plans are exploring using two additional channels to support a third or perhaps fourth droning string as well, essentially making it a six "string" gurdy.

DIGIGURDY OWNERS: If you wish to use this code, I have a digigurdy with the SH1106 display, not SSD1306.  A few changes (left intact from the original code) are necessary to use the other display.  I also have a newer (circa Feb '22) version that includes dedicated octave up/down and capo buttons.  These will also be necessary as I plan to re-purpose them in the tunings menu.
