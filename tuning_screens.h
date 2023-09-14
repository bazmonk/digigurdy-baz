#ifndef TUNING_SCREENS_H
#define TUNING_SCREENS_H

#include "common.h"
#include "notes.h"

extern int use_solfege;

bool tuning();

void tuning_hi_melody();
void tuning_low_melody();
void tuning_drone();
void tuning_tromp();

void manual_tuning_screen();
void tune_string_screen(GurdyString *this_string);

void volume_screen();
void change_volume_screen(GurdyString *this_string);

void channel_screen();
void change_channel_screen(GurdyString *this_string);

void cool_kids_screen();
#endif
