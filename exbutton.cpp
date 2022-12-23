#include "exbutton.h"

ExButton::ExButton(int my_pin, int func, int interval) : GurdyButton(my_pin, interval) {
  my_func = func;
};

int ExButton::getFunc() {
  return my_func;
};

/// @brief Set the function to be called when this button is pressed.
/// @param func 
/// The function to be called.  See ExButton::doFunc() to determine the internal numbering of the functions.
void ExButton::setFunc(int func) {
  my_func = func;
};

void ExButton::doFunc() {
  if (my_func == 1) {
    return;
  } else if (my_func == 2) {
    cycle_mel_mute();
  } else if (my_func == 3) {
    cycle_drone_tromp_mute();
  } else if (my_func == 4) {
    cycle_drone_mute();
  } else if (my_func == 5) {
    cycle_tromp_mute();
  }
  return;
};

// Trying to put this as a function with the menu screens was a big problem because
// this class calls those functions and there was a circular definition problem.
void ExButton::fn_choice_screen(int but_num) {

  bool done = false;
  while (!done) {

    print_menu_5("Ex Button Func.", "Open Pause Menu", "Cycle Mel. Mute", "Cycle Dn/Tr. Mute",
                 "Toggle Drone Mute", "Toggle Trompette Mute");
    delay(200);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      setFunc(1);
      // this is ugly, I know...
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 1); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 1); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 1); };

      done = true;
    } else if (my2Button->wasPressed()) {
      setFunc(2);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 2); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 2); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 2); };

      done = true;
    } else if (my3Button->wasPressed()) {
      setFunc(3);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 3); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 3); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 3); };

      done = true;
    } else if (my4Button->wasPressed()) {
      setFunc(4);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 4); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 4); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 4); };

      done = true;
    } else if (my5Button->wasPressed()) {
      setFunc(5);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 5); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 5); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 5); };

      done = true;
    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
