#include "pause_screens.h"

// This is the screen that X+A gets you.
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
    if (mel_mode == 1) {
      l_str = "OFF";
    };
    if (mel_mode == 2) {
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

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
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
  u8g2.clearBuffer();
  u8g2.drawBitmap(0, 0, 16, 64, crank_on_logo);
  u8g2.sendBuffer();
  delay(500);
};


// This screen prompts which kind of tuning to load.
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

// Checks if save slot is occupied and prompts user to overwrite if necessary.
// Returns true if slot is empty or OK to overwrite.
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

// This is the screen for saving to a save slot
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

void options_about_screen() {

  about_screen();

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

  // BAZ - FIX THIS LATER
  // String disp_str = "      DigiGurdy      \n"
  //                   "---------------------\n"
  //                   "Special Thanks:      \n"
  //                   "                     \n"
  //                   "John Dingley         \n"
  //                   "David Jacobs         \n"
  //                   "lune36400            \n"
  //                   "SalusaSecondus       ";
  //
  // print_screen(disp_str);
  //
  // while (true) {
  //   myXButton->update();
  //
  //   if(myXButton->wasPressed()) {
  //     break;
  //   };
  // };
};

bool other_options_screen() {

  bool done = false;
  while (!done) {

    String opt2 = "This Option Disabled";
    String opt3 = "This Option Disabled";
    #ifdef LED_KNOB
    opt2 = "Buzz LED On/Off";
    #endif
    #ifdef USE_PEDAL
    opt3 = "Vibrato Pedal On/Off";
    #endif

    print_menu_5("Other Options", "EX Button Config", "Screen Configuration", opt2, opt3, "About Digi-Gurdy");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex_btn_choice_screen();

    } else if (my2Button->wasPressed()) {
      playing_config_screen();

    } else if (my3Button->wasPressed()) {
      #ifdef LED_KNOB
      led_screen();
      #endif

    } else if (my4Button->wasPressed()) {
      #ifdef USE_PEDAL
      vib_screen();
      #endif

    } else if (my5Button->wasPressed()) {
      options_about_screen();

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};

void signal_scene_change(int scene_idx) {
  if (scene_signal_type == 1) {
    // Signal as a Program Control message on Channel 1
    mystring->setProgram(scene_idx);
  } else {
    // 0 = Do nothing
    // While this should be 0, if there is bad data we'll just ignore it and do nothing.
  }
};

// load_preset_tunings accepts an int between 1-4 and sets the appropriate preset.
// See the default_tunings.h file to modify what the presets actually are.
void load_preset_tunings(int preset) {
  const int *tunings;
  if (preset == 1) { tunings = PRESET1; };
  if (preset == 2) { tunings = PRESET2; };
  if (preset == 3) { tunings = PRESET3; };
  if (preset == 4) { tunings = PRESET4; };

  mystring->setOpenNote(tunings[0]);
  mylowstring->setOpenNote(tunings[1]);
  mydrone->setOpenNote(tunings[2]);
  mytromp->setOpenNote(tunings[3]);
  mybuzz->setOpenNote(tunings[4]);
  tpose_offset = tunings[5];
  capo_offset = tunings[6];
}

// load_saved_tunings requires one argument: the "save slot" which
// should be one of the EEPROM_SLOT[1-4] values in eeprom_values.h.
void load_saved_tunings(int slot) {
  byte value;

  // Notes
  value = EEPROM.read(slot + EEPROM_HI_MEL);
  mystring->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_LO_MEL);
  mylowstring->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_DRONE);
  mydrone->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_TROMP);
  mytromp->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_BUZZ);
  mybuzz->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_TPOSE);
  tpose_offset = value - 12;
  value = EEPROM.read(slot + EEPROM_CAPO);
  capo_offset = value;

  // Volumes
  value = EEPROM.read(slot + EEPROM_HI_MEL_VOL);
  mystring->setVolume(value);
  value = EEPROM.read(slot + EEPROM_LOW_MEL_VOL);
  mylowstring->setVolume(value);
  value = EEPROM.read(slot + EEPROM_DRONE_VOL);
  mydrone->setVolume(value);
  value = EEPROM.read(slot + EEPROM_TROMP_VOL);
  mytromp->setVolume(value);
  value = EEPROM.read(slot + EEPROM_BUZZ_VOL);
  mybuzz->setVolume(value);
  value = EEPROM.read(slot + EEPROM_KEYCLICK_VOL);
  mykeyclick->setVolume(value);

};

// save_tunings accepts one argument, which should be one of the
// EEPROM_SLOT[1-4] values defined in eeprom_values.h.
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

};

// This clears the EEPROM and overwrites it all with zeroes
void clear_eeprom() {
  // Not much to say here... write 0 everywhere:
  for (int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);

  // But now let's fill in the defaults:
  EEPROM.write(EEPROM_BUZZ_LED, 1);
  EEPROM.write(EEPROM_EX1, 1);
  EEPROM.write(EEPROM_EX2, 2);
  EEPROM.write(EEPROM_EX3, 3);
  EEPROM.write(EEPROM_SCENE_SIGNALLING, 0);
  EEPROM.write(EEPROM_USE_SOLFEGE, 0);

};

// This screen is for viewing a save slot's settings.
// It returns true if the user wants to use it, false
// otherwise.
// Accepts integer slot: the save slot number.
bool view_slot_screen(int slot_num) {
  int slot;
  if (slot_num == 1) { slot = EEPROM_SLOT1; };
  if (slot_num == 2) { slot = EEPROM_SLOT2; };
  if (slot_num == 3) { slot = EEPROM_SLOT3; };
  if (slot_num == 4) { slot = EEPROM_SLOT4; };

  String t_str = "";
  if (EEPROM.read(slot + EEPROM_TPOSE) > 12) { t_str += "+"; };
  t_str = t_str + ((EEPROM.read(slot + EEPROM_TPOSE))-12);

  String cap_str = "";
  if (EEPROM.read(slot + EEPROM_CAPO) > 0) { cap_str += "+"; };
  cap_str = cap_str + (EEPROM.read(slot + EEPROM_CAPO));

  print_tuning("Saved Slot Tuning",
               getLongNoteNum(EEPROM.read(slot + EEPROM_HI_MEL)),
               getLongNoteNum(EEPROM.read(slot + EEPROM_LO_MEL)),
               getLongNoteNum(EEPROM.read(slot + EEPROM_DRONE)),
               getLongNoteNum(EEPROM.read(slot + EEPROM_TROMP)),
               t_str, cap_str);
  delay(150);

  bool done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myAButton->update();
    myXButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      load_saved_tunings(slot);
      signal_scene_change(slot_num + 3); // Zero indexed and first 4 are reserved for presets
      done = true;

    } else if (my2Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

// This screen previews a preset's settings.
// Returns true if users wants to use it, false otherwise.
// Accept integer preset_num: the preset number.
bool view_preset_screen(int preset) {
  const int *tunings;
  if (preset == 1) { tunings = PRESET1; };
  if (preset == 2) { tunings = PRESET2; };
  if (preset == 3) { tunings = PRESET3; };
  if (preset == 4) { tunings = PRESET4; };

  String t_str = "";
  if (tunings[5] > 12) { t_str += "+"; };
  t_str = t_str + tunings[5];

  String cap_str = "";
  if (tunings[6] > 0) { cap_str += "+"; };
  cap_str = cap_str + tunings[6];

  print_tuning("Preset Tuning",
               getLongNoteNum(tunings[0]),
               getLongNoteNum(tunings[1]),
               getLongNoteNum(tunings[2]),
               getLongNoteNum(tunings[3]),
               t_str, cap_str);
  delay(150);

  bool done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myAButton->update();
    myXButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      load_preset_tunings(preset);
      signal_scene_change(preset - 1); // Zero indexed
      done = true;

    } else if (my2Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

// This screen asks the user to select a save slot for
// preview and optionally choosing.
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

// This screen lets the user choose a preset.
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

// This screen lets you choose how the gurdy indicates the turning to the controller
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

// This screen lets you choose what kind of display you want.
void playing_scr_screen() {
  bool done = false;
  while (!done) {

    print_menu_4("Choose Play Screen", "Big Note + Staff", "Staff + Big Note", "Big Note Only", "Staff Only");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 0;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Note + Staff Screen", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 1;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Staff + Note Screen", "Saved to EEPROM");
      delay(750);
      done = true;
    } else if (my3Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 2;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Big Note Screen", "Saved to EEPROM");
      delay(750);
      done = true;
    } else if (my4Button->wasPressed()) {

      play_screen_type = ((play_screen_type / 10) * 10) + 3;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Choose Play Screen", "Staff Only Screen", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };
  };
};

// This screen is for other setup options.  Currently, that's
// just an option to clear the EEPROM.
void options_screen() {

  bool done = false;
  while (!done) {

    print_menu_3("Options", "Clear EEPROM", "Scene Control", "About Digi-Gurdy");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      clear_eeprom();

      print_message_2("Clear EEPROM", "EEPROM Cleared,", "(Defaults Reset)");
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {
      scene_options_screen();
      done = true;
    } else if (my3Button->wasPressed()) {
      options_about_screen();
      done = true;
    } else if (myXButton->wasPressed() || my4Button->wasPressed()) {
      done = true;
    };
  };
};

// This is the first screen after the credits n' stuff.
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
      mycrank->enableLED();

      print_message_2("Buzz LED On/Off", "Buzz LED On", "Saved to EEPROM");
      delay(1000);

      done = true;

    } else if (my2Button->wasPressed()) {
      EEPROM.write(EEPROM_BUZZ_LED, 0);
      mycrank->disableLED();

      print_message_2("Buzz LED On/Off", "Buzz LED Off", "Saved to EEPROM");
      delay(1000);

      done = true;

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      done = true;

    };
  };
};

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

void playing_config_screen() {
  bool done = false;
  while (!done) {

    print_menu_4("Screen Configuration", "Notation (ABC/DoReMi)", "Enable Buzz Indicator", "Disable Buzz Indicator", "Choose Play Screen");
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      notation_config_screen();

    } else if (my2Button->wasPressed()) {
      play_screen_type = 10 + (play_screen_type % 10);
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Play Screen Options", "Buzz Indicator Enabled", "Saved to EEPROM");
      delay(750);
      done = true;

    } else if (my3Button->wasPressed()) {

      play_screen_type = play_screen_type % 10;
      EEPROM.write(EEPROM_DISPLY_TYPE, play_screen_type);

      print_message_2("Play Screen Options", "Buzz Indicator Disabled", "Saved to EEPROM");
      delay(750);
      done = true;
    } else if (my4Button->wasPressed()) {
      playing_scr_screen();

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      done = true;
    };
  };
};

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

      print_message_2("Note Notation", "Solfrege (DoReMi)", "Saved to EEPROM");
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
