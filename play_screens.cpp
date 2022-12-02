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

  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  String tpose_str = "Transpose: ";
  if (tpose > 0) {
    tpose_str += "+";
  };
  tpose_str = tpose_str + tpose;

  String capo_str = "Capo: ";
  if (cap > 0) {
    capo_str += "+";
  };
  capo_str = capo_str + cap;

  u8g2.drawStr(48 - (u8g2.getStrWidth("Transpose:")), 8, tpose_str.c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth("Capo:")), 8, capo_str.c_str());

  u8g2.drawHLine(0, 12, 128);
  u8g2.drawHLine(0, 13, 128);

  u8g2.drawStr(64 - u8g2.getStrWidth("Hi Melody:"), 24, "Hi Melody:");

  u8g2.drawHLine(8, 26, 110);

  u8g2.drawStr(64 - u8g2.getStrWidth("Low Melody:"), 35, "Low Melody:");

  u8g2.drawHLine(0, 39, 128);
  u8g2.drawHLine(0, 40, 128);

  u8g2.drawStr(64 - u8g2.getStrWidth("Trompette:"), 51, "Trompette:");

  u8g2.drawHLine(8, 55, 110);

  u8g2.drawStr(64 - u8g2.getStrWidth("Drone:"), 64, "Drone:");

  if (!hi_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(LongNoteNum[mel1 + tpose].c_str()) / 2), 24, LongNoteNum[mel1 + tpose].c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 24, "MUTE");
  };

  if (!lo_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(LongNoteNum[mel2 + tpose].c_str()) / 2), 35, LongNoteNum[mel2 + tpose].c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 35, "MUTE");
  };

  if (!tromp_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(LongNoteNum[tromp + tpose + cap].c_str()) / 2), 51, LongNoteNum[tromp + tpose + cap].c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 51, "MUTE");
  };

  if (!drone_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(LongNoteNum[drone + tpose + cap].c_str()) / 2), 64, LongNoteNum[drone + tpose + cap].c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 64, "MUTE");
  };

  u8g2.sendBuffer();
};
