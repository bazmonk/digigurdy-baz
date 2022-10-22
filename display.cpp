#include "display.h"

#ifdef WHITE_OLED
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif
#ifdef BLUE_OLED
  Adafruit_SH1106 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif

void start_display() {
  #ifdef WHITE_OLED
    display.begin(SSD1306_SWITCHCAPVCC);
  #endif
  #ifdef BLUE_OLED
    display.begin(SH1106_SWITCHCAPVCC);
  #endif
};
