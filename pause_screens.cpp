#include "pause_screens.h"

// This is the screen that X+A gets you.
void pause_screen() {

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

bool tuning() {

  bool done = false;
  while (!done) {

    String disp_str = " ----Tuning Menu---- \n"
                      "  1) G/C, Guided     \n"
                      "  2) D/G, Guided     \n"
                      "  3) Manual Setup    \n"
                      "                     \n"
                      "  4) Volume Control  \n"
                      "                     \n"
                      "  X or 5) Go Back    \n";

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
      gc_or_dg = true;
      done = true;

    } else if (my2Button->wasPressed()) {
      gc_or_dg = false;
      done = true;

    } else if (my3Button->wasPressed()) {
      manual_tuning_screen();
    } else if (my4Button->wasPressed()) {
      volume_screen();
    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  tuning_hi_melody();
  tuning_low_melody();
  tuning_drone();
  tuning_tromp();

  // SUMMARY
  String disp_str = " -----Summary:------ \n"
                    "  High Melody:   " + NoteNum[mystring->getOpenNote()] + "  \n"
                    "   Low Melody:   " + NoteNum[mylowstring->getOpenNote()] + "  \n"
                    "        Drone:   " + NoteNum[mydrone->getOpenNote()] + "  \n"
                    "    Trompette:   " + NoteNum[mytromp->getOpenNote()] + "  \n"
                    "                     \n"
                    "A or 1) Accept     \n"
                    "X or 2) Go Back    \n";

  print_screen(disp_str);

  delay(150);

  done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myAButton->update();
    myXButton->update();

    if (myAButton->wasPressed() || my1Button->wasPressed()) {
      done = true;
      return true;

    } else if (myXButton->wasPressed() || my2Button->wasPressed()) {
      return false;
      done = true;
    };
  };
  return true;
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
                      "                     \n"
                      " 1)   EX Button      \n"
                      "    Configuration    \n\n"
                      " 2) About DigiGurdy  \n\n"
                      "  X or 3) Go Back    \n";

    print_screen(disp_str);
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex_btn_choice_screen();

    } else if (my2Button->wasPressed()) {
      options_about_screen();

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
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
};

void tuning_hi_melody() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;

  String disp_str = "         ";
  if (gc_or_dg) {
    base_note = Note(g4);
    disp_str += "G/C         \n";
  } else {
    base_note = Note(d4);
    disp_str += "D/G         \n";
  };

  choice1 = base_note + 12;
  choice2 = base_note + 17;
  choice3 = base_note;
  choice4 = base_note + 5;

  disp_str += " Choose High Melody: \n"
              "                     \n"
              " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
              " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
              "                     \n"
              "                     \n"
              " A) Default (**)     \n";

  print_screen(disp_str);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mystring->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mystring->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mystring->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mystring->setOpenNote(choice4);
      done = true;
    };
  };
};

void tuning_low_melody() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;

  if (gc_or_dg) {
    base_note = Note(g4);
  } else {
    base_note = Note(d4);
  };

  choice1 = base_note;
  choice2 = base_note - 7;
  choice3 = base_note - 12;
  choice4 = base_note - 19;

  String disp_str = " High Melody:   " + NoteNum[mystring->getOpenNote()] + "   \n"
                    " Choose Low Melody:  \n"
                    "                     \n"
                    " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
                    " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
                    "                     \n"
                    "                     \n"
                    " A) Default (**)     \n"
                    "                     \n";

  print_screen(disp_str);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mylowstring->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mylowstring->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mylowstring->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mylowstring->setOpenNote(choice4);
      done = true;
    };
  };
};

void tuning_drone() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;
  int choice5;
  int choice6;

  if (gc_or_dg) {
    base_note = Note(g4);
    choice1 = base_note - 31;
    choice2 = base_note - 24;
    choice3 = base_note - 19;
    choice4 = base_note - 12;
    choice5 = base_note - 7;
    choice6 = base_note;

  } else {
    base_note = Note(d4);
    choice1 = base_note - 19;
    choice2 = base_note - 24;
    choice3 = base_note - 7;
    choice4 = base_note - 12;
    choice5 = base_note + 5;
    choice6 = base_note;
  };

  String disp_str = " Hi/Lo Melody: " + NoteNum[mystring->getOpenNote()] + "/" + NoteNum[mylowstring->getOpenNote()] + " \n"
                    "    Choose Drone:    \n"
                    " 1) " + NoteNum[choice1] + "     2) " + NoteNum[choice2] + "     \n"
                    " 3) " + NoteNum[choice3] + " **  4) " + NoteNum[choice4] + "     \n"
                    " 5) " + NoteNum[choice5] + "     6) " + NoteNum[choice6] + "     \n"
                    "                     \n"
                    "                     \n"
                    " A) Default (**)     \n";

  print_screen(disp_str);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myAButton->update();

    if (my1Button->wasPressed()) {
      mydrone->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mydrone->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed() || myAButton->wasPressed()) {
      mydrone->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mydrone->setOpenNote(choice4);
      done = true;
    } else if (my5Button->wasPressed()) {
      mydrone->setOpenNote(choice5);
      done = true;
    } else if (my6Button->wasPressed()) {
      mydrone->setOpenNote(choice6);
      done = true;
    };
  };
};

void tuning_tromp() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;
  int choice5;
  int choice6;

  if (gc_or_dg) {
    base_note = Note(g4);
  } else {
    base_note = Note(d4);
  };

  choice1 = base_note + 5;
  choice2 = base_note + 12;
  choice3 = base_note - 7;
  choice4 = base_note;
  choice5 = base_note - 19;
  choice6 = base_note - 12;

  String disp_str = " Hi/Lo Melody: " + NoteNum[mystring->getOpenNote()] + "/" + NoteNum[mylowstring->getOpenNote()] + " \n"
                    " Drone: " + NoteNum[mydrone->getOpenNote()] + "           \n"
                    "  Choose Trompette:  \n"
                    " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
                    " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
                    " 5) " + NoteNum[choice5] + "     6) " + NoteNum[choice6] + "     \n"
                    "                     \n"
                    " A) Default (**)     \n";

  print_screen(disp_str);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mytromp->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mytromp->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mytromp->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mytromp->setOpenNote(choice4);
      done = true;
    } else if (my5Button->wasPressed()) {
      mytromp->setOpenNote(choice5);
      done = true;
    } else if (my6Button->wasPressed()) {
      mytromp->setOpenNote(choice6);
      done = true;
    };

    // I think the buzz sounds best between D3 and D4, so this pops
    // the buzz tone up and down octaves until it lands there.
    int buzz_note = mytromp->getOpenNote();
    while (buzz_note > Note(d4)) {
      buzz_note -= 12;
    };
    while (buzz_note < Note(d3)) {
      buzz_note += 12;
    };
    mybuzz->setOpenNote(buzz_note);
  };
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
                    " Drone:     " + LongNoteNum[tunings[2]] + "  \n"
                    " Trompette: " + LongNoteNum[tunings[3]] + "  \n"
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

// This is the tuning screen for an individual string.  Single argument is a GurdyString pointer.
void tune_string_screen(GurdyString *this_string) {
  bool done = false;
  int new_note = this_string->getOpenNote();
  delay(300);
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ---String Tuning--- \n"
    " 1) Tune Down        \n"
    " 2) Tune Up          \n\n";

    display.print(disp_str);

    display.setTextSize(2);
    disp_str = ""
    "  " + LongNoteNum[new_note] + "\n";

    display.print(disp_str);

    display.setTextSize(1);
    disp_str = "\n"
    " X) Done / Go Back   \n";

    display.print(disp_str);

    display.display();

    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (new_note > 24) {
        new_note -= 1;
        delay(300);
      };
    } else if (my1Button->beingPressed()) {
      if (new_note > 24) {
        new_note -= 1;
        delay(150);
      };
    } else if (my2Button->wasPressed()) {
      if (new_note < 111) {
        new_note += 1;
        delay(300);
      };
    } else if (my2Button->beingPressed()) {
      if (new_note < 111) {
        new_note += 1;
        delay(150);
      };
    } else if (myXButton->wasPressed()) {
      this_string->setOpenNote(new_note);
      done = true;
    };
  };
};

// This screen allows the user to make manual changes to each string.
void manual_tuning_screen() {

  while (true) {

    String disp_str = " ---Manual Tuning--- \n"
                      " Choose a string:    \n"
                      " 1) Hi Mel.- " + NoteNum[mystring->getOpenNote()] + " \n"
                      " 2) Lo Mel.- " + NoteNum[mylowstring->getOpenNote()] + " \n"
                      " 3) Drone. - " + NoteNum[mydrone->getOpenNote()] + " \n"
                      " 4) Tromp. - " + NoteNum[mytromp->getOpenNote()] + " \n"
                      " 5) Buzz   - " + NoteNum[mybuzz->getOpenNote()] + "  \n"
                      " X or 6) Go Back     \n";

    print_screen(disp_str);
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
      tune_string_screen(mystring);

    } else if (my2Button->wasPressed()) {
      tune_string_screen(mylowstring);

    } else if (my3Button->wasPressed()) {
      tune_string_screen(mydrone);

    } else if (my4Button->wasPressed()) {
      tune_string_screen(mytromp);

    } else if (my5Button->wasPressed()) {
      tune_string_screen(mybuzz);

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

void change_volume_screen(GurdyString *this_string) {
  bool done = false;
  int new_vol = this_string->getVolume();
  delay(300);
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = " ---String Volume--- \n"
                      " 1) Volume Down      \n"
                      " 2) Volume Up        \n\n";

    display.print(disp_str);

    display.setTextSize(2);
    disp_str = String("   ") + new_vol + "\n";

    display.print(disp_str);

    display.setTextSize(1);
    disp_str = "\n"
    " X) Done / Go Back   \n";

    display.print(disp_str);

    display.display();

    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (new_vol > 0) {
        new_vol -= 1;
        delay(300);
      };
    } else if (my1Button->beingPressed()) {
      if (new_vol > 0) {
        new_vol -= 1;
        delay(100);
      };
    } else if (my2Button->wasPressed()) {
      if (new_vol < 127) {
        new_vol += 1;
        delay(300);
      };
    } else if (my2Button->beingPressed()) {
      if (new_vol < 127) {
        new_vol += 1;
        delay(100);
      };
    } else if (myXButton->wasPressed()) {
      this_string->setVolume(new_vol);
      done = true;
    };
  };
};

// This screen allows the user to make manual changes to each string's volume.
void volume_screen() {

  while (true) {

    String disp_str = String("") +
                      " ------Volume------- \n"
                      " 1) Hi Mel.- " + mystring->getVolume() + " \n"
                      " 2) Lo Mel.- " + mylowstring->getVolume() + " \n"
                      " 3) Drone. - " + mydrone->getVolume() + " \n"
                      " 4) Tromp. - " + mytromp->getVolume() + " \n"
                      " 5) Buzz   - " + mybuzz->getVolume() + " \n"
                      " 6) Click  - " + mykeyclick->getVolume() + " \n"
                      " X) Go Back     \n";

    print_screen(disp_str);
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
      change_volume_screen(mystring);

    } else if (my2Button->wasPressed()) {
      change_volume_screen(mylowstring);

    } else if (my3Button->wasPressed()) {
      change_volume_screen(mydrone);

    } else if (my4Button->wasPressed()) {
      change_volume_screen(mytromp);

    } else if (my5Button->wasPressed()) {
      change_volume_screen(mybuzz);

    } else if (my6Button->wasPressed()) {
      change_volume_screen(mykeyclick);

    } else if (myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
