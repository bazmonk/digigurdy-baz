#ifndef PLAY_FUNCTIONS_H
#define PLAY_FUNCTIONS_H

#include "common.h"
#include "play_screens.h"


void vol_up();
void vol_down();
void all_soundOff();
void all_soundKill();
void no_buzz_soundOff();
void tpose_up_1(bool playing);
void tpose_down_1(bool playing);
void cycle_capo(bool playing);
void tpose_up_x(bool playing, int steps);

#endif
