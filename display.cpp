#include "display.h"

#ifdef WHITE_OLED
  U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, 10, 9, 12, 11, 13);
#endif
#ifdef BLUE_OLED
  U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, 10, 9, 12, 11, 13);
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
  u8g2.drawStr(64 - (u8g2.getStrWidth("7 or X) Go Back") / 2), 64, "7 or X) Go Back");

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
