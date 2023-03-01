#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include "config.h"
#include "notes.h"

/// @ingroup display
/// @{

#ifdef REV4_MODE
// These are the Teensy pins wired up for the OLED.
// 2.2.1 - changes for HW SPI
/// @brief The MOSI/DATA pin to the screen.
#define OLED_MOSI 43
/// @brief The CLK/SCL/SCK pin to the screen.
#define OLED_CLK 45
/// @brief The DC pin to the screen.
#define OLED_DC 46
/// @brief The CS pin to the screen.
#define OLED_CS 44
/// @brief The RESET pin to the screen.
#define OLED_RESET 47

#else
#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RESET 38

#endif

/// @}

#ifdef WHITE_OLED
  extern U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2;

  // This is just here for my reference/testing
  // when I want to see how bad SW SPI speed is.
  // extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
#endif
#ifdef BLUE_OLED
  #ifdef REV4_MODE
  extern U8G2_SH1106_128X64_NONAME_F_3RD_4W_HW_SPI u8g2;
  #else
  extern U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2;
  #endif
  //extern U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2;
#endif


void start_display();
void print_menu_6(String title, String opt1, String opt2, String opt3, String opt4, String opt5, String opt6);
void print_menu_5(String title, String opt1, String opt2, String opt3, String opt4, String opt5);
void print_menu_4(String title, String opt1, String opt2, String opt3, String opt4);
void print_menu_3(String title, String opt1, String opt2, String opt3);
void print_menu_2(String title, String opt1, String opt2);
void print_message_2(String title, String opt1, String opt2);
void print_message_3(String title, String opt1, String opt2, String opt3);
void print_menu_4_nobk(String title, String opt1, String opt2, String opt3, String opt4);
void print_pause_screen(String d_string, String t_string, String h_string, String l_string);
void print_confirm_screen(String title, String msg1, String msg2, String opt1);
void print_tuning(String title, String hi_note, String low_note, String drone_note, String tromp_note, String tpose_str, String capo_str);
void print_tuning_summary(int hi, int lo, int tromp, int drone);
void print_tuning_choice_3(String title, int opt1, int opt2, int opt3);
void print_value_selection(String title, String value);
void draw_xbm(const uint8_t *bitmap);

#endif
