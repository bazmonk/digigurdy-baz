#include "play_screens.h"
#include "note_bitmaps.h"
#include "staff_bitmaps.h"
#include "notes.h"

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

void printDisplay(int mel1, int mel2, int drone, int tromp, int tpose, int cap, int offset, bool hi_mute, bool lo_mute, bool drone_mute, bool tromp_mute) {

  // This whole thing could be written more clearly...

  String disp_str0 = "";
  String disp_str = "";

  disp_str0 = "\n Tpose: ";
  disp_str = "\n\n";
  if (!hi_mute) {
    disp_str = disp_str + "  Hi Melody: " + String(LongNoteNum[mel1 + tpose]) + "\n";
  } else {
    disp_str = disp_str + "  Hi Melody:   MUTE \n";
  };
  if (!lo_mute) {
    disp_str = disp_str + " Low Melody: " + LongNoteNum[mel2 + tpose] + "\n\n";
  } else {
    disp_str = disp_str + " Low Melody:   MUTE \n\n";
  };
  if (!tromp_mute) {
    disp_str = disp_str + "  Trompette: " + LongNoteNum[tromp + tpose + cap] + "\n";
  } else {
    disp_str = disp_str + "  Trompette:   MUTE \n";
  };
  if (!drone_mute) {
    disp_str = disp_str + "      Drone: " + LongNoteNum[drone + tpose + cap] + "\n\n";
  } else {
    disp_str = disp_str + "      Drone:   MUTE \n";
  };

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print(disp_str0);

  // This is because the version of gcc Teensy uses has a bug with std::to_string(int)...
  // I need to print the tpose separately here because the display object will aceept it if it's not
  // concatenated with a string.
  if (tpose > 0) { display.print("+"); };
  display.print(tpose);

  display.print("  Capo: ");

  if (cap > 0) { display.print("+"); };
  display.print(cap);

  display.print(disp_str);
  display.display();
};
