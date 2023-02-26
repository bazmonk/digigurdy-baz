#ifndef EXFUNCTIONS_H
#define EXFUNCTIONS_H

#include "play_screens.h"
#include "play_functions.h"
#include "common.h"
#include "usb_power.h"
#include "bitmaps.h"

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
void ex_sec_out_toggle();
void ex_cycle_hi_mel_mute();
void ex_cycle_lo_mel_mute();

#endif
