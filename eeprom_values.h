#ifndef EEPROM_VALUES_H
#define EEPROM_VALUES_H

// This file holds memory addresses within the EEPROM used by digigurdy-baz.
//
// EEPROM memory is rated for at least 100,000 writes (reads are harmless).
// If you end up burning out one of the memory locations, you can shift them around
// by redefining the assingments here.

// The slots below should be arranged so that they are at least 20 apart.  This is
// to provide room for future upgrades.  There may be two more channels later, as well
// as a second value (volume) for each channel.
//
// Teensy3.5 has 4096 bytes of EEPROM, so there's lots of room here.
static const int EEPROM_SLOT1 = 0;
static const int EEPROM_SLOT2 = 20;
static const int EEPROM_SLOT3 = 40;
static const int EEPROM_SLOT4 = 60;

// These define what the values are within the "slots".  You'd add this to one of the
// SLOTs above to get the actual address in EEPROM.
static const int EEPROM_HI_MEL = 0;
static const int EEPROM_LO_MEL = 1;
static const int EEPROM_DRONE = 2;
static const int EEPROM_TROMP = 3;
static const int EEPROM_BUZZ = 4;
static const int EEPROM_TPOSE = 5;
static const int EEPROM_CAPO = 6;
static const int EEPROM_HI_MEL_VOL = 7;
static const int EEPROM_LOW_MEL_VOL = 8;
static const int EEPROM_DRONE_VOL = 9;
static const int EEPROM_TROMP_VOL = 10;
static const int EEPROM_BUZZ_VOL = 11;
static const int EEPROM_KEYCLICK_VOL = 12;
static const int EEPROM_HI_MEL_GROS = 13;
static const int EEPROM_LOW_MEL_GROS = 14;
static const int EEPROM_TROMP_GROS = 15;
static const int EEPROM_DRONE_GROS = 16;
static const int EEPROM_BUZZ_GROS = 17;

// This int saves the play screen type.  0 = note + staff, 1 = note only;
static const int EEPROM_DISPLY_TYPE = 100;

// This int saves how saved tunings are conveyed to the midi controller.
// 0 = nothing is done (default and old style)
// 1 = Conveyed as a Program Control message on channel 1
// ... Reserved for future use
static const int EEPROM_SCENE_SIGNALLING = 101;

// This int saved the LED on/off preference.
// 0 = no buzz LED
// 1 = LED buzz
static const int EEPROM_BUZZ_LED = 102;

// These three ints hold the last-chosen EX1/2/3/4/5/6 button functions.
// See ExButton::fn_choice_screen() for the numbering used.
static const int EEPROM_EX1 = 103;
static const int EEPROM_EX2 = 104;
static const int EEPROM_EX3 = 105;
static const int EEPROM_EX4 = 106;
static const int EEPROM_EX5 = 107;
static const int EEPROM_EX6 = 108;
static const int EEPROM_EX7 = 109;
static const int EEPROM_EX8 = 110;
static const int EEPROM_EX9 = 111;
static const int EEPROM_EX10 = 112;
static const int EEPROM_EXBB = 113;

static const bool EEPROM_USE_SOLFEGE = 114;

// This determines which output to use besides usbMIDI
// 0 = MIDI-OUT
// 1 = Trigger/Tsunami
// 2 = Both
static const int EEPROM_SEC_OUT = 115;

static const int EEPROM_MEL_VIBRATO = 116;

// These hold transpose steps, if they exist.
static const int EEPROM_EX1_TSTEP = 117;
static const int EEPROM_EX2_TSTEP = 118;
static const int EEPROM_EX3_TSTEP = 119;
static const int EEPROM_EX4_TSTEP = 120;
static const int EEPROM_EX5_TSTEP = 121;
static const int EEPROM_EX6_TSTEP = 122;
static const int EEPROM_EX7_TSTEP = 123;
static const int EEPROM_EX8_TSTEP = 124;
static const int EEPROM_EX9_TSTEP = 125;
static const int EEPROM_EX10_TSTEP = 126;
static const int EEPROM_EXBB_TSTEP = 127;

#endif
