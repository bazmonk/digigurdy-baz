#ifndef PAUSE_SCREENS_H
#define PAUSE_SCREENS_H

// Need this now because EEPROM supports strings, which
// apparently doesn't load it itself.
#include <Arduino.h>
#include <EEPROM.h>

#include "common.h"
#include "display.h"
#include "bitmaps.h"
#include "notes.h"
#include "default_tunings.h"
#include "eeprom_values.h"
#include "startup_screens.h"
#include "ex_screens.h"
#include "tuning_screens.h"
#include "usb_power.h"

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
void signal_scene_change(int scene_idx);
void load_preset_tunings(int preset);
void load_saved_tunings(int slot);
void clear_eeprom();
bool view_slot_screen(int slot_num);
bool view_preset_screen(int preset);
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

#endif
