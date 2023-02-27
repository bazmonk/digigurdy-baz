#include "exbutton.h"

/// @brief Constructor for ExButton
/// @param my_pin The digital pin this button is connected to
/// @param func The beginning function for this button (See ExButton::doFunc for the numbering)
/// @param interval The debounce interval for this button in milliseconds
ExButton::ExButton(int my_pin, int interval, int my_addr, int my_step_addr, int my_slot_addr) : ToggleButton(my_pin, interval) {
  eeprom_addr = my_addr;
  my_func = EEPROM.read(my_addr);
  eeprom_step_addr = my_step_addr;
  eeprom_slot_addr = my_slot_addr;
  slot = EEPROM.read(my_slot_addr);
  t_toggle_steps = EEPROM.read(my_step_addr) - 12;
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
  Serial.println(printFunc());

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
  } else if (my_func == 12) {
    ex_tpose_toggle(playing, t_toggle_steps);
  } else if (my_func == 13) {
    ex_sec_out_toggle();
  } else if (my_func == 14) {
    ex_cycle_hi_mel_mute();
  } else if (my_func == 15) {
    ex_cycle_lo_mel_mute();
  } else if (my_func == 16) {
    ex_load_preset(slot);
  } else if (my_func == 17) {
    ex_load_save_slot(slot);
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
  } else if (my_func == 12) {
    return String("Transpose: ") + t_toggle_steps;
  } else if (my_func == 13) {
    return String("Sec. Output Toggle");
  } else if (my_func == 14) {
    return String("Hi Melody Mute");
  } else if (my_func == 15) {
    return String("Lo Melody Mute");
  } else if (my_func == 16) {
    return String("Load Preset: " + slot);
  } else if (my_func == 17) {
    return String("Load Save Slot: " + slot);
  };

  return String("FIX ME!!!");
};

/// @brief Prompt user to choose the button function
/// @return If choice was made
bool ExButton::fn_choice_screen() {

  // Trying to put this as a function with the menu screens was a big problem because
  // this class calls those functions and there was a circular definition problem.


  bool done = false;
  while (!done) {

    print_menu_5("Ex Button Func.", "Actions", "Mutes", "Transpose/Capo",
                 "Audio", "Load Tunings");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      done = fn_choice_actions();

    } else if (my2Button->wasPressed()) {
      done = fn_choice_mutes();

    } else if (my3Button->wasPressed()) {
      done = fn_choice_tpose();

    } else if (my4Button->wasPressed()) {
      done = fn_choice_audio();

    } else if (my5Button->wasPressed()) {
      done = fn_choice_tuning();

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return false;

    };
  };

  return true;
};

/// @brief Prompt user to select an action-based EX button function.
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.9.8*
bool ExButton::fn_choice_actions() {

  bool done = false;
  while (!done) {

    print_menu_2("Ex Button Actions", "Open Pause Menu", "Auto-Crank");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      setFunc(1);
      EEPROM.write(eeprom_addr,1);
      done = true;

    } else if (my2Button->wasPressed()) {
      setFunc(11);
      EEPROM.write(eeprom_addr,11);
      done = true;

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      return false;

    };
  };

  return true;
};

/// @brief Prompt user to select an EX button string mute function.
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.9.8*
bool ExButton::fn_choice_mutes() {

  bool done = false;
  while (!done) {

    print_menu_6("Ex Button Mutes", "Cycle Mel. Mutes", "Cycle Dn/Tr. Mutes", "Hi Melody Mute", "Lo Melody Mute", "Drone Mute", "Tromp/Buzz Mute");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      setFunc(2);
      EEPROM.write(eeprom_addr,2);
      done = true;

    } else if (my2Button->wasPressed()) {
      setFunc(3);
      EEPROM.write(eeprom_addr,3);
      done = true;

    } else if (my3Button->wasPressed()) {
      setFunc(14);
      EEPROM.write(eeprom_addr,14);
      done = true;

    } else if (my4Button->wasPressed()) {
      setFunc(15);
      EEPROM.write(eeprom_addr,15);
      done = true;

    } else if (my5Button->wasPressed()) {
      setFunc(4);
      EEPROM.write(eeprom_addr,4);
      done = true;

    } else if (my6Button->wasPressed()) {
      setFunc(5);
      EEPROM.write(eeprom_addr,5);
      done = true;
      
    } else if (myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};

/// @brief Prompt user to select a transpose/capo EX button function.
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.9.8*
bool ExButton::fn_choice_tpose() {

  bool done = false;
  while (!done) {

    print_menu_4("Ex Button Tpose/Capo", "Transpose Down", "Transpose Up", "Transpose Toggle", "Cycle Capo");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      setFunc(8);
      EEPROM.write(eeprom_addr,8);
      done = true;

    } else if (my2Button->wasPressed()) {
      setFunc(9);
      EEPROM.write(eeprom_addr,9);
      done = true;

    } else if (my3Button->wasPressed()) {

      int new_step = 0;
      bool done2 = false;
      while (!done2) {

        print_value_selection("Choose Transpose Toggle", new_step);

        my1Button->update();
        my2Button->update();
        myXButton->update();

        if (my1Button->wasPressed()) {
          if (new_step > -12) {
            new_step -= 1;
            delay(300);
          };
        } else if (my1Button->beingPressed()) {
          if (new_step > -12) {
            new_step -= 1;
            delay(100);
          };
        } else if (my2Button->wasPressed()) {
          if (new_step < 12) {
            new_step += 1;
            delay(300);
          };
        } else if (my2Button->beingPressed()) {
          if (new_step < 12) {
            new_step += 1;
            delay(100);
          };
        } else if (myXButton->wasPressed()) {
          setFunc(12);
          t_toggle_steps = new_step;
          EEPROM.write(eeprom_addr, 12);
          EEPROM.write(eeprom_step_addr, t_toggle_steps + 12);
          done2 = true;
        };
      };
      done = true;

    } else if (my4Button->wasPressed()) {
      setFunc(10);
      EEPROM.write(eeprom_addr,10);
      done = true;

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;

    };
  };

  return true;
};

/// @brief Prompt user to select an audio-related EX button function.
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.9.8*
bool ExButton::fn_choice_audio() {

  bool done = false;
  while (!done) {

    print_menu_3("Ex Button Audio", "Turn Volume Down", "Turn Volume Up", "Sec. Output Toggle");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
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
      setFunc(13);
      EEPROM.write(eeprom_addr,13);
      done = true;

    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      return false;

    };
  };

  return true;
};

/// @brief Prompt user to select an load-tuning EX button function.
/// @return True if user chose an option, false if user chose "go back" option
/// @version *New in 2.9.8*
bool ExButton::fn_choice_tuning() {

  bool done = false;
  while (!done) {

    print_menu_2("Ex Button Load Tuning", "Load Preset Tuning", "Load Save Slot");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      bool done2 = false;
      while (!done2) {

        print_menu_4("Ex Button Preset",
                 PRESET1_NAME, PRESET2_NAME,
                 PRESET3_NAME, PRESET4_NAME);
        delay(150);

        // Check the 1 and 2 buttons
        my1Button->update();
        my2Button->update();
        my3Button->update();
        my4Button->update();
        my5Button->update();
        myXButton->update();

        if (my1Button->wasPressed()) {
          slot = 1;
          done2 = true;
        
        } else if (my2Button->wasPressed()) {
          slot = 2;
          done2 = true;
      
        } else if (my3Button->wasPressed()) {
          slot = 3;
          done2 = true;
    
        } else if (my4Button->wasPressed()) {
          slot = 4;
          done2 = true;
  
        } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
          return false;
        };
      };

      setFunc(16);
      EEPROM.write(eeprom_addr, 16);
      EEPROM.write(eeprom_slot_addr, slot);
      done = true;

    } else if (my2Button->wasPressed()) {

      bool done2 = false;
      while (!done2) {

        print_menu_4("Ex Button Save Slot",
                 "Save Slot 1", "Save Slot 2",
                 "Save Slot 3", "Save Slot 4");
        delay(150);

        // Check the 1 and 2 buttons
        my1Button->update();
        my2Button->update();
        my3Button->update();
        my4Button->update();
        my5Button->update();
        myXButton->update();

        if (my1Button->wasPressed()) {
          slot = 1;
          done2 = true;
        
        } else if (my2Button->wasPressed()) {
          slot = 2;
          done2 = true;
      
        } else if (my3Button->wasPressed()) {
          slot = 3;
          done2 = true;
    
        } else if (my4Button->wasPressed()) {
          slot = 4;
          done2 = true;

        } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
          return false;
        };
      };

      setFunc(17);
      EEPROM.write(eeprom_addr, 17);
      EEPROM.write(eeprom_slot_addr, slot);
      done = true;

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      return false;

    };
  };

  return true;
};
