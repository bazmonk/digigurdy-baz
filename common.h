#ifndef COMMON_H
#define COMMON_H

#include "gurdybutton.h"     // For basic buttons
#include "togglebutton.h"    // For click-on, click-on buttons
#include "keyboxbutton.h"    // For the keybox buttons
#include "gurdycrank.h"      // For the crank!
#include "gurdystring.h"     // For talking to MIDI
#include "hurdygurdy.h"      // For managing the keybox buttons

// I want to be able to interact with these objects across several files, so this is how
// I'm doing it.  Is it proper?  Probably not, but it seems simple enough.

// Declare the "keybox" and buttons.
HurdyGurdy *mygurdy;
ToggleButton *bigbutton;
GurdyCrank *mycrank;

// As musical keys, these are referred to in the mygurdy object above.
// This declaration of them is specifically for their use as navigational
// buttons in the menu screens.  ok = O, back = X.
KeyboxButton *myAButton;
KeyboxButton *myXButton;
KeyboxButton *my1Button;
KeyboxButton *my2Button;
KeyboxButton *my3Button;
KeyboxButton *my4Button;
KeyboxButton *my5Button;
KeyboxButton *my6Button;

// For legacy button-combo support:
KeyboxButton *myAltTposeButton;
KeyboxButton *myAltTposeUp;
KeyboxButton *myAltTposeDown;
KeyboxButton *myBButton;

// Note that there aren't special classes for melody, drone, even the keyclick.
// They are differentiated in the main loop():
// * A melody string is one that changes with the keybox offset.
// * A drone/trompette is one that doesn't change.
// * The keyclick "string" is just a drone that comes on and off at particular times.
// * The buzz "string" is also just a drone that comes on/off at other particular times.
GurdyString *mystring;
GurdyString *mylowstring;
GurdyString *mykeyclick;
GurdyString *mytromp;
GurdyString *mydrone;
GurdyString *mybuzz;

// These are the dedicated transpose/capo buttons
GurdyButton *tpose_up;
GurdyButton *tpose_down;
GurdyButton *capo;

// These are the "extra" buttons, new on the rev3.0 gurdies
GurdyButton *ex1Button;
GurdyButton *ex2Button;
GurdyButton *ex3Button;

#endif
