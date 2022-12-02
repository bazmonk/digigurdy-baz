#include "play_screens.h"

void draw_note(int note, int x_offset) {
  int octave = (note / 12 ) - 1;
  int note_pos = (note % 12);

  // Draw the letter
  u8g2.drawXBM(x_offset, 0, 64, 64, letter[note_pos]);

  if (octave >=0) {
    // If we're making a regular note...
    if (note_pos == 0 || note_pos == 2 || note_pos == 4 || note_pos == 5 ||
        note_pos == 7 || note_pos == 9 || note_pos == 11) {

      u8g2.drawXBM(x_offset + 52, 43, 16, 21, octave_num[octave]);
    // If we're making a sharp/flat...
    } else {
      u8g2.drawXBM(x_offset, 0, 64, 64, sharp_num[octave]);
    };
  };
};

void draw_staff(int note, int x_offset) {
  if (dot_pos[note] >= 0) {
    u8g2.drawXBM(x_offset, 0, 64, 64, staffs[staff_index[note]]);

    u8g2.drawXBM(x_offset + 29, dot_pos[note], 8, 7, dot);

    if (va_marker[note] == 2) {
      u8g2.drawXBM(x_offset + 42, 36, 24, 10, va15);
    } else if (va_marker[note] == 1) {
      u8g2.drawXBM(x_offset + 42, 36, 24, 10, va8);
    } else if (va_marker[note] == 3) {
      u8g2.drawXBM(x_offset + 42, 10, 24, 10, va8);
    } else if (va_marker[note] == 4) {
      u8g2.drawXBM(x_offset + 42, 10, 24, 10, va15);
    }
  };
};

void draw_play_screen(int note, int screen_type) {

  u8g2.clearBuffer();
  u8g2.setBitmapMode(1); // this lets you overlay bitmaps transparently

  if (screen_type == 0) {
    draw_note(note, 0);
    draw_staff(note, 64);
  } else {
    draw_note(note, 32);
  };

  u8g2.sendBuffer();
};

void print_display(int mel1, int mel2, int drone, int tromp, int tpose, int cap, int offset, bool hi_mute, bool lo_mute, bool drone_mute, bool tromp_mute) {

  // This whole thing could be written more clearly...

  String disp_str = "";

  disp_str = "\n Tpose: ";
  if (tpose > 0) {
    disp_str += "+";
  };
  disp_str = disp_str + tpose + "  Capo: ";
  if (cap > 0) {
    disp_str += "+";
  };
  disp_str = disp_str + cap + "\n\n";

  if (!hi_mute) {
    disp_str += "  Hi Melody: " + LongNoteNum[mel1 + tpose] + "\n";
  } else {
    disp_str += "  Hi Melody:   MUTE \n";
  };
  if (!lo_mute) {
    disp_str += " Low Melody: " + LongNoteNum[mel2 + tpose] + "\n\n";
  } else {
    disp_str += " Low Melody:   MUTE \n\n";
  };
  if (!tromp_mute) {
    disp_str += "  Trompette: " + LongNoteNum[tromp + tpose + cap] + "\n";
  } else {
    disp_str += "  Trompette:   MUTE \n";
  };
  if (!drone_mute) {
    disp_str += "      Drone: " + LongNoteNum[drone + tpose + cap] + "\n\n";
  } else {
    disp_str += "      Drone:   MUTE \n";
  };

  print_screen(disp_str);
};
