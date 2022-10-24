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

void pause_screen();
bool tuning();
bool load_tuning_screen();
bool check_save_tuning(int slot);
void save_tuning_screen();
void options_about_screen();
bool other_options_screen();
void signal_scene_change(int scene_idx);
void load_preset_tunings(int preset);
void load_saved_tunings(int slot);
void save_tunings(int slot);
void clear_eeprom();
void tuning_hi_melody();
void tuning_low_melody();
void tuning_drone();
void tuning_tromp();
bool view_slot_screen(int slot_num);
bool view_preset_screen(int preset);
bool load_saved_screen();
bool load_preset_screen();
void scene_options_screen();
void playing_options_screen();
void options_screen();
void welcome_screen();
void tune_string_screen(GurdyString *this_string);
void manual_tuning_screen();
void change_volume_screen(GurdyString *this_string);
void volume_screen();

#endif
