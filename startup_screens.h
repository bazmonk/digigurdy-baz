#ifndef STARTUP_SCREENS_H
#define STARTUP_SCREENS_H

#include <EEPROM.h>

#include "common.h"
#include "display.h"
#include "config.h"
#include "bitmaps.h"
#include "tuning_screens.h"
#include "save_load_screens.h"

void startup_screen_sqeuence();
void about_screen();
void welcome_screen();
void options_screen();
void clear_eeprom();
void scene_options_screen();
void playing_options_screen();

#endif
