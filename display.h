#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <U8g2lib.h>
#include "config.h"

// These are the Teensy pins wired up for the OLED.
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13

// Initiate (declare?  Not sure why this works in the header) the correct kind of
// display object based on OLED type
#ifdef WHITE_OLED
  extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
#endif
#ifdef BLUE_OLED
  extern U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2;
#endif

void start_display();
void print_menu_6(String title, String opt1, String opt2, String opt3, String opt4, String opt5, String opt6);
void print_menu_5(String title, String opt1, String opt2, String opt3, String opt4, String opt5);
void print_menu_4(String title, String opt1, String opt2, String opt3, String opt4);
void print_menu_3(String title, String opt1, String opt2, String opt3);
void print_menu_2(String title, String opt1, String opt2);

void print_message_2(String title, String opt1, String opt2);

void print_menu_4_nobk(String title, String opt1, String opt2, String opt3, String opt4);


#endif
