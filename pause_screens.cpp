#include "pause_screens.h"

/// @defgroup pause Pause Screens
/// These are the menu screens which don't have to do with tuning.
/// @{

/// @brief This is the main Pause Screen, branching out to all other runtime menus.
void pause_screen() {

  bool done = false;
  while (!done) {
    String d_str = "On";
    String t_str = "On";
    String h_str = "On";
    String l_str = "On";

    if (drone_mode == 1 || drone_mode == 2) {
      t_str = "OFF";
    };
    if (drone_mode == 1 || drone_mode == 3) {
      d_str = "OFF";
    };
    if (mel_mode == 1 || mel_mode == 3) {
      l_str = "OFF";
    };
    if (mel_mode == 2 || mel_mode == 3) {
      h_str = "OFF";
    };

    print_pause_screen(d_str, t_str, h_str, l_str);

    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();
    myAButton->update();
    myBButton->update();

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

    } else if (my5Button->wasPressed()) {

      delay(150);

      print_message_2("Help/User Guide", "Scan with your phone,", "Press X to continue...");

      while (true) {
        myXButton->update();

        if(myXButton->wasPressed()) {
          break;
        };
      };
      
      u8g2.clearBuffer();
      u8g2.drawXBM(0, 0, 128, 64, qrcode);
      u8g2.sendBuffer();
      
      while (true) {
        delay(150);
        myXButton->update();

        if(myXButton->wasPressed()) {
          break;
        };
      };

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      done = true;

    } else if (myAButton->wasPressed()) {
      if (drone_mode == 0) {
        drone_mode = 1; // 1 == both off
        mydrone->setMute(true);
        mytromp->setMute(true);
        mybuzz->setMute(true);
      } else if (drone_mode == 1) {
        drone_mode = 2; // 2 == drone on, tromp off
        mydrone->setMute(false);
        mytromp->setMute(true);
        mybuzz->setMute(true);
      } else if (drone_mode == 2) {
        drone_mode = 3; // 3 == drone off, tromp on
        mydrone->setMute(true);
        mytromp->setMute(false);
        mybuzz->setMute(false);
      } else if (drone_mode == 3) {
        drone_mode = 0; // 0 == both on
        mydrone->setMute(false);
        mytromp->setMute(false);
        mybuzz->setMute(false);
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
        mel_mode = 3; // 3 = high off, low off
        mystring->setMute(true);
        mylowstring->setMute(true);
      } else if (mel_mode == 3) {
        mel_mode = 0; // 0 == high on, low on
        mystring->setMute(false);
        mylowstring->setMute(false);
      };
    };
  };

  // Crank On! for half a sec.
  u8g2.clearBuffer();
  u8g2.drawBitmap(0, 0, 16, 64, crank_on_logo);
  u8g2.sendBuffer();
  delay(500);
};

/// @brief This screen prompts the user to choose what kind of tuning they wish to load, and runs the appropriate load screen or exits.
/// @return True if a new tuning choice was made, false if user chooses the "go back" option.
bool load_tuning_screen() {

  bool done = false;
  while (!done) {

    print_menu_2("Load Tuning", "Preset Tuning", "Saved Tuning");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (load_preset_screen()) {
        done = true;
      };

    } else if (my2Button->wasPressed()) {
      if (load_saved_screen()) {
        done = true;
      };

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};

/// @brief Checks if a given save slot is occupied, prompts user to continue if necessary.
/// @param slot The EEPROM save slot to check
/// @return True if slot is empty or user wants to overwrite it, false otherwise
bool check_save_tuning(int slot) {

  if (EEPROM.read(slot) == 0) {
    return true;
  } else {

    print_confirm_screen("Save Tuning", "Save slot is full...", "Save anyway?", "Overwrite");
    delay(150);

    bool done = false;
    while (!done) {

      my1Button->update();
      my2Button->update();
      myXButton->update();

      if (my1Button->wasPressed()) {
        return true;

      } else if (my2Button->wasPressed() || myXButton->wasPressed()) {
        return false;
      };
    };
    return false;
  };
};

/// @brief This screen prompts to the user to choose between four save slots, and attempts to save to that slot.
/// @details User is prompted with check_save_tuning() if the chosen slot is full.  Also prints confirmation screen if saving occurs.
void save_tuning_screen() {

  bool done = false;
  int slot = 0;
  while (!done) {

    print_menu_4("Save Tuning", "Use Slot 1", "Use Slot 2", "Use Slot 3", "Use Slot 4");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT1)) {
        save_tunings(EEPROM_SLOT1);
        slot = 1;
        done = true;
      };

    } else if (my2Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT2)) {
        save_tunings(EEPROM_SLOT2);
        slot = 2;
        done = true;
      };

    } else if (my3Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT3)) {
        save_tunings(EEPROM_SLOT3);
        slot = 3;
        done = true;
      };

    } else if (my4Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT4)) {
        save_tunings(EEPROM_SLOT4);
        slot = 4;
        done = true;
      };

    } else if (myXButton->wasPressed() || my5Button->wasPressed()) {
      // Just return.
      return;
    };
  };

  // Display a confirmation message
  print_message_2("Save Tuning", String("Slot ") + slot, "Saved to EEPROM");
  delay(500);
};

/// @brief This is the about screen shown when the user selects it, which displays until the user presses "X" to continue.
void options_about_screen() {

  about_screen();

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

  u8g2.clearBuffer();
  u8g2.drawXBM(0, 0, 128, 64, qrcode_digigurdy);
  u8g2.sendBuffer();
  
  while (true) {
    delay(150);
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

  print_message_3("Special Thanks!", "John Dingley (UK)", "David Jacobs (AUS)", "Matteo Billia (ITA)");

  while (true) {
    delay(150);
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

  print_message_2("Special Thanks!", "lune36400", "SalusaSecondus");

  while (true) {
    delay(150);
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

};

/// @brief This prompts the user to choose between the non-tuning/volume configuration options.
/// @return True if the user chooses one of the options, false otherwise
bool other_options_screen() {

  bool done = false;
  while (!done) {

    #ifndef USE_GEARED_CRANK
    print_menu_4("Other Options", "EX Button Config", "Screen Configuration", "Input/Output Config", "About Digi-Gurdy");

    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex_btn_choice_screen();
      
    } else if (my2Button->wasPressed()) {
      playing_config_screen();

    } else if (my3Button->wasPressed()) {
      io_screen();

    } else if (my4Button->wasPressed()) {
      options_about_screen();

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };

    #else

    print_menu_5("Other Options", "Crank Detection", "EX Button Config", "Screen Configuration", "Input/Output Config", "About Digi-Gurdy");
    
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      
      print_message_2("Crank Detection", "Crank is detecting,", "Please wait...");
      mycrank->detect();
      if (mycrank->isDetected()) {
        print_message_2("Crank Detection", "Crank is detecting,", "CRANK DETECTED!");
        delay(1000);
      } else {
        print_message_2("Crank Detection", "Crank is detecting,", "CRANK NOT FOUND.");
        delay(1000);
      };

    } else if (my2Button->wasPressed()) {
      ex_btn_choice_screen();

    } else if (my3Button->wasPressed()) {
      playing_config_screen();

    } else if (my4Button->wasPressed()) {
      io_screen();

    } else if (my5Button->wasPressed()) {
      options_about_screen();

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
 
    #endif
  };

  return true;
};

/// @brief Saves the current tuning/volume to the given save slot.
/// @param slot 1-4, the EEPROM save slot to write to.
void save_tunings(int slot) {

  EEPROM.write(slot + EEPROM_HI_MEL, mystring->getOpenNote());
  EEPROM.write(slot + EEPROM_LO_MEL, mylowstring->getOpenNote());
  EEPROM.write(slot + EEPROM_DRONE, mydrone->getOpenNote());
  EEPROM.write(slot + EEPROM_TROMP, mytromp->getOpenNote());
  EEPROM.write(slot + EEPROM_BUZZ, mybuzz->getOpenNote());
  EEPROM.write(slot + EEPROM_TPOSE, tpose_offset + 12);
  EEPROM.write(slot + EEPROM_CAPO, capo_offset);
  EEPROM.write(slot + EEPROM_HI_MEL_VOL, mystring->getVolume());
  EEPROM.write(slot + EEPROM_LOW_MEL_VOL, mylowstring->getVolume());
  EEPROM.write(slot + EEPROM_DRONE_VOL, mydrone->getVolume());
  EEPROM.write(slot + EEPROM_TROMP_VOL, mytromp->getVolume());
  EEPROM.write(slot + EEPROM_BUZZ_VOL, mybuzz->getVolume());
  EEPROM.write(slot + EEPROM_KEYCLICK_VOL, mykeyclick->getVolume());
  EEPROM.write(slot + EEPROM_HI_MEL_GROS, mystring->getGrosMode());
  EEPROM.write(slot + EEPROM_LOW_MEL_GROS, mylowstring->getGrosMode());
  EEPROM.write(slot + EEPROM_TROMP_GROS, mytromp->getGrosMode());
  EEPROM.write(slot + EEPROM_DRONE_GROS, mydrone->getGrosMode());
  EEPROM.write(slot + EEPROM_BUZZ_GROS, mybuzz->getGrosMode());

};

/// @brief Resets EX EEPROM values to their defaults
void reset_ex_eeprom() {

  #ifdef REV4_MODE
  EEPROM.write(EEPROM_BUZZ_LED, 1);
  EEPROM.write(EEPROM_EX1, 1);
  EEPROM.write(EEPROM_EX2, 2);
  EEPROM.write(EEPROM_EX3, 4);
  EEPROM.write(EEPROM_EX4, 5);
  EEPROM.write(EEPROM_EX5, 13);
  EEPROM.write(EEPROM_EX6, 6);
  EEPROM.write(EEPROM_EX7, 7);
  EEPROM.write(EEPROM_EX8, 8);
  EEPROM.write(EEPROM_EX9, 9);
  EEPROM.write(EEPROM_EX10, 10);
  #else
  EEPROM.write(EEPROM_BUZZ_LED, 1);
  EEPROM.write(EEPROM_EX1, 1);
  EEPROM.write(EEPROM_EX2, 2);
  EEPROM.write(EEPROM_EX3, 3);
  EEPROM.write(EEPROM_EX4, 8);
  EEPROM.write(EEPROM_EX5, 9);
  EEPROM.write(EEPROM_EX6, 10);
  #endif

  EEPROM.write(EEPROM_EXBB, 11);
};

/// @brief Clears the EEPROM and sets some default values in it.
/// @note Most values set to zero, but LED and EX values have non-zero defaults also set here.
void reset_eeprom() {
  // Not much to say here... write 0 everywhere:
  for (int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);

  // But now let's fill in the defaults:
  reset_ex_eeprom();

  #ifndef USB_ALWAYS_ON
  usb_power_off();
  #endif
  MIDI.begin(MIDI_CHANNEL_OMNI);
  delay(100);

  mystring->setOutputMode(0);
  mylowstring->setOutputMode(0);
  mytromp->setOutputMode(0);
  mydrone->setOutputMode(0);
  mykeyclick->setOutputMode(0);
  mybuzz->setOutputMode(0);  
};

/// @brief Prompts the user to choose a saved tuning slot, and calls view_slot_screen() for that slot.
/// @return True if the user selects a slot, false otherwise.
bool load_saved_screen() {

  bool done = false;
  while (!done) {

    print_menu_4("Load Saved Tuning", "Slot 1", "Slot 2", "Slot 3", "Slot 4");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (view_slot_screen(1)) { done = true; };

    } else if (my2Button->wasPressed()) {
      if (view_slot_screen(2)) { done = true; };

    } else if (my3Button->wasPressed()) {
      if (view_slot_screen(3)) { done = true; };

    } else if (my4Button->wasPressed()) {
      if (view_slot_screen(4)) { done = true; };

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

/// @brief Prompts the user to choose a preset tuning slot, and calls view_preset_screen() for that slot.
/// @return True if the user selects a slot, false otherwise.
bool load_preset_screen() {

  bool done = false;
  while (!done) {

    print_menu_4("Select Tuning Preset",
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
      if (view_preset_screen(1)) {done = true;};

    } else if (my2Button->wasPressed()) {
      if (view_preset_screen(2)) {done = true;};

    } else if (my3Button->wasPressed()) {
      if (view_preset_screen(3)) {done = true;};
    } else if (my4Button->wasPressed()) {
      if (view_preset_screen(4)) {done = true;};
    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

/// @brief Prompts user to activate or disable the Scene Signaling (Program Change) feature.
void scene_options_screen() {
  bool done = false;
  while (!done) {

    print_menu_2("Scene Signaling", "None", "Prog Chg, Ch. 1");
    delay(150);

    // Check the buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      scene_signal_type = 0;
      EEPROM.write(EEPROM_SCENE_SIGNALLING, scene_signal_type);
      done = true;
    } else if (my2Button->wasPressed()) {
      scene_signal_type = 1;
      EEPROM.write(EEPROM_SCENE_SIGNALLING, scene_signal_type);
      done = true;
    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    }
  }
}

/// @brief Prompts user to choose which play screen to use.
void playing_scr_screen() {
  bool done = false;
  while (!done) {

    print_menu_6("Choose Play Screen", "Note Bitmap + Staff", "Printed Note + Staff", "Note Bitmap Only", "Printed Note Only", "Staff Only", "Blank");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 0;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Note Bitmap + Staff", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 1;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Printed Note + Staff", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my3Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 2;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Note Bitmap Only", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my4Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 3;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Printed Note Only", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my5Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 4;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Staff Only Screen", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my6Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 5;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Blank Play Screen", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (myXButton->wasPressed()) {
      done = true;
    };
  };
};

/// @brief The startup other-options screen: prompts user to clear the EEPROM, adjust Scene Control, change Secondary Output, view the about screen, or go back.
void options_screen() {

  bool done = false;
  while (!done) {

    print_menu_4("Options", "Reset All Settings", "Scene Control", "Secondary Output", "About Digi-Gurdy");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      reset_eeprom();

      print_message_2("Reset All Settings", "All settings reset", "to defaults!");
      delay(1000);
      done = true;

    } else if (my2Button->wasPressed()) {
      scene_options_screen();
      done = true;
    } else if (my3Button->wasPressed()) {
      sec_output_screen();
      done = true;
    } else if (my4Button->wasPressed()) {
      options_about_screen();
      done = true;
    } else if (myXButton->wasPressed() || my5Button->wasPressed()) {
      done = true;
    };
  };
};

/// @brief This is the opening menu screen, prompting user to choose some kind of tuning or view the other startup options.
void welcome_screen() {

  bool done = false;
  while (!done) {

    print_menu_4_nobk("Digi-Gurdy", "Load Preset", "Load Save Slot",
                      "New Tuning Setup", "Other Options");
    delay(150);

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

      // Need to re-check use_solfege in case it changed...
      use_solfege = EEPROM.read(EEPROM_USE_SOLFEGE);

      // Not done = true here, we'd want to get prompted again.
    };
  };
};

/// @brief Prompts user to enable or disable the buzz LED indicator feature.
void led_screen() {

  bool done = false;
  while (!done) {

    print_menu_2("Buzz LED On/Off", "Enable Buzz LED (On)", "Disable Buzz LED (Off)");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      EEPROM.write(EEPROM_BUZZ_LED, 1);

      #ifndef USE_GEARED_CRANK
        mycrank->enableLED();
      #endif

      print_message_2("Buzz LED On/Off", "Buzz LED On", "Saved to EEPROM");
      delay(1000);

      done = true;

    } else if (my2Button->wasPressed()) {
      EEPROM.write(EEPROM_BUZZ_LED, 0);

      #ifndef USE_GEARED_CRANK
        mycrank->disableLED();
      #endif
      
      print_message_2("Buzz LED On/Off", "Buzz LED Off", "Saved to EEPROM");
      delay(1000);

      done = true;

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      done = true;

    };
  };
};

/// @brief This menu screen is for enabling/disabling the accessory/vibrato pedal.
void vib_screen() {

  bool done = false;
  while (!done) {

    print_menu_2("Vibrato Pedal", "Enable Pedal (On)", "Disable Pedal (Off)");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      myvibknob->enable();

      print_message_2("Vibrato Pedal", "Pedal On", "Saved to EEPROM");
      delay(1000);

      done = true;

    } else if (my2Button->wasPressed()) {
      myvibknob->disable();

      print_message_2("Vibrato Pedal", "Pedal Off", "Saved to EEPROM");
      delay(1000);

      done = true;

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      done = true;

    };
  };
};

/// @brief Prompts user to adjust the on-screen options: note notation, on-screen buzz indicator, play screen type.
void playing_config_screen() {
  bool done = false;
  while (!done) {

    if (play_screen_type / 10 > 0) {
      print_menu_3("Screen Configuration", "Notation (ABC/DoReMi)", "Disable Buzz Indicator", "Choose Play Screen");
    } else {
      print_menu_3("Screen Configuration", "Notation (ABC/DoReMi)", "Enable Buzz Indicator", "Choose Play Screen");
    };

    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      notation_config_screen();

    } else if (my2Button->wasPressed()) {

      if (play_screen_type / 10 > 0) {
        play_screen_type = play_screen_type % 10;
        EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

        print_message_2("Play Screen Options", "Buzz Indicator Disabled", "Saved to EEPROM");
        delay(750);
      } else {
        play_screen_type = 10 + (play_screen_type % 10);
        EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

        print_message_2("Play Screen Options", "Buzz Indicator Enabled", "Saved to EEPROM");
        delay(750);
      };

    } else if (my3Button->wasPressed()) {
      playing_scr_screen();

    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };
  };
};

/// @brief Prompts user to choose the on-screen note notation to be used.
void notation_config_screen() {
  bool done = false;
  while (!done) {

    print_menu_3("Note Notation", "Scientific (ABC)", "Solfege (DoReMi)", "Combination (C/Do)");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      EEPROM.write(EEPROM_USE_SOLFEGE, 0);
      use_solfege = 0;

      print_message_2("Note Notation", "Scientific (ABC)", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {

      EEPROM.write(EEPROM_USE_SOLFEGE, 1);
      use_solfege = 1;

      print_message_2("Note Notation", "Solfege (DoReMi)", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my3Button->wasPressed()) {

      EEPROM.write(EEPROM_USE_SOLFEGE, 2);
      use_solfege = 2;

      print_message_2("Note Notation", "Combination (C/Do)", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };
  };
};

/// @brief Prompts the user to choose between various input/output options
void io_screen() {
  bool done = false;
  while (!done) {

    String opt2 = "This Option Disabled";
    String opt3 = "This Option Disabled";
    #ifdef USE_PEDAL
    opt2 = "Vibrato Pedal On/Off";
    #endif
    #ifdef LED_KNOB
    opt3 = "Buzz LED On/Off";
    #endif

    #ifndef USE_GEARED_CRANK
    print_menu_4("Input/Output Options", "Secondary Output", "MIDI Melody Vibrato", opt2, opt3);
    #else
    print_menu_4("Input/Output Options", "Secondary Output", "MIDI Melody Vibrato", opt2, opt3);
    #endif

    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      sec_output_screen();

    } else if (my2Button->wasPressed()) {
      mel_vib_screen();
    
    } else if (my3Button->wasPressed()) {
      #ifdef USE_PEDAL
      vib_screen();
      #endif

    } else if (my4Button->wasPressed()) {
      #ifdef LED_KNOB
      led_screen();
      #endif

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };
  };
};

/// @brief Promts user to choose the secondary output (primary output is usbMIDI)
void sec_output_screen() {
  bool done = false;
  while (!done) {

    #ifdef ALLOW_COMBO_MODE
      print_menu_3("Secondary Output", "MIDI-OUT Socket", "Audio Socket", "MIDI-OUT and Audio Socket");
    #else
      print_menu_2("Secondary Output", "MIDI-OUT Socket", "Audio Socket");
    #endif

    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      EEPROM.write(EEPROM_SEC_OUT, 0);

      #ifndef USB_ALWAYS_ON
      usb_power_off();
      #endif
      MIDI.begin(MIDI_CHANNEL_OMNI);
      delay(100);

      all_clearVolArray();
      mystring->setOutputMode(0);
      mylowstring->setOutputMode(0);
      mytromp->setOutputMode(0);
      mydrone->setOutputMode(0);
      mykeyclick->setOutputMode(0);
      mybuzz->setOutputMode(0);
      
      print_message_2("Secondary Output", "MIDI-OUT", "Saved to EEPROM!");
      delay(750);

      done = true;

    } else if (my2Button->wasPressed()) {

      #ifndef ALLOW_COMBO_MODE
      print_message_2("Secondary Output", "Un-plug any MIDI adapters!", "Press X to Continue");

      bool done2= false;
      while (!done2) {
        delay(150);

        myXButton->update();

        if (myXButton->wasPressed()) {
          done2 = true;
        };
      };
      #endif

      draw_xbm(progress[0]);
      delay(100);
      draw_xbm(progress[1]);

      EEPROM.write(EEPROM_SEC_OUT, 1);

      usb_power_on();
      trigger_obj.start();
      delay(100);

      draw_xbm(progress[2]);

      all_clearVolArray();
      mystring->setOutputMode(1);
      mylowstring->setOutputMode(1);
      mytromp->setOutputMode(1);
      mydrone->setOutputMode(1);
      mykeyclick->setOutputMode(1);
      mybuzz->setOutputMode(1);

      draw_xbm(progress[3]);
      mystring->setTrackLoops();
      draw_xbm(progress[4]);
      mylowstring->setTrackLoops();
      draw_xbm(progress[5]);
      mytromp->setTrackLoops();
      draw_xbm(progress[6]);
      mydrone->setTrackLoops();
      draw_xbm(progress[7]);
      mybuzz->setTrackLoops();
      draw_xbm(progress[8]);
      mykeyclick->setTrackLoops();
      draw_xbm(progress[9]);
      delay(400);

      print_message_2("Secondary Output", "Completed! Audio Socket", "Saved to EEPROM!");
      delay(600);

      done = true;

    #ifdef ALLOW_COMBO_MODE
      
    } else if (my3Button->wasPressed()) {

      draw_xbm(progress[0]);
      delay(100);
      draw_xbm(progress[1]);

      EEPROM.write(EEPROM_SEC_OUT, 2);

      usb_power_on();
      MIDI.begin(MIDI_CHANNEL_OMNI);
      trigger_obj.start();
      delay(100);

      draw_xbm(progress[2]);

      all_clearVolArray();
      mystring->setOutputMode(2);
      mylowstring->setOutputMode(2);
      mytromp->setOutputMode(2);
      mydrone->setOutputMode(2);
      mykeyclick->setOutputMode(2);
      mybuzz->setOutputMode(2);

      draw_xbm(progress[3]);
      mystring->setTrackLoops();
      draw_xbm(progress[4]);
      mylowstring->setTrackLoops();
      draw_xbm(progress[5]);
      mytromp->setTrackLoops();
      draw_xbm(progress[6]);
      mydrone->setTrackLoops();
      draw_xbm(progress[7]);
      mybuzz->setTrackLoops();
      draw_xbm(progress[8]);
      mykeyclick->setTrackLoops();
      draw_xbm(progress[9]);
      delay(400);

      print_message_2("Secondary Output", "MIDI-OUT + Audio", "Saved to EEPROM!");
      delay(600);

      done = true;

    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };
    
    #else

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };

    #endif
  };
};

/// @brief Prompts user to choose what amount of constant vibrato to send with the melody strings.
void mel_vib_screen() {
  bool done = false;
  int new_vib = mel_vibrato;

  delay(300);
  while (!done) {

    print_value_selection("Choose Vibrato Amount", new_vib);

    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (new_vib > 0) {
        new_vib -= 1;
        delay(300);
      };
    } else if (my1Button->beingPressed()) {
      if (new_vib > 0) {
        new_vib -= 1;
        delay(100);
      };
    } else if (my2Button->wasPressed()) {
      if (new_vib < 127) {
        new_vib += 1;
        delay(300);
      };
    } else if (my2Button->beingPressed()) {
      if (new_vib < 127) {
        new_vib += 1;
        delay(100);
      };
    } else if (myXButton->wasPressed()) {
      mel_vibrato = new_vib;
      EEPROM.write(EEPROM_MEL_VIBRATO, mel_vibrato);
      print_message_2("Choose Vibrato Amount", "MIDI Melody Vibrato", "Saved to EEPROM");
      delay(500);
      done = true;
    };
  };
};

/// @}
