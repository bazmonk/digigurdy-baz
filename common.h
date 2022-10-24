#ifndef COMMON_H
#define COMMON_H

// https://www.pjrc.com/teensy/td_midi.html
// https://www.pjrc.com/teensy/td_libs_MIDI.html
#include <MIDI.h>

#include "gurdybutton.h"     // For basic buttons
#include "togglebutton.h"    // For click-on, click-on buttons
#include "keyboxbutton.h"    // For the keybox buttons
#include "gurdycrank.h"      // For the crank!
#include "gurdystring.h"     // For talking to MIDI
#include "hurdygurdy.h"      // For managing the keybox buttons
#include "notes.h"

// ************
// * COMMON.H *
// ************
// This file declares external objects out of the main file for use in other files here.
// For example, menu screen functions that modify strings need access to their objects.
// The idea is that a new screen function file just needs this in its header to grab the stuff
// typically needed.  Some things (like EEPROM) are left out because they're only used in one
// or two places.

// I want to be able to interact with these objects across several files, so this is how
// I'm doing it.  Is it proper?  Probably not, but it seems simple enough.

// Create a new MidiInterface object using that serial interface
extern MidiInterface<SerialMIDI<HardwareSerial>> *myMIDI;

// This is for the crank, the audio-to-digital chip
extern ADC* adc;

// Declare the "keybox" and buttons.
extern HurdyGurdy *mygurdy;
extern ToggleButton *bigbutton;
extern GurdyCrank *mycrank;

// As musical keys, these are referred to in the mygurdy object above.
// This declaration of them is specifically for their use as navigational
// buttons in the menu screens.  ok = O, back = X.
extern KeyboxButton *myAButton;
extern KeyboxButton *myXButton;
extern KeyboxButton *my1Button;
extern KeyboxButton *my2Button;
extern KeyboxButton *my3Button;
extern KeyboxButton *my4Button;
extern KeyboxButton *my5Button;
extern KeyboxButton *my6Button;

// For legacy button-combo support:
extern KeyboxButton *myAltTposeButton;
extern KeyboxButton *myAltTposeUp;
extern KeyboxButton *myAltTposeDown;
extern KeyboxButton *myBButton;

// Note that there aren't special classes for melody, drone, even the keyclick.
// They are differentiated in the main loop():
// * A melody string is one that changes with the keybox offset.
// * A drone/trompette is one that doesn't change.
// * The keyclick "string" is just a drone that comes on and off at particular times.
// * The buzz "string" is also just a drone that comes on/off at other particular times.
extern GurdyString *mystring;
extern GurdyString *mylowstring;
extern GurdyString *mykeyclick;
extern GurdyString *mytromp;
extern GurdyString *mydrone;
extern GurdyString *mybuzz;

// These are the dedicated transpose/capo buttons
extern GurdyButton *tpose_up;
extern GurdyButton *tpose_down;
extern GurdyButton *capo;

// These are the "extra" buttons, new on the rev3.0 gurdies
extern GurdyButton *ex1Button;
extern GurdyButton *ex2Button;
extern GurdyButton *ex3Button;

extern int tpose_offset;
extern int capo_offset;

extern uint8_t scene_signal_type;
extern int play_screen_type;

extern int drone_mode;
extern int mel_mode;

#endif
