#ifndef COMMON_H
#define COMMON_H

// This header gives access to the common gurdy objects for menu functions

#include "gurdybutton.h"     // For basic buttons
#include "keyboxbutton.h"    // For the keybox buttons
#include "gurdystring.h"     // For talking to MIDI
#include "exbutton.h"
#include "togglebutton.h"

// I want to be able to interact with these objects across several files, so this is how
// I'm doing it.  Is it proper?  Probably not, but it seems simple enough.

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

extern ToggleButton *bigbutton;

// This defines the +/- one octave transpose range.
extern int max_tpose;
extern int tpose_offset;

// This defines the 0, +2, +4 capo range.
extern int max_capo;
extern int capo_offset;

extern int drone_mode;
extern int mel_mode;

extern int d_mode;
extern int t_mode;

extern int play_screen_type;
extern uint8_t scene_signal_type;
extern bool gc_or_dg;

extern int myoffset;

#endif
