#include "startup_screens.h"

void about_screen() {

  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_elispe_tr);

  u8g2.drawStr(64 - (u8g2.getStrWidth("Digi-Gurdy") / 2), 14, "Digi-Gurdy");

  // Print a pretty 3-stripe line "around" the title
  u8g2.drawHLine(0, 3, 62 - (u8g2.getStrWidth("Digi-Gurdy") / 2));
  u8g2.drawHLine(0, 4, 62 - (u8g2.getStrWidth("Digi-Gurdy") / 2));
  u8g2.drawHLine(0, 7, 62 - (u8g2.getStrWidth("Digi-Gurdy") / 2));
  u8g2.drawHLine(0, 8, 62 - (u8g2.getStrWidth("Digi-Gurdy") / 2));
  u8g2.drawHLine(0, 11, 62 - (u8g2.getStrWidth("Digi-Gurdy") / 2));
  u8g2.drawHLine(0, 12, 62 - (u8g2.getStrWidth("Digi-Gurdy") / 2));

  u8g2.drawHLine(66 + (u8g2.getStrWidth("Digi-Gurdy") / 2), 3, 64);
  u8g2.drawHLine(66 + (u8g2.getStrWidth("Digi-Gurdy") / 2), 4, 64);
  u8g2.drawHLine(66 + (u8g2.getStrWidth("Digi-Gurdy") / 2), 7, 64);
  u8g2.drawHLine(66 + (u8g2.getStrWidth("Digi-Gurdy") / 2), 8, 64);
  u8g2.drawHLine(66 + (u8g2.getStrWidth("Digi-Gurdy") / 2), 11, 64);
  u8g2.drawHLine(66 + (u8g2.getStrWidth("Digi-Gurdy") / 2), 12, 64);

  u8g2.setFont(u8g2_font_finderskeepers_tf);

  u8g2.drawStr(64 - (u8g2.getStrWidth("By Basil Lalli") / 2), 28, "By Basil Lalli");
  u8g2.drawStr(64 - (u8g2.getStrWidth("Concept By John Dingley") / 2), 37, "Concept By John Dingley");
  u8g2.drawStr(64 - (u8g2.getStrWidth(REL_DATE.c_str()) / 2), 46, REL_DATE.c_str());
  u8g2.drawStr(64 - (u8g2.getStrWidth(EXTRA_LINE.c_str()) / 2), 55, EXTRA_LINE.c_str());
  u8g2.drawStr(64 - (u8g2.getStrWidth("shorturl.at/tuDY1") / 2), 64, "shorturl.at/tuDY1");

  u8g2.sendBuffer();
};

void startup_screen_sequence() {

  for (int x = 0; x < 8; x++) {
    // Clear the buffer.
    u8g2.clearBuffer();
    u8g2.drawBitmap(0, 0, 16, 64, logos[x]);
    u8g2.sendBuffer();
    if (x == 0) {
      delay(800);
    } else if (x > 6) {
      delay(500);
    } else {
      delay(120);
    };
  };
  // Display the about screen
  about_screen();
  delay(2500);
};
