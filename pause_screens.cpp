#include "pause_screens.h"

// This is the screen that X+A gets you.
void pause_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " ----Pause  Menu---- \n"
                      " 1) Load    2) Save  \n"
                      " 3) Tuning  4) Other \n\n"
                      "   X or 5) Go Back\n\n";

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
  display.clearDisplay();
  display.drawBitmap(0, 0, crank_on_logo, 128, 64, 1);
  display.display();
  delay(500);
};


// This screen prompts which kind of tuning to load.
bool load_tuning_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " ----Load Tuning---- \n"
                      "                     \n"
                      " 1) Preset Tuning    \n"
                      "                     \n"
                      " 2) Saved Tuning     \n\n"
                      " X or 3) Go Back     \n";

    print_screen(disp_str);

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

    String disp_str = " ----Save Tuning---- \n"
                      "                     \n"
                      " Save slot is full,  \n"
                      "    Save anyway?     \n"
                      "                     \n"
                      " 1) Overwrite        \n"
                      "                     \n"
                      " X or 2) Go Back     \n";

    print_screen(disp_str);
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

    String disp_str = " ----Save Tuning---- \n"
                      " Choose a Save Slot: \n"
                      "                     \n"
                      "      1) Slot 1      \n"
                      "      2) Slot 2      \n"
                      "      3) Slot 3      \n"
                      "      4) Slot 4      \n"
                      "      X) Go Back     \n";

    print_screen(disp_str);
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
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

    } else if (myXButton->wasPressed()) {
      // Just return.
      return;
    };
  };

  // Display a confirmation message
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("\n  Slot ");
  display.print(slot);
  display.print("\n  Saved!");
  display.display();
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

  String disp_str = "      DigiGurdy      \n"
                    "---------------------\n"
                    "Special Thanks:      \n"
                    "                     \n"
                    "John Dingley         \n"
                    "David Jacobs         \n"
                    "lune36400            \n"
                    "SalusaSecondus       ";

  print_screen(disp_str);

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };
};

bool other_options_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " ---Other Options--- \n"
                      " 1)   EX Button      \n"
                      "    Configuration    \n"
                      " 2) Buzz LED         \n"
                      " 3) Vibrato Pedal    \n"
                      " 4) About DigiGurdy  \n\n"
                      "  X or 5) Go Back    \n";

    print_screen(disp_str);
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
      #ifdef LED_KNOB
      led_screen();
      #endif

    } else if (my3Button->wasPressed()) {
      vib_screen();

    } else if (my4Button->wasPressed()) {
      options_about_screen();

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
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

  String disp_str = " -Saved Slot Tuning- \n"
                    " Hi Melody: " + LongNoteNum[EEPROM.read(slot + EEPROM_HI_MEL)] + "  \n"
                    " Lo Melody: " + LongNoteNum[EEPROM.read(slot + EEPROM_LO_MEL)] + "  \n"
                    " Drone:     " + LongNoteNum[EEPROM.read(slot + EEPROM_DRONE)] + "  \n"
                    " Trompette: " + LongNoteNum[EEPROM.read(slot + EEPROM_TROMP)] + "  \n"
                    " Tpose: ";

  if (EEPROM.read(slot + EEPROM_TPOSE) > 12) { disp_str += "+"; };
  disp_str = disp_str + ((EEPROM.read(slot + EEPROM_TPOSE))-12) + "  Capo: ";

  if (EEPROM.read(slot + EEPROM_CAPO) > 0) { disp_str += "+"; };
  disp_str = disp_str + (EEPROM.read(slot + EEPROM_CAPO)) + "\n"
             " A or 1) Accept \n"
             " X or 2) Go Back  \n";

  print_screen(disp_str);
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

  String disp_str = " ---Preset Tuning--- \n"
                    " Hi Melody: " + LongNoteNum[tunings[0]] + "  \n"
                    " Lo Melody: " + LongNoteNum[tunings[1]] + "  \n"
                    " Trompette: " + LongNoteNum[tunings[3]] + "  \n"
                    " Drone:     " + LongNoteNum[tunings[2]] + "  \n"
                    " Tpose: ";

  if (tunings[5] > 12) { disp_str += "+"; };
  disp_str = disp_str + tunings[5] + "  Capo: ";
  if (tunings[6] > 0) { disp_str += "+"; };
  disp_str = disp_str + tunings[6] + "\n"
             " A or 1) Accept  \n"
             " X or 2) Go Back  \n";

  print_screen(disp_str);
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

    String disp_str = " --Load Saved Slot-- \n"
                      " Select for Preview: \n"
                      "                     \n"
                      "     1) Slot 1       \n"
                      "     2) Slot 2       \n"
                      "     3) Slot 3       \n"
                      "     4) Slot 4       \n"
                      "X or 5) Go Back      \n";

    print_screen(disp_str);
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

    String disp_str = " ---Load A Preset--- \n"
                      " Select a Preset:    \n"
                      "                     \n"
                      " 1) " + PRESET1_NAME + "\n"
                      " 2) " + PRESET2_NAME + "\n"
                      " 3) " + PRESET3_NAME + "\n"
                      " 4) " + PRESET4_NAME + "\n"
                      " X or 5) Go Back     \n";

    print_screen(disp_str);
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

    String disp_str = " -Scene Signalliing- \n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) None             \n"
                      " 2) Prog Chg, Ch. 1  \n"
                      "                     \n"
                      " X) Go Back          \n";

    print_screen(disp_str);
    delay(150);

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
    delay(150);

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
    delay(150);

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
      // Not done = true here, we'd want to get prompted again.
    };
  };
};

void led_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " --Buzz LED On/Off-- \n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) LED On           \n"
                      " 2) LED Off          \n"
                      "                     \n"
                      " 3 or X) Go Back     \n"
                      "                     \n";

    print_screen(disp_str);
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      EEPROM.write(EEPROM_BUZZ_LED, 1);
      mycrank->enableLED();

      disp_str =  " --Buzz LED On/Off-- \n"
                  " Select an Option:   \n"
                  "                     \n"
                  "     LED ON SAVED    \n"
                  "                     \n"
                  "                     \n"
                  "                     \n"
                  "                     \n";

      print_screen(disp_str);
      delay(1000);

      done = true;

    } else if (my2Button->wasPressed()) {
      EEPROM.write(EEPROM_BUZZ_LED, 0);
      mycrank->disableLED();

      disp_str =  " --Buzz LED On/Off-- \n"
                  " Select an Option:   \n"
                  "                     \n"
                  "                     \n"
                  "     LED OFF SAVED   \n"
                  "                     \n"
                  "                     \n"
                  "                     \n";

      print_screen(disp_str);
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

    String disp_str = " ---Vibrato Pedal--- \n"
                      " Select an Option:   \n"
                      "                     \n"
                      " 1) Pedal On         \n"
                      " 2) Pedal Off        \n"
                      "                     \n"
                      " 3 or X) Go Back     \n"
                      "                     \n";

    print_screen(disp_str);
    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      myvibknob->enable();

      disp_str =  " ---Vibrato Pedal--- \n"
                  " Select an Option:   \n"
                  "                     \n"
                  "   PEDAL ON SAVED    \n"
                  "                     \n"
                  "                     \n"
                  "                     \n"
                  "                     \n";

      print_screen(disp_str);
      delay(1000);

      done = true;

    } else if (my2Button->wasPressed()) {
      myvibknob->disable();

      disp_str =  " ---Vibrato Pedal--- \n"
                  " Select an Option:   \n"
                  "                     \n"
                  "                     \n"
                  "   PEDAL OFF SAVED   \n"
                  "                     \n"
                  "                     \n"
                  "                     \n";

      print_screen(disp_str);
      delay(1000);

      done = true;

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      done = true;

    };
  };
};
