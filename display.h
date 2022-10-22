#ifndef DISPLAY_H
#define DISPLAY_H

// The white OLED uses Adafruit SSD1306.  Blue uses SH1106.
#ifdef WHITE_OLED
  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64
  #include <Adafruit_SSD1306.h>
#endif
#ifdef BLUE_OLED
  #include <Adafruit_SH1106.h>
#endif

// These are the Teensy pins wired up for the OLED.
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13

// Initiate (declare?  Not sure why this works in the header) the correct kind of
// display object based on OLED type
#ifdef WHITE_OLED
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif
#ifdef BLUE_OLED
  Adafruit_SH1106 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif

#endif
