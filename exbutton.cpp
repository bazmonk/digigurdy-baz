#include "exbutton.h"

/// @brief Constructor for ExButton
/// @param my_pin The digital pin this button is connected to
/// @param func The beginning function for this button (See ExButton::doFunc for the numbering)
/// @param interval The debounce interval for this button in milliseconds
ExButton::ExButton(int my_pin, int func, int interval, int my_addr) : ToggleButton(my_pin, interval) {
  my_func = func;
  eeprom_addr = my_addr;
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
  } else if (my_func == 11) {
    return;
  };
  return;
};

/// @brief Returns a short text label of the button's function.
/// @return A short description of the current button fucntion.
String ExButton::printFunc() {
  if (my_func == 1) {
    return String("Open Pause Menu");
  } else if (my_func == 2) {
    return String("Melody Mutes");
  } else if (my_func == 3) {
    return String("Dro./Tro. Mutes");
  } else if (my_func == 4) {
    return String("Drone Mute");
  } else if (my_func == 5) {
    return String("Trompette Mute");
  } else if (my_func == 6) {
    return String("Volume Down");
  } else if (my_func == 7) {
    return String("Volume Up");
  } else if (my_func == 8) {
    return String("Transpose Down");
  } else if (my_func == 9) {
    return String("Transpose Up");
  } else if (my_func == 10) {
    return String("Cycle Capo");
  } else if (my_func == 11) {
    return String("Auto-Crank");
  };

  return String("FIX ME!!!");
};

/// @brief Prompt user to choose the button function
void ExButton::fn_choice_screen() {

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
      EEPROM.write(eeprom_addr,1);
      done = true;

    } else if (my2Button->wasPressed()) {
      setFunc(2);
      EEPROM.write(eeprom_addr,2);
      done = true;

    } else if (my3Button->wasPressed()) {
      setFunc(3);
      EEPROM.write(eeprom_addr,3);
      done = true;

    } else if (my4Button->wasPressed()) {
      setFunc(4);
      EEPROM.write(eeprom_addr,4);
      done = true;

    } else if (my5Button->wasPressed()) {
      setFunc(5);
      EEPROM.write(eeprom_addr,5);
      done = true;

    } else if (my6Button->wasPressed()) {
      done = fn_choice_screen_2();
    
    } else if (myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

/// @brief Prompt user to choose the button function, second page
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.3.3*
bool ExButton::fn_choice_screen_2() {

  bool done = false;
  while (!done) {

    print_menu_6("Ex Button Func., P.2", "Turn Volume Down", "Turn Volume Up", "Transpose Down", "Transpose Up", "Cycle Capo", "Auto-Crank");
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
      EEPROM.write(eeprom_addr,6);
      done = true;

    } else if (my2Button->wasPressed()) {
      setFunc(7);
      EEPROM.write(eeprom_addr,7);
      done = true;

    } else if (my3Button->wasPressed()) {
      setFunc(8);
      EEPROM.write(eeprom_addr,8);
      done = true;

    } else if (my4Button->wasPressed()) {
      setFunc(9);
      EEPROM.write(eeprom_addr,9);
      done = true;

    } else if (my5Button->wasPressed()) {
      setFunc(10);
      EEPROM.write(eeprom_addr,10);
      done = true;

    } else if (my6Button->wasPressed()) {
      setFunc(11);
      EEPROM.write(eeprom_addr,11);
      done = true;

    } else if (myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};
