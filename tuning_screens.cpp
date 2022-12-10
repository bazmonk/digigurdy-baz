#include "tuning_screens.h"

bool tuning() {

  bool done = false;
  while (!done) {

    if (use_solfege == 0) {
      print_menu_4("Tuning Menu", "G/C, Guided", "D/G, Guided", "Manual Setup", "Volume Control");
    } else if (use_solfege == 1) {
      print_menu_4("Tuning Menu", "Sol/Do, Guided", "Re/Sol, Guided", "Manual Setup", "Volume Control");
    } else {
      print_menu_4("Tuning Menu", "G/C (Sol/Do), Guided", "D/G (Re/Sol), Guided", "Manual Setup", "Volume Control");
    };
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

// This screen allows the user to make manual changes to each string.
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

// This is the tuning screen for an individual string.  Single argument is a GurdyString pointer.
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
