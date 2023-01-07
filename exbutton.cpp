#include "exbutton.h"

/// @brief Constructor for ExButton
/// @param my_pin The digital pin this button is connected to
/// @param func The beginning function for this button (See ExButton::doFunc for the numbering)
/// @param interval The debounce interval for this button in milliseconds
ExButton::ExButton(int my_pin, int func, int interval) : GurdyButton(my_pin, interval) {
  my_func = func;
};

/// @brief Return the current function number of the button
/// @return the current function number
int ExButton::getFunc() {
  return my_func;
};

/// @brief Set the function to be called when this button is pressed.
/// @param func The function to be called.  See ExButton::doFunc() to determine the internal numbering of the functions.
void ExButton::setFunc(int func) {
  my_func = func;
};

/// @brief Execute the button's configured fucntion
void ExButton::doFunc(bool playing) {

  Serial.print("I was clicked for function: ");
  Serial.println(my_func);

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
  } else if (my_func == 6) {
    turn_volume_down();
  } else if (my_func == 7) {
    turn_volume_up();
  } else if (my_func == 8) {
    ex_tpose_down(playing);
  } else if (my_func == 9) {
    ex_tpose_up(playing);
  } else if (my_func == 10) {
    ex_cycle_capo(playing);
  
  };
  return;
};

/// @brief Prompt user to choose the button function
/// @param but_num an int, 1-3, corresponding to the left-to-right button being configured
void ExButton::fn_choice_screen(int but_num) {

  // Trying to put this as a function with the menu screens was a big problem because
  // this class calls those functions and there was a circular definition problem.


  bool done = false;
  while (!done) {

    print_menu_6("Ex Button Func.", "Open Pause Menu", "Cycle Mel. Mute", "Cycle Dn/Tr. Mute",
                 "Toggle Drone Mute", "Toggle Trompette Mute", "Next Page...");
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
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 1); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 1); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 1); };

      done = true;
    } else if (my2Button->wasPressed()) {
      setFunc(2);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 2); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 2); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 2); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 2); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 2); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 2); };

      done = true;
    } else if (my3Button->wasPressed()) {
      setFunc(3);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 3); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 3); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 3); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 3); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 3); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 3); };

      done = true;
    } else if (my4Button->wasPressed()) {
      setFunc(4);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 4); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 4); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 4); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 4); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 4); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 4); };

      done = true;
    } else if (my5Button->wasPressed()) {
      setFunc(5);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 5); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 5); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 5); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 5); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 5); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 5); };

      done = true;
    } else if (my6Button->wasPressed()) {
      done = fn_choice_screen_2(but_num);
    
    } else if (myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

/// @brief Prompt user to choose the button function, second page
/// @param but_num an int, 1-3, corresponding to the left-to-right button being configured
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.3.3*
bool ExButton::fn_choice_screen_2(int but_num) {

  bool done = false;
  while (!done) {

    print_menu_5("Ex Button Func., P.2", "Turn Volume Down", "Turn Volume Up", "Transpose Down", "Transpose Up", "Cycle Capo");
    delay(200);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      setFunc(6);
      // this is ugly, I know...
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 6); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 6); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 6); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 6); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 6); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 6); };

      done = true;
    } else if (my2Button->wasPressed()) {
      setFunc(7);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 7); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 7); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 7); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 7); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 7); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 7); };

      done = true;
    } else if (my3Button->wasPressed()) {
      setFunc(8);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 8); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 8); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 8); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 8); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 8); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 8); };

      done = true;
    } else if (my4Button->wasPressed()) {
      setFunc(9);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 9); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 9); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 9); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 9); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 9); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 9); };

      done = true;
    } else if (my5Button->wasPressed()) {
      setFunc(10);
      if (but_num == 1) { EEPROM.write(EEPROM_EX1, 10); };
      if (but_num == 2) { EEPROM.write(EEPROM_EX2, 10); };
      if (but_num == 3) { EEPROM.write(EEPROM_EX3, 10); };
      if (but_num == 4) { EEPROM.write(EEPROM_EX4, 10); };
      if (but_num == 5) { EEPROM.write(EEPROM_EX5, 10); };
      if (but_num == 6) { EEPROM.write(EEPROM_EX6, 10); };

      done = true;
    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};
