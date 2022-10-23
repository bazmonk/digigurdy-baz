#ifndef PLAY_SCREENS_H
#define PLAY_SCREENS_H

#include "display.h"

void draw_note(int note, int x_offset);
void draw_staff(int note, int x_offset);
void draw_play_screen(int note, int screen_type);
void printDisplay(int mel1, int mel2, int drone, int tromp, int tpose, int cap, int offset, bool hi_mute, bool lo_mute, bool drone_mute, bool tromp_mute);

#endif
