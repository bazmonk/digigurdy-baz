#ifndef EXFUNCTIONS_H
#define EXFUNCTIONS_H

#include "play_screens.h"
#include "play_functions.h"
#include "common.h"

void cycle_mel_mute();
void cycle_drone_tromp_mute();
void cycle_drone_mute();
void cycle_tromp_mute();
void turn_volume_down();
void turn_volume_up();
void ex_cycle_capo(bool playing);
void ex_tpose_up(bool playing);
void ex_tpose_down(bool playing);
void ex_tpose_toggle(bool playing,int steps);

#endif
