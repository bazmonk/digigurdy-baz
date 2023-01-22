#include "ex_screens.h"

/// @ingroup pause
/// @brief  Prompt user to choose which EX button to configure.
void ex_btn_choice_screen() {

  bool sel_1 = false;
  bool sel_2 = false;
  bool sel_3 = false;
  bool sel_4 = false;
  bool sel_5 = false;
  bool sel_6 = false;
  bool sel_BB = false;

  print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
  delay(150);
  
  while (true) {

    #ifndef USE_GEARED_CRANK
    ex1Button->update();
    ex2Button->update();
    ex3Button->update();
    #endif

    ex4Button->update();
    ex5Button->update();
    ex6Button->update();
    bigButton->update();
    myXButton->update();
    my1Button->update();

    if (ex4Button->wasPressed()) {
      if (sel_4) {
        if (ex4Button->fn_choice_screen()) {
          print_message_2("EX Buttons", "EX4 Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_4 = true;
        print_message_2("EX Buttons", "Click a button to view...", String("EX4: ") + ex4Button->printFunc());
        delay(150);
      };
      
    } else if (ex5Button->wasPressed()) {
      if (sel_5) {
        if (ex5Button->fn_choice_screen()) {
          print_message_2("EX Buttons", "EX5 Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_5 = true;
        print_message_2("EX Buttons", "Click a button to view...", String("EX5: ") + ex5Button->printFunc());
        delay(150);
      };

    } else if (ex6Button->wasPressed()) {
      if (sel_6) {
        if (ex6Button->fn_choice_screen()) {
          print_message_2("EX Buttons", "EX6 Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_6 = true;
        print_message_2("EX Buttons", "Click a button to view...", String("EX6: ") + ex6Button->printFunc());
        delay(150);
      };

    } else if (bigButton->wasPressed()) {
      if (sel_BB) {
        if (bigButton->fn_choice_screen()) {
          print_message_2("EX Buttons", "Big Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_BB = true;
        print_message_2("EX Buttons", "Click a button to view...", String("Big: ") + bigButton->printFunc());
        delay(150);
      };

    #ifndef USE_GEARED_CRANK 

    } else if (ex1Button->wasPressed()) {
      if (sel_1) {
        if (ex1Button->fn_choice_screen()) {
          print_message_2("EX Buttons", "EX1 Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_1 = true;
        print_message_2("EX Buttons", "Click a button to view...", String("EX1: ") + ex1Button->printFunc());
        delay(150);
      };
    } else if (ex2Button->wasPressed()) {
      if (sel_2) {
        if (ex2Button->fn_choice_screen()) {
          print_message_2("EX Buttons", "EX2 Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_2 = true;
        print_message_2("EX Buttons", "Click a button to view...", String("EX2: ") + ex2Button->printFunc());
        delay(150);
      };
    } else if (ex3Button->wasPressed()) {
      if (sel_3) {
        if (ex3Button->fn_choice_screen()) {
          print_message_2("EX Buttons", "EX3 Button", "Saved!");
          delay(1000);
          print_message_3("EX Buttons", "Click a button to view...", "X) Go Back", "1) Reset All Buttons");
        };
      } else {
        sel_1 = sel_2 = sel_3 = sel_4 = sel_5 = sel_6 = sel_BB = false;
        sel_3 = true;
        print_message_2("EX Buttons", "Click a button to view...", String("EX3: ") + ex3Button->printFunc());
        delay(150);
      };

    #endif
    
    } else if (myXButton->wasPressed()) {
      return;
    } else if (my1Button->wasPressed()) {

      print_message_2("EX Buttons", "Reset all buttons?", "1) Confirm, X) Go Back");
      delay(500);

      while (true) {
        
        my1Button->update();
        myXButton->update();

        if (my1Button->wasPressed()) {
          reset_ex_eeprom();

          #ifndef USE_GEARED_CRANK
          ex1Button->setFunc(EEPROM.read(EEPROM_EX1));
          ex2Button->setFunc(EEPROM.read(EEPROM_EX2));
          ex3Button->setFunc(EEPROM.read(EEPROM_EX3));
          #endif
          ex4Button->setFunc(EEPROM.read(EEPROM_EX4));
          ex5Button->setFunc(EEPROM.read(EEPROM_EX5));
          ex6Button->setFunc(EEPROM.read(EEPROM_EX6));
          bigButton->setFunc(EEPROM.read(EEPROM_EXBB));

          print_message_2("EX Buttons", "All buttons reset", "and saved!!!");
          delay(1000);
          return;
        } else if (myXButton->wasPressed()) {
          return;
        };
      };
    };
  };

  return;
};
