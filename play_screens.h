#ifndef PLAY_SCREENS_H
#define PLAY_SCREENS_H

#include "display.h"
#include "note_bitmaps.h"
#include "staff_bitmaps.h"
#include "notes.h"

// true = G/C tuning, false = D/G.  For the menus.
extern bool gc_or_dg;

void draw_note(int note, int x_offset);
void draw_staff(int note, int x_offset);
void print_note(String note_str, int x_offset);
void draw_play_screen(int note, int screen_type, bool draw_buzz);
void print_display(int mel1, int mel2, int drone, int tromp, int tpose, int cap, int offset, bool hi_mute, bool lo_mute, bool drone_mute, bool tromp_mute);

#endif
