#include "other_screens.h"

bool other_options_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " ---Other Options--- \n"
                      "                     \n"
                      " 1) Remove/Attach    \n"
                      "      Crank          \n\n"
                      " 2) About DigiGurdy  \n\n"
                      " X or 3) Go Back     \n";

    print_screen(disp_str);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      redetect_crank_screen();
      done = true;

    } else if (my2Button->wasPressed()) {
      options_about_screen();

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};

void redetect_crank_screen() {

  // In case the user has already tried removing it and it's freaking out, let's kill the sound.
  mystring->soundKill();
  mylowstring->soundKill();
  mykeyclick->soundKill();
  mytromp->soundKill();
  mydrone->soundKill();
  mybuzz->soundKill();

  bool done = false;
  while (!done) {

    String disp_str = "                     \n"
                      "                     \n"
                      " Remove/Attach Crank \n"
                      " And press 1...      \n";

    print_screen(disp_str);

    my1Button->update();

    if (my1Button->wasPressed()) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(" DigiGurdy");
      display.setTextSize(1);
      display.println(" --------------------");
      display.setTextSize(2);
      display.println("   Crank  ");

      if (mycrank->isDetected()) {
        display.println(" Detected ");
      } else {
        display.println("   Absent ");
      };

      display.display();
      delay(1000);

      done = true;
    };
  };
};

void options_about_screen() {

  about_screen();

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

  String disp_str = "      DigiGurdy      "
                    "---------------------"
                    "Special Thanks:      "
                    "                     "
                    "John Dingley         "
                    "David Jacobs         "
                    "lune36400            "
                    "SalusaSecondus       ";

  print_screen(disp_str);

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };
};
