#include "startup_screens.h"

/// @ingroup display
/// @{

/// @brief Draws the about screen.
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

  u8g2.drawStr(64 - (u8g2.getStrWidth("Hardware by John Dingley") / 2), 28, "Hardware by John Dingley");
  u8g2.drawStr(64 - (u8g2.getStrWidth("Software by Basil Lalli") / 2), 37, "Software by Basil Lalli");
  u8g2.drawStr(64 - (u8g2.getStrWidth(REL_DATE.c_str()) / 2), 46, REL_DATE.c_str());
  u8g2.drawStr(64 - (u8g2.getStrWidth("https://digigurdy.com") / 2), 55, "https://digigurdy.com");
  u8g2.drawStr(64 - (u8g2.getStrWidth(EXTRA_LINE.c_str()) / 2), 64, EXTRA_LINE.c_str());

  u8g2.sendBuffer();
};

/// @brief Draws the startup animation sequence.
void startup_screen_sequence() {

  for (int x = 0; x < 11; x++) {
    // Clear the buffer.
    draw_xbm(logos[x]);
    if (x == 0) {
      delay(140);
    } else if (x > 9) {
      delay(750);
    } else {
      delay(140);
    };
  };
  // Display the about screen
  about_screen();
  delay(2000);
};

/// @}
