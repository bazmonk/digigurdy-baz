#ifndef PAUSE_SCREENS_H
#define PAUSE_SCREENS_H

// Need this now because EEPROM supports strings, which
// apparently doesn't load it itself.
#include <Arduino.h>
#include <EEPROM.h>

#include "common.h"
#include "config.h"
#include "display.h"
#include "bitmaps.h"
#include "notes.h"
#include "default_tunings.h"
#include "eeprom_values.h"
#include "startup_screens.h"
#include "ex_screens.h"
#include "tuning_screens.h"
#include "play_functions.h"
#include "usb_power.h"
#include "load_tunings.h"

#ifdef USE_GEARED_CRANK
  #include "gearcrank.h"
#else
  #include "gurdycrank.h"
#endif

#include "vibknob.h"

#ifdef USE_GEARED_CRANK
  extern GearCrank *mycrank;
#else
  extern GurdyCrank *mycrank;
#endif

extern VibKnob *myvibknob;

void pause_screen();
void options_about_screen();
bool other_options_screen();
void save_tunings(int slot);
bool load_tuning_screen();
bool check_save_tuning(int slot);
void save_tuning_screen();

void reset_ex_eeprom();
void reset_eeprom();

bool load_saved_screen();
bool load_preset_screen();
void scene_options_screen();
void playing_scr_screen();
void io_screen();
void sec_output_screen();
void options_screen();
void welcome_screen();
void led_screen();
void vib_screen();
void playing_config_screen();
void notation_config_screen();
void mel_vib_screen();

#endif
