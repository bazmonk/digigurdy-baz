#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

const String VERSION = "2.0.1";
const String REL_DATE = "2022-12-16, v" + VERSION;

// Use one of these if you want, on the title/about screen.  Or make your own!
//const String EXTRA_LINE = "                     ";
const String EXTRA_LINE = " MIDI-OUT - LED KNOB ";
//const String EXTRA_LINE = "  MIDI-OUT - PEDAL   ";
//const String EXTRA_LINE = " TRIGGER - LED KNOB  ";
//const String EXTRA_LINE = " TSUNAMI - LED KNOB  ";

// ALL USERS!!! Uncomment one of these lines depending on what kind of OLED screen you have.
#define WHITE_OLED
//#define BLUE_OLED

// ALL USERS!!! If you're hooking up to a WAV Trigger or Super Tsunami, uncomment one of these.
// * These expect to transmit via pin 1, Serial1 Tx.
// * These effectively disables Serial MIDI.  USB MIDI is still available.
//#define USE_TRIGGER
//#define USE_TSUNAMI

// PEDAL and LED knob support:
//
// If you got your gurdy from John or made yours like he did, pin 40 was variously used for both
// an LED buzz indicator, or a vibrato pedal accessory socket.  You probably can't use both features
// below and should enable only one.

// Comment this out if you do not want to use the buzz LED feature
#define LED_KNOB

// LED_PIN is the pin used for the buzz LED.
const int LED_PIN = 40;

// Comment this out if you do not want to use the vibrato accessory pedal
//#define USE_PEDAL

// PEDAL_PIN is the pin used for the pedal accessory.
const int PEDAL_PIN = 40;

// PEDA_MAX_V should be set near (doesn't need to be exact) the max voltage your pedal will output.
// 1023 = 3.3v, so (1023/3.3) * [Your voltage] = the value you want here.
const float PEDAL_MAX_V = 658.0;

// VIBRATO: I use a long-delay, very slow vibrato on the melody strings.  This variable controls how
// much vibrato (how much modulation like with a physical mod wheel on a MIDI keyboard) to send.
// Setting it to 0 sends no modulation.  Max is 127.  I use 16...
const int MELODY_VIBRATO = 16;

// EXPRESSION:
// Beyond EXPRESSION_VMAX RPMs, the volume will be at max.
const float EXPRESSION_VMAX = 120.0;

// At the slowest crank velocity, volume will start at this value (0 = silent, 127 = max)
const int EXPRESSION_START = 90;

// Cranking and buzz behavior:

// NEW FOR OPTICAL CRANKS:
//
// The crank algorithm works in terms of the crank's actual velocity and acceleration.  Thus it is
// necessary to know how many slots there are in a revolution.
//
// This is a count of the black/blocking bars on your wheel, not the number of transitions.
const int NUM_SPOKES = 80;

// This is the minimum velocity that produces sound.  This is actual crank rpm (rev/minute).
const float V_THRESHOLD = 5.5;

// This is how long (in microseconds, 1000us = 1ms = 0.001s) the code waits in between reading the
// crank.  This determines the resolution, not how long we're waiting to detect movement.
const int SAMPLE_RATE = 100;

// How long we wait for potential movement of the crank changes dynamically, but not longer than
// this time in microseconds.
const int MAX_WAIT_TIME = 40000;

// Once the crank has stopped (after the DECAY_RATE above), we multiply the last velocity by this.
// E.g. 0.2 means if the velocity was 60, once we stop we consider the current velocity to be 12,
// then 2.4, then 0.48, etc.  This gets averaged in to give us a "smooth" spin-down.
const float DECAY_FACTOR = 0.00;

// This is how long in milliseconds to buzz *at least* once it starts.
const int BUZZ_MIN = 100;

// KEYBOX VARIABLES:
// The pin_array[] index here represents the MIDI note offset, and the value is the corresponding
// teensy pin.  This defines which physical keys are part of the "keybox" and what order they're in.
//
// NOTE: There's no key that produces 0 offset (an "open" string),
// so the first element is bogus.  It gets skipped entirely.
const int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
                   7, 31, 8, 32, 33, 18, 34, 19, 35, 36, 20, 37};

// This is literally just the size of the above array minus one.  I need this as a const to
// declare the KeyboxButton array later on... or I just don't know enough C++ to know how to
// *not* need it ;-)
const int num_keys = 24;

// These control which buttons on the keybox have the roles of the X, A/B, 1-6, etc. buttons.
// Users with non-"standard" keyboxes (if there is such a thing!) may need to adjust these.
//
// These are array indexes, so if you count chromatically up your keybox and then subtract 1,
// that is its index.
const int X_INDEX = 0;
const int A_INDEX = num_keys - 2;
const int B_INDEX = num_keys - 5;
const int BUTTON_1_INDEX = 1;
const int BUTTON_2_INDEX = 3;
const int BUTTON_3_INDEX = 4;
const int BUTTON_4_INDEX = 6;
const int BUTTON_5_INDEX = 8;
const int BUTTON_6_INDEX = 9;
const int TPOSE_UP_INDEX = num_keys - 1;
const int TPOSE_DN_INDEX = num_keys - 3;

#endif
