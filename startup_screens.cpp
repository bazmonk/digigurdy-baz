#include "display.h"
#include "bitmaps.h"
#include "startup_screens.h"

void startup_screen_sqeuence() {

  // Clear the buffer.
  display.clearDisplay();

  // Intro animated sequence
  // display.clearDisplay();
  display.drawBitmap(0, 0, logos[0], 128, 64, 1);
  display.display();
  delay(800);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[1], 128, 64, 1);
  display.display();
  delay(110);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[2], 128, 64, 1);
  display.display();
  delay(110);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[3], 128, 64, 1);
  display.display();
  delay(110);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[4], 128, 64, 1);
  display.display();
  delay(110);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[5], 128, 64, 1);
  display.display();
  delay(110);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[6], 128, 64, 1);
  display.display();
  delay(110);
  display.clearDisplay();
  display.drawBitmap(0, 0, logos[7], 128, 64, 1);
  display.display();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(" DigiGurdy");
  display.setTextSize(1);
  display.println("---------------------");
  display.println("   By Basil Lalli,   ");
  display.println("Concept By J. Dingley");
  display.println("22 Oct 2022,  1.9.2 ");
  display.println("                     ");
  display.println("  shorturl.at/tuDY1  ");
  display.display();
  delay(600);
};
