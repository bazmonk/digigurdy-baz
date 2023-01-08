#include "play_screens.h"

/// @defgroup play Play Screens and Functions
/// These functions draw the screens shown during play and outside of the menu tree, and help manage play behavior.
/// @{

/// @brief Draws a 64x64 ABC-style bitmap of the note being played.
/// @param note 0-127, the MIDI note to be displayed
/// @param x_offset 0-64, the x-offset to display the image.  0 = far left, 32 = centered, 64 = far right
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

/// @brief Draws a staff bitmap and the given note on it.
/// @param note 0-127, the MIDI note to be drawn.
/// @param x_offset 0-64, the x-offset to display the image.  0 = far left, 32 = centered, 64 = far right
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

/// @brief Print as text the note being played in a large font.
/// @param note_str The *text* of the note to be displayed.
/// @param x_offset 0-64, the x-offset to display the text.  0 = far left, 32 = centered, 64 = far right
/// @warning This function will only correctly display text matching the "LongNoteNum" text pattern.  See `notes.h`, `notes.cpp`.
void print_note(String note_str, int x_offset) {

  String note = "";
  String acc = "";
  String oct = "";
  String first_str = "";
  String second_str = "";

  //u8g2.setFont(u8g2_font_elispe_tr);
  //u8g2.setFont(u8g2_font_crox4hb_tf);
  u8g2.setFont(u8g2_font_timB14_tf);
  u8g2.setFontMode(1);

  int slash_idx = note_str.indexOf("/");
  if (slash_idx != -1) {
    first_str = note_str.substring(0, slash_idx);
    second_str = note_str.substring(slash_idx + 1);
  } else {
    first_str = note_str;
  };

  if (first_str.indexOf("b") != -1) {
    int acc_idx = first_str.indexOf("b");
    acc = "b";
    note = first_str.substring(0, acc_idx);
    oct = first_str.substring(acc_idx + 1);

  } else if (first_str.indexOf("#") != -1) {
    int acc_idx = first_str.indexOf("#");
    acc = "#";
    note = first_str.substring(0, acc_idx);
    oct = first_str.substring(acc_idx + 1);
  } else {
    oct = first_str.substring(first_str.length() - 1);
    note = first_str.substring(0, first_str.length() - 1);
  };

  if (second_str.length() == 0) {

    u8g2.drawStr(x_offset + 26 - (u8g2.getStrWidth(String(note + oct).c_str()) / 2), 38, note.c_str());
    u8g2.drawStr(x_offset + 24 + (u8g2.getStrWidth(note.c_str()) / 2), 30, acc.c_str());
    u8g2.drawStr(x_offset + 24 + (u8g2.getStrWidth(note.c_str()) / 2), 46, oct.c_str());

  } else {

    u8g2.drawStr(x_offset + 26 - (u8g2.getStrWidth(String(note + oct).c_str()) / 2), 20, note.c_str());
    u8g2.drawStr(x_offset + 24 + (u8g2.getStrWidth(note.c_str()) / 2), 12, acc.c_str());
    u8g2.drawStr(x_offset + 24 + (u8g2.getStrWidth(note.c_str()) / 2), 28, oct.c_str());

    if (second_str.indexOf("b") != -1) {
      int acc_idx = second_str.indexOf("b");
      acc = "b";
      note = second_str.substring(0, acc_idx);
      oct = second_str.substring(acc_idx + 1);

    } else if (second_str.indexOf("#") != -1) {
      int acc_idx = second_str.indexOf("#");
      acc = "#";
      note = second_str.substring(0, acc_idx);
      oct = second_str.substring(acc_idx + 1);
    } else {
      oct = second_str.substring(second_str.length() - 1);
      note = second_str.substring(0, second_str.length() - 1);
    };

    u8g2.drawStr(x_offset + 26 - (u8g2.getStrWidth(String(note + oct).c_str()) / 2), 56, note.c_str());
    u8g2.drawStr(x_offset + 24 + (u8g2.getStrWidth(note.c_str()) / 2), 48, acc.c_str());
    u8g2.drawStr(x_offset + 24 + (u8g2.getStrWidth(note.c_str()) / 2), 64, oct.c_str());
  };
};

/// @brief Draws the play screen in one of six ways depending on the user's chosen screen type.
/// @param note 0-127, the MIDI note to be drawn.
/// @param screen_type The arrangement to display.  See the code itself to know what they mean: this is a magic number.
/// @param draw_buzz If true, draw the on-screen buzz indicator.
void draw_play_screen(int note, int screen_type, bool draw_buzz) {

  u8g2.clearBuffer();
  u8g2.setBitmapMode(1); // this lets you overlay bitmaps transparently

  if (draw_buzz && screen_type / 10 == 1) {
    u8g2.drawHLine(0, 0, 128);
    u8g2.drawHLine(0, 63, 128);
    u8g2.drawVLine(0, 0, 64);
    u8g2.drawVLine(127, 0, 64);
  };

  if (screen_type % 10 == 0) {
    draw_note(note, 0);
    draw_staff(note, 64);
  } else if (screen_type % 10 == 1) {
    print_note(getLongNoteNum(note), 0);
    draw_staff(note, 64);
  } else if (screen_type % 10 == 2) {
    draw_note(note, 32);
  } else if (screen_type % 10 == 3) {
    print_note(getLongNoteNum(note), 32);
  } else if (screen_type % 10 == 4) {
    draw_staff(note, 32);
  } else {
    ;
  };

  u8g2.sendBuffer();
};

/// @brief Draws the screen visible when sound is not being made, the "heads-up display" screen.
/// @param mel1 The high melody MIDI note, 0-127
/// @param mel2 The low melody MIDI note, 0-127
/// @param drone The drone MIDI note, 0-127
/// @param tromp The trompette MIDI note, 0-127
/// @param tpose The transpose value, -12 to +12
/// @param cap The capo value, 0-4
/// @param offset The note offset (*Currently unused*)
/// @param hi_mute True if high melody is muted
/// @param lo_mute True if low melody is muted
/// @param drone_mute True if drone is muted
/// @param tromp_mute True if trompette is muted
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
    u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(mel1 + tpose).c_str()) / 2), 24, getLongNoteNum(mel1 + tpose).c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 24, "MUTE");
  };

  if (!lo_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(mel2 + tpose).c_str()) / 2), 35, getLongNoteNum(mel2 + tpose).c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 35, "MUTE");
  };

  if (!tromp_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(tromp + tpose + cap).c_str()) / 2), 51, getLongNoteNum(tromp + tpose + cap).c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 51, "MUTE");
  };

  if (!drone_mute) {
    u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(drone + tpose + cap).c_str()) / 2), 64, getLongNoteNum(drone + tpose + cap).c_str());
  } else {
    u8g2.drawStr(96 - (u8g2.getStrWidth("MUTE") / 2), 64, "MUTE");
  };

  u8g2.sendBuffer();
};

/// @}