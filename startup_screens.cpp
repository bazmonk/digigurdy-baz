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

  // Display the about screen
  about_screen();
  delay(600);
};

void about_screen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(" DigiGurdy");
  display.setTextSize(1);

  String disp_str = "---------------------\n"
                    "   By Basil Lalli,   \n"
                    "Concept By J. Dingley\n"
                    + REL_DATE + "\n"
                    "                     \n"
                    "  shorturl.at/tuDY1  \n";

  display.println(disp_str);
  display.display();
};

// This is the first screen after the credits n' stuff.
void welcome_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " -----DigiGurdy----- \n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) Load Preset      \n"
                      " 2) Load Save Slot   \n"
                      " 3) New Tuning Setup \n"
                      " 4) Other Options    \n"
                      "                     \n";

    print_screen(disp_str);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();

    if (my1Button->wasPressed()) {
      if (load_preset_screen()) {done = true;};

    } else if (my2Button->wasPressed()) {
      if (load_saved_screen()) {done = true;};

    } else if (my3Button->wasPressed()) {
      if (tuning()) {done = true;};

    } else if (my4Button->wasPressed()) {
      options_screen();
      // Not done = true here, we'd want to get prompted again.
    };
  };
};

// This screen is for other setup options.  Currently, that's
// just an option to clear the EEPROM.
void options_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " ------Options------ \n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) Clear EEPROM     \n"
                      " 2) Playing Screen   \n"
                      " 3) Scene Control    \n"
                      "                     \n"
                      " X) Go Back          \n";

    print_screen(disp_str);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      clear_eeprom();

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      String disp_str = ""
      "  EEPROM  \n"
      "  \n"
      "  CLEARED \n";

      display.print(disp_str);
      display.display();
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {
      playing_options_screen();
      done = true;
    } else if (my3Button->wasPressed()) {
      scene_options_screen();
      done = true;
    } else if (myXButton->wasPressed()) {
      done = true;
    };
  };
};

// This clears the EEPROM and overwrites it all with zeroes
void clear_eeprom() {
  // Not much to say here... write 0 everywhere:
  for (int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);
};

// This screen lets you choose how the gurdy indicates the turning to the controller
void scene_options_screen() {
  bool done = false;
  while (!done) {

    String disp_str = " -Scene Signalliing- \n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) None             \n"
                      " 2) Prog Chg, Ch. 1  \n"
                      "                     \n"
                      " X) Go Back          \n";

    print_screen(disp_str);

    // Check the buttons
    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      scene_signal_type = 0;
      EEPROM.write(EEPROM_SCENE_SIGNALLING, scene_signal_type);
      done = true;
    } else if (my2Button->wasPressed()) {
      scene_signal_type = 1;
      EEPROM.write(EEPROM_SCENE_SIGNALLING, scene_signal_type);
      done = true;
    } else if (myXButton->wasPressed()) {
      done = true;
    }
  }
}

// This screen lets you choose what kind of display you want.
void playing_options_screen() {
  bool done = false;
  while (!done) {

    String disp_str = " -Choose Play Screen-\n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) Big Note + Staff \n"
                      "                     \n"
                      " 2) Big Note         \n"
                      "                     \n"
                      " X) Go Back          \n";

    print_screen(disp_str);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      play_screen_type = 0;
      EEPROM.write(EEPROM_DISPLY_TYPE, 0);

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      String disp_str = ""
      "  DISPLAY  \n"
      "  \n"
      "  SAVED \n";

      display.print(disp_str);
      display.display();
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {

      play_screen_type = 1;
      EEPROM.write(EEPROM_DISPLY_TYPE, 1);

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      String disp_str = ""
      "  DISPLAY  \n"
      "  \n"
      "   SAVED  \n";

      display.print(disp_str);
      display.display();
      delay(750);
      done = true;
    } else if (myXButton->wasPressed()) {
      done = true;
    };
  };
};
