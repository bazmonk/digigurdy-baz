#ifndef LOADTUNINGS_H
#define LOADTUNINGS_H

#include "common.h"
#include "default_tunings.h"
#include "eeprom_values.h"

void load_preset_tunings(int preset);
void load_saved_tunings(int slot);

bool view_slot_screen(int slot_num);
bool view_preset_screen(int preset);

void signal_scene_change(int scene_idx);

#endif