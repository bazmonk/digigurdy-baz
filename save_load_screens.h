#ifndef SAVE_LOAD_SCREENS_H
#define SAVE_LOAD_SCREENS_H

#include <EEPROM.h>

#include "common.h"
#include "config.h"
#include "display.h"
#include "eeprom_values.h"
#include "default_tunings.h" // Preset tunings.

void save_tuning_screen();
bool load_tuning_screen();
void save_tunings(int slot);
bool load_saved_screen();
bool load_preset_screen();
bool check_save_tuning(int slot);
bool view_preset_screen(int preset);
bool view_slot_screen(int slot_num);
void signal_scene_change(int scene_idx);
void load_preset_tunings(int preset);
void load_saved_tunings(int slot);

#endif
