#include "tuning_screens.h"

/// @defgroup tuning Tuning Screens
/// These menus concern the tuning and volume of the GurdyString objects.
/// @warning These functions are hardcoded to expect certain string objects to exist.
/// @{

/// @brief Prompts the user to choose between the primary tunining options: guided tuinings, manual tunings, volume control, MIDI channel assignment.
/// @return True if an option was chosen, false otherwise.
bool tuning() {

  bool done = false;
  while (!done) {

    if (use_solfege == 0) {
      print_menu_5("Tuning Menu", "G/C, Guided", "D/G, Guided", "Manual Setup", "Volume Control", "MIDI Channels");
    } else if (use_solfege == 1) {
      print_menu_5("Tuning Menu", "Sol/Do, Guided", "Re/Sol, Guided", "Manual Setup", "Volume Control", "MIDI Channels");
    } else {
      print_menu_5("Tuning Menu", "G/C (Sol/Do), Guided", "D/G (Re/Sol), Guided", "Manual Setup", "Volume Control", "MIDI Channels");
    };
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
      gc_or_dg = true;
      done = true;

    } else if (my2Button->wasPressed()) {
      gc_or_dg = false;
      done = true;

    } else if (my3Button->wasPressed()) {
      manual_tuning_screen();
    } else if (my4Button->wasPressed()) {
      volume_screen();
    } else if (my5Button->wasPressed()) {
      channel_screen();
    } else if (myXButton->wasPressed()) {
      return false;
    } else if (my6Button->wasPressed()) {
      cool_kids_screen();
    };
  };

  tuning_hi_melody();
  tuning_low_melody();
  tuning_tromp();
  tuning_drone();

  print_tuning_summary(mystring->getOpenNote(), mylowstring->getOpenNote(),
                       mytromp->getOpenNote(), mydrone->getOpenNote());
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

/// @brief Prompts the user to choose beween high melody string choices.
/// Part of the guided tuining menu tree.
void tuning_hi_melody() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;

  if (gc_or_dg) {
    base_note = Note(g3);
  } else {
    base_note = Note(d3);
  };

  choice1 = base_note;
  choice2 = base_note + 12;
  choice3 = base_note + 24;

  print_tuning_choice_3("Choose Hi Melody", choice2, choice3, choice1);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mystring->setOpenNote(choice2);
      done = true;
    } else if (my2Button->wasPressed()) {
      mystring->setOpenNote(choice3);
      done = true;
    } else if (my3Button->wasPressed()) {
      mystring->setOpenNote(choice1);
      done = true;
    };
  };
};

/// @brief Prompts the user to choose between low melody string tunings based off the high melody tuning.
/// Part of the guided tuning menu tree.
void tuning_low_melody() {

  int base_note = mystring->getOpenNote();
  int choice1 = base_note - 12;
  int choice2 = base_note;
  int choice3 = base_note - 7;

  print_tuning_choice_3("Choose Low Melody", choice1, choice2, choice3);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
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
    };
  };
};

/// @brief Prompts the user to choose between trompette string tunings based off the current high melody string tuning.
void tuning_tromp() {

  int base_note = mystring->getOpenNote() - 12;
  
  int choice1 = base_note;
  int choice2 = base_note + 12;
  int choice3 = base_note - 7;

  print_tuning_choice_3("Choose Trompette", choice1, choice2, choice3);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
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

/// @brief Prompts the user to choose between drone string tunings based off the current high melody string tuning.
void tuning_drone() {

  int base_note = mystring->getOpenNote() - 12;
  int choice1 = base_note - 12;
  int choice2 = base_note - 19;
  int choice3 = base_note - 7;

  print_tuning_choice_3("Choose Drone", choice1, choice2, choice3);
  delay(150);

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mydrone->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mydrone->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mydrone->setOpenNote(choice3);
      done = true;
    };
  };
};

/// @brief Prompts the user to choose a string to adjust the tuning of manually.
void manual_tuning_screen() {

  while (true) {

    print_menu_5("Manual Tuning",
                 String("Hi Melody - ") + getLongNoteNum(mystring->getOpenNote()),
                 String("Low Melody - ") + getLongNoteNum(mylowstring->getOpenNote()),
                 String("Trompette - ") + getLongNoteNum(mytromp->getOpenNote()),
                 String("Drone - ") + getLongNoteNum(mydrone->getOpenNote()),
                 String("Buzz - ") + getLongNoteNum(mybuzz->getOpenNote()));
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
      tune_string_screen(mytromp);

    } else if (my4Button->wasPressed()) {
      tune_string_screen(mydrone);

    } else if (my5Button->wasPressed()) {
      tune_string_screen(mybuzz);

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

/// @brief Prompts the user to manually tune the given string.
/// @param this_string The GurdyString object to be tuned
void tune_string_screen(GurdyString *this_string) {
  bool done = false;
  int new_note = this_string->getOpenNote();
  delay(300);
  while (!done) {

    print_value_selection("Tuning - " + this_string->getName(), getLongNoteNum(new_note));

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

// This screen allows the user to make manual changes to each string's volume.

/// @brief Promts the user to choose a string to adjust the volume of manually.
void volume_screen() {

  while (true) {

    print_menu_6("Volume",
                 String("Hi Melody - ") + mystring->getVolume(),
                 String("Low Melody - ") + mylowstring->getVolume(),
                 String("Trompette - ") + mytromp->getVolume(),
                 String("Drone - ") + mydrone->getVolume(),
                 String("Buzz - ") + mybuzz->getVolume(),
                 String("Key Click - ") + mykeyclick->getVolume());
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
      change_volume_screen(mytromp);

    } else if (my4Button->wasPressed()) {
      change_volume_screen(mydrone);

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

/// @brief Prompts the user to adjust the volume of the given string.
/// @param this_string The GurdyString object to adjust the volume of
void change_volume_screen(GurdyString *this_string) {
  bool done = false;
  int new_vol = this_string->getVolume();
  delay(300);
  while (!done) {

    print_value_selection("Volume - " + this_string->getName(), new_vol);

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

// This screen allows the user to make manual changes to each string's MIDI channel assignment.

/// @brief Promts the user to choose a string to change the MIDI channel of.
void channel_screen() {

  while (true) {

    print_menu_6("MIDI Channel",
                 String("Hi Melody - ") + mystring->getChannel(),
                 String("Low Melody - ") + mylowstring->getChannel(),
                 String("Trompette - ") + mytromp->getChannel(),
                 String("Drone - ") + mydrone->getChannel(),
                 String("Buzz - ") + mybuzz->getChannel(),
                 String("Key Click - ") + mykeyclick->getChannel());
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
      change_channel_screen(mystring);

    } else if (my2Button->wasPressed()) {
      change_channel_screen(mylowstring);

    } else if (my3Button->wasPressed()) {
      change_channel_screen(mytromp);

    } else if (my4Button->wasPressed()) {
      change_channel_screen(mydrone);

    } else if (my5Button->wasPressed()) {
      change_channel_screen(mybuzz);

    } else if (my6Button->wasPressed()) {
      change_channel_screen(mykeyclick);

    } else if (myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

/// @brief Prompts the user to adjust the volume of the given string.
/// @param this_string The GurdyString object to adjust the volume of
void change_channel_screen(GurdyString *this_string) {
  bool done = false;
  int new_cha = this_string->getChannel();
  delay(300);
  while (!done) {

    print_value_selection("MIDI Channel - " + this_string->getName(), new_cha);

    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (new_cha > 1) {
        new_cha -= 1;
        delay(300);
      };
    } else if (my1Button->beingPressed()) {
      if (new_cha > 1) {
        new_cha -= 1;
        delay(100);
      };
    } else if (my2Button->wasPressed()) {
      if (new_cha < 16) {
        new_cha += 1;
        delay(300);
      };
    } else if (my2Button->beingPressed()) {
      if (new_cha < 16) {
        new_cha += 1;
        delay(100);
      };
    } else if (myXButton->wasPressed()) {
      this_string->setChannel(new_cha);
      done = true;
    };
  };
};

/// @brief Prompts the user to add additional tones to each string (and buzz).
/// @details This is a "hidden" feature.
void cool_kids_screen() {

  while (true) {

    print_menu_5("Second String Tones",
                 String("Hi Mel. - ") + mystring->getGrosString(),
                 String("Low Mel. - ") + mylowstring->getGrosString(),
                 String("Tromp. - ") + mytromp->getGrosString(),
                 String("Drone - ") + mydrone->getGrosString(),
                 String("Buzz - ") + mybuzz->getGrosString());
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
      if (mystring->getGrosMode() == 3) {
        mystring->setGrosMode(0);
      } else { 
        mystring->setGrosMode(mystring->getGrosMode() + 1);
      };

    } else if (my2Button->wasPressed()) {
      if (mylowstring->getGrosMode() == 3) {
        mylowstring->setGrosMode(0);
      } else { 
        mylowstring->setGrosMode(mylowstring->getGrosMode() + 1);
      };

    } else if (my3Button->wasPressed()) {
      if (mytromp->getGrosMode() == 3) {
        mytromp->setGrosMode(0);
      } else { 
        mytromp->setGrosMode(mytromp->getGrosMode() + 1);
      };

    } else if (my4Button->wasPressed()) {
      if (mydrone->getGrosMode() == 3) {
        mydrone->setGrosMode(0);
      } else { 
        mydrone->setGrosMode(mydrone->getGrosMode() + 1);
      };

    } else if (my5Button->wasPressed()) {
      if (mybuzz->getGrosMode() == 3) {
        mybuzz->setGrosMode(0);
      } else { 
        mybuzz->setGrosMode(mybuzz->getGrosMode() + 1);
      };

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
