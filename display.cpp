#include "display.h"

// The first argument here (U8G2_R0) determines the orientation, FYI.  Useful if you put one in
// upside-down.
#ifdef WHITE_OLED
  U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);
#endif
#ifdef BLUE_OLED
  U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);
#endif

void start_display() {
  u8g2.begin();
};

void print_menu_6(String title, String opt1, String opt2, String opt3, String opt4, String opt5, String opt6) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 16, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 24, "2) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("3) "), 32, "3) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("4) "), 40, "4) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("5) "), 48, "5) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("6) "), 56, "6) ");
  u8g2.drawStr(64 - (u8g2.getStrWidth("X) Go Back") / 2), 64, "X) Go Back");

  // Draw all six options
  u8g2.drawStr(13, 16, opt1.c_str());
  u8g2.drawStr(13, 24, opt2.c_str());
  u8g2.drawStr(13, 32, opt3.c_str());
  u8g2.drawStr(13, 40, opt4.c_str());
  u8g2.drawStr(13, 48, opt5.c_str());
  u8g2.drawStr(13, 56, opt6.c_str());

  u8g2.sendBuffer();
};

void print_menu_5(String title, String opt1, String opt2, String opt3, String opt4, String opt5) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 19, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 28, "2) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("3) "), 37, "3) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("4) "), 46, "4) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("5) "), 55, "5) ");
  u8g2.drawStr(64 - (u8g2.getStrWidth("6 or X) Go Back") / 2), 64, "6 or X) Go Back");

  // Draw all six options
  u8g2.drawStr(13, 19, opt1.c_str());
  u8g2.drawStr(13, 28, opt2.c_str());
  u8g2.drawStr(13, 37, opt3.c_str());
  u8g2.drawStr(13, 46, opt4.c_str());
  u8g2.drawStr(13, 55, opt5.c_str());

  u8g2.sendBuffer();
};

void print_menu_4(String title, String opt1, String opt2, String opt3, String opt4) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 20, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 31, "2) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("3) "), 42, "3) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("4) "), 53, "4) ");
  u8g2.drawStr(64 - (u8g2.getStrWidth("5 or X) Go Back") / 2), 64, "5 or X) Go Back");

  // Draw all six options
  u8g2.drawStr(13, 20, opt1.c_str());
  u8g2.drawStr(13, 31, opt2.c_str());
  u8g2.drawStr(13, 42, opt3.c_str());
  u8g2.drawStr(13, 53, opt4.c_str());

  u8g2.sendBuffer();
};

void print_menu_3(String title, String opt1, String opt2, String opt3) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 20, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 34, "2) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("3) "), 48, "3) ");
  u8g2.drawStr(64 - (u8g2.getStrWidth("4 or X) Go Back") / 2), 64, "4 or X) Go Back");

  // Draw all six options
  u8g2.drawStr(13, 20, opt1.c_str());
  u8g2.drawStr(13, 34, opt2.c_str());
  u8g2.drawStr(13, 48, opt3.c_str());

  u8g2.sendBuffer();
};

void print_menu_2(String title, String opt1, String opt2) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 24, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 44, "2) ");
  u8g2.drawStr(64 - (u8g2.getStrWidth("3 or X) Go Back") / 2), 64, "3 or X) Go Back");

  // Draw all six options
  u8g2.drawStr(13, 24, opt1.c_str());
  u8g2.drawStr(13, 44, opt2.c_str());

  u8g2.sendBuffer();
};

void print_message_2(String title, String opt1, String opt2) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  u8g2.drawStr(64 - (u8g2.getStrWidth(opt1.c_str()) / 2), 34, opt1.c_str());
  u8g2.drawStr(64 - (u8g2.getStrWidth(opt2.c_str()) / 2), 54, opt2.c_str());

  u8g2.sendBuffer();
};

void print_menu_4_nobk(String title, String opt1, String opt2, String opt3, String opt4) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 20, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 34, "2) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("3) "), 48, "3) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("4) "), 62, "4) ");

  // Draw all six options
  u8g2.drawStr(13, 20, opt1.c_str());
  u8g2.drawStr(13, 34, opt2.c_str());
  u8g2.drawStr(13, 48, opt3.c_str());
  u8g2.drawStr(13, 62, opt4.c_str());


  u8g2.sendBuffer();
};

void print_pause_screen(String d_string, String t_string, String h_string, String l_string) {
  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  String title = "Pause Menu";

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  u8g2.drawStr(25 - u8g2.getStrWidth("1) "), 18, "1) Load");
  u8g2.drawStr(89 - u8g2.getStrWidth("2) "), 18, "2) Save");
  u8g2.drawStr(25 - u8g2.getStrWidth("3) "), 28, "3) Tuning");
  u8g2.drawStr(89 - u8g2.getStrWidth("4) "), 28, "4) Other");
  u8g2.drawStr(64 - (u8g2.getStrWidth("5 or X) Go Back") / 2), 39, "5 or X) Go Back");

  // Divider line between mute options and menu choices
  u8g2.drawHLine(0, 42, 128);
  u8g2.drawHLine(0, 41, 128);

  String a_string = String("A) Drone: ") + d_string + ", Tromp: " + t_string;
  String b_string = String("B) High: ") + h_string + ", Low: " + l_string;

  u8g2.drawStr(64 - (u8g2.getStrWidth(a_string.c_str()) / 2), 52, a_string.c_str());
  u8g2.drawStr(64 - (u8g2.getStrWidth(b_string.c_str()) / 2), 62, b_string.c_str());

  u8g2.sendBuffer();
};

void print_confirm_screen(String title, String msg1, String msg2, String opt1) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  u8g2.drawStr(64 - (u8g2.getStrWidth(msg1.c_str()) / 2), 20, msg1.c_str());
  u8g2.drawStr(64 - (u8g2.getStrWidth(msg2.c_str()) / 2), 30, msg2.c_str());

  String this_line = "1) " + opt1;

  u8g2.drawStr(64 - (u8g2.getStrWidth(this_line.c_str()) / 2), 46, this_line.c_str());

  u8g2.drawStr(64 - (u8g2.getStrWidth("2 or X) Go Back") / 2), 60, "2 or X) Go Back");

  u8g2.sendBuffer();
};

void print_tuning(String title, String hi_note, String low_note, String drone_note, String tromp_note, String tpose_str, String capo_str) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six options
  u8g2.drawStr(64 - u8g2.getStrWidth("Hi Melody: "), 18, "Hi Melody: ");
  u8g2.drawStr(64 - u8g2.getStrWidth("Lo Melody: "), 27, "Lo Melody: ");
  u8g2.drawStr(64 - u8g2.getStrWidth("Trompette: "), 36, "Trompette: ");
  u8g2.drawStr(64 - u8g2.getStrWidth("Drone: "), 45, "Drone: ");
  u8g2.drawStr(44 - u8g2.getStrWidth("Tpose: "), 54, "Tpose: ");
  u8g2.drawStr(105 - u8g2.getStrWidth("Capo: "), 54, "Capo: ");

  u8g2.drawStr(96 - (u8g2.getStrWidth(hi_note.c_str()) / 2), 18, hi_note.c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth(low_note.c_str()) / 2), 27, low_note.c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth(drone_note.c_str()) / 2), 36, tromp_note.c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth(tromp_note.c_str()) / 2), 45, drone_note.c_str());
  u8g2.drawStr(44, 54, tpose_str.c_str());
  u8g2.drawStr(105, 54, capo_str.c_str());

  u8g2.drawStr(64 - (u8g2.getStrWidth("1) OK  2 or X) Go Back") / 2), 64, "1) OK  2 or X) Go Back");

  u8g2.sendBuffer();
};

void print_tuning_summary(int hi, int lo, int tromp, int drone) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 62 - (u8g2.getStrWidth("Summary") / 2));
  u8g2.drawHLine(0, 4, 62 - (u8g2.getStrWidth("Summary") / 2));
  u8g2.drawHLine(0, 6, 62 - (u8g2.getStrWidth("Summary") / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth("Summary") / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth("Summary") / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth("Summary") / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth("Summary") / 2), 8, "Summary");

  // Draw all six numbers
  u8g2.drawStr(64 - u8g2.getStrWidth("Hi Melody:"), 19, "Hi Melody:");
  u8g2.drawStr(64 - u8g2.getStrWidth("Low Melody:"), 27, "Low Melody:");
  u8g2.drawStr(64 - u8g2.getStrWidth("Trompette:"), 35, "Trompette:");
  u8g2.drawStr(64 - u8g2.getStrWidth("Drone:"), 43, "Drone:");

  u8g2.drawStr(64 - (u8g2.getStrWidth("1 or A) Accept") / 2), 55, "1 or A) Accept");
  u8g2.drawStr(64 - (u8g2.getStrWidth("2 or X) Go Back") / 2), 64, "2 or X) Go Back");

  u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(hi).c_str()) / 2), 19, getLongNoteNum(hi).c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(lo).c_str()) / 2), 27, getLongNoteNum(lo).c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(tromp).c_str()) / 2), 35, getLongNoteNum(tromp).c_str());
  u8g2.drawStr(96 - (u8g2.getStrWidth(getLongNoteNum(drone).c_str()) / 2), 43, getLongNoteNum(drone).c_str());

  u8g2.sendBuffer();
};

void print_tuning_choice_3(String title, int opt1, int opt2, int opt3) {

  // Clear the screen, set overlay font mode (don't draw background)
  // FontMode 1 requires a t* font
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());


  u8g2.drawStr(25 - u8g2.getStrWidth("**1) "), 20, "**1) ");
  u8g2.drawStr(25 - u8g2.getStrWidth("2) "), 34, "2) ");
  u8g2.drawStr(25 - u8g2.getStrWidth("3) "), 48, "3) ");

  u8g2.drawStr(64 - (u8g2.getStrWidth("A) Default (**)") / 2), 64, "A) Default (**)");

  u8g2.drawStr(25, 20, getLongNoteNum(opt1).c_str());
  u8g2.drawStr(25, 34, getLongNoteNum(opt2).c_str());
  u8g2.drawStr(25, 48, getLongNoteNum(opt3).c_str());

  u8g2.sendBuffer();
};

void print_value_selection(String title, String value) {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_finderskeepers_tf);

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 2, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 4, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(0, 6, 61 - (u8g2.getStrWidth(title.c_str()) / 2));
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 2, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 4, 64);
  u8g2.drawHLine(67 + (u8g2.getStrWidth(title.c_str()) / 2), 6, 64);

  // Print the title centered on the top "line"
  u8g2.drawStr(64 - (u8g2.getStrWidth(title.c_str()) / 2), 8, title.c_str());

  // Draw all six numbers
  u8g2.drawStr(15 - u8g2.getStrWidth("1) "), 20, "1) ");
  u8g2.drawStr(15 - u8g2.getStrWidth("2) "), 34, "2) ");
  u8g2.drawStr(64 - (u8g2.getStrWidth("X) Done / Go Back") / 2), 64, "X) Done / Go Back");

  u8g2.drawStr(13, 20, "Down");
  u8g2.drawStr(13, 34, "Up");

  u8g2.setFont(u8g2_font_elispe_tr);

  u8g2.drawStr(64 - (u8g2.getStrWidth(value.c_str()) / 2), 49, value.c_str());

  u8g2.sendBuffer();
};
