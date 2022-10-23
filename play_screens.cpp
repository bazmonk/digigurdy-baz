#include "play_screens.h"
#include "note_bitmaps.h"
#include "staff_bitmaps.h"

void draw_note(int note, int x_offset) {
  int octave = (note / 12 ) - 1;
  int note_pos = (note % 12);

  display.drawXBitmap(x_offset,0, letter[note_pos], 64, 64, 1);

  if (octave >=0) {
    // If we're making a regular note...
    if (note_pos == 0 || note_pos == 2 || note_pos == 4 || note_pos == 5 ||
        note_pos == 7 || note_pos == 9 || note_pos == 11) {

      display.drawXBitmap(x_offset + 52,43, octave_num[octave], 14, 21, 1);
    // If we're making a sharp/flat...
    } else {
      display.drawXBitmap(x_offset,0, sharp_num[octave], 64, 64, 1);
    };
  };

  display.display();

};

void draw_staff(int note, int x_offset) {
  if (dot_pos[note] >= 0) {
    display.drawXBitmap(x_offset,0, staffs[staff_index[note]], 64, 64, 1);

    display.drawXBitmap(x_offset + 29, dot_pos[note], dot, 7, 7, 1);

    if (va_marker[note] == 2) {
      display.drawXBitmap(x_offset + 42, 36, va15, 20, 10, 1);
    } else if (va_marker[note] == 1) {
      display.drawXBitmap(x_offset + 42, 36, va8, 20, 10, 1);
    } else if (va_marker[note] == 3) {
      display.drawXBitmap(x_offset + 42, 10, va8, 20, 10, 1);
    } else if (va_marker[note] == 4) {
      display.drawXBitmap(x_offset + 42, 10, va15, 20, 10, 1);
    }
  };
  display.display();
};

void draw_play_screen(int note, int screen_type) {
  display.clearDisplay();
  if (screen_type == 0) {
    draw_note(note, 0);
    draw_staff(note, 64);
  } else {
    draw_note(note, 32);
  };
};
