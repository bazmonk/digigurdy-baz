#include "tuning_screens.h"

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
