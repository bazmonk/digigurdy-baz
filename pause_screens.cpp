#include "pause_screens.h"

// This is the screen that X+A (or an ex button) gets you.
void pause_screen() {
  Serial.println("In the pause screen");
  bool done = false;
  while (!done) {

    String disp_str = " ----Pause  Menu---- \n"
                      " 1) Load    2) Save  \n"
                      " 3) Tuning  4) Other \n\n"
                      " X, 5 or ex1) Go Back\n\n";

    if (drone_mode == 0) {
      disp_str += "A) Drone:On ,Trmp:On \n";
    } else if (drone_mode == 1) {
      disp_str += "A) Drone:Off,Trmp:Off\n";
    } else if (drone_mode == 2) {
      disp_str += "A) Drone:On, Trmp:Off\n";
    } else if (drone_mode == 3) {
      disp_str += "A) Drone:Off,Trmp:On \n";
    };

    if (mel_mode == 0) {
      disp_str += "B) High:On ,  Low:On \n";
    } else if (mel_mode == 1) {
      disp_str += "B) High:On ,  Low:Off\n";
    } else if (mel_mode == 2) {
      disp_str += "B) High:Off,  Low:On \n";
    };

    print_screen(disp_str);
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();
    myAButton->update();
    myBButton->update();
    ex1Button->update();

    if (my1Button->wasPressed()) {
      if (load_tuning_screen()) {
        done = true;
      };

    } else if (my2Button->wasPressed()) {
      save_tuning_screen();
      done = true;

    } else if (my3Button->wasPressed()) {
      if (tuning()) {
        done = true;
      };

    } else if (my4Button->wasPressed()) {
      if (other_options_screen()) {
        done = true;
      };

    } else if (my5Button->wasPressed() || myXButton->wasPressed() || ex1Button->wasPressed()) {
      done = true;

    } else if (myAButton->wasPressed()) {
      if (drone_mode == 0) {
        drone_mode = 1; // 1 == both off
        mydrone->setMute(true);
        mytromp->setMute(true);
      } else if (drone_mode == 1) {
        drone_mode = 2; // 2 == drone on, tromp off
        mydrone->setMute(false);
        mytromp->setMute(true);
      } else if (drone_mode == 2) {
        drone_mode = 3; // 3 == drone off, tromp on
        mydrone->setMute(true);
        mytromp->setMute(false);
      } else if (drone_mode == 3) {
        drone_mode = 0; // 0 == both on
        mydrone->setMute(false);
        mytromp->setMute(false);
      };
    } else if (myBButton->wasPressed()) {
      if (mel_mode == 0) {
        mel_mode = 1; // 1 == high on, low off
        mystring->setMute(false);
        mylowstring->setMute(true);
      } else if (mel_mode == 1) {
        mel_mode = 2; // 2 == high off, low on
        mystring->setMute(true);
        mylowstring->setMute(false);
      } else if (mel_mode == 2) {
        mel_mode = 0; // 0 == high on, low on
        mystring->setMute(false);
        mylowstring->setMute(false);
      };
    };
  };

  // Crank On! for half a sec.
  display.clearDisplay();
  display.drawBitmap(0, 0, crank_on_logo, 128, 64, 1);
  display.display();
  delay(500);
};
