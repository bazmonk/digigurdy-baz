#include "ex_screens.h"

/// @ingroup pause
/// @brief  Prompt user to choose which EX button to configure.
void ex_btn_choice_screen() {

  bool done = false;
  while (!done) {

    #ifdef USE_GEARED_CRANK

    print_menu_5("EX Buttons",
                 String("EX4 - ") + ex4Button->printFunc(),
                 String("EX5 - ") + ex5Button->printFunc(),
                 String("EX6 - ") + ex6Button->printFunc(),
                 String("Big - ") + bigButton->printFunc(),
                 "Reset All Buttons");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex4Button->fn_choice_screen(4);

      print_message_2("EX Buttons", "EX4 Button", "Saved!");
      delay(1000);

    } else if (my2Button->wasPressed()) {
      ex5Button->fn_choice_screen(5);

      print_message_2("EX Buttons", "EX5 Button", "Saved!");
      delay(1000);

    } else if (my3Button->wasPressed()) {
      ex6Button->fn_choice_screen(6);

      print_message_2("EX Buttons", "EX6 Button", "Saved!");
      delay(1000);

    } else if (my4Button->wasPressed()) {
      bigButton->fn_choice_screen(0);

      print_message_2("EX Buttons", "Big Button", "Saved!");
      delay(1000);

    } else if (my5Button->wasPressed()) {
      reset_ex_eeprom();

      ex4Button->setFunc(EEPROM.read(EEPROM_EX4));
      ex5Button->setFunc(EEPROM.read(EEPROM_EX5));
      ex6Button->setFunc(EEPROM.read(EEPROM_EX6));
      bigButton->setFunc(EEPROM.read(EEPROM_EXBB));

      print_message_2("EX Buttons", "All Buttons", "Reset and Saved!");
      delay(1000);

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };

    #else

    print_menu_4("EX Buttons",
                 String("EX1 - ") + ex1Button->printFunc(),
                 String("EX2 - ") + ex2Button->printFunc(),
                 String("EX3 - ") + ex3Button->printFunc(),
                 "Next Page");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex1Button->fn_choice_screen();

      print_message_2("EX Buttons", "EX1 Button", "Saved!");
      delay(1000);

    } else if (my2Button->wasPressed()) {
      ex2Button->fn_choice_screen();

      print_message_2("EX Buttons", "EX2 Button", "Saved!");
      delay(1000);

    } else if (my3Button->wasPressed()) {
      ex3Button->fn_choice_screen();

      print_message_2("EX Buttons", "EX3 Button", "Saved!");
      delay(1000);

    } else if (my4Button->wasPressed()) {

      bool done2 = false;
      while (!done2) {

        print_menu_5("EX Buttons II",
                  String("EX4 - ") + ex4Button->printFunc(),
                  String("EX5 - ") + ex5Button->printFunc(),
                  String("EX6 - ") + ex6Button->printFunc(),
                  String("Big - ") + bigButton->printFunc(),
                  "Reset All Buttons");
        delay(150);

        my1Button->update();
        my2Button->update();
        my3Button->update();
        my4Button->update();
        my5Button->update();
        my6Button->update();
        myXButton->update();

        if (my1Button->wasPressed()) {
          ex4Button->fn_choice_screen();

          print_message_2("EX Buttons", "EX4 Button", "Saved!");
          delay(1000);

        } else if (my2Button->wasPressed()) {
          ex5Button->fn_choice_screen();

          print_message_2("EX Buttons", "EX5 Button", "Saved!");
          delay(1000);

        } else if (my3Button->wasPressed()) {
          ex6Button->fn_choice_screen();

          print_message_2("EX Buttons", "EX6 Button", "Saved!");
          delay(1000);

        } else if (my4Button->wasPressed()) {
          bigButton->fn_choice_screen();

          print_message_2("EX Buttons", "Big Button", "Saved!");
          delay(1000);

        } else if (my5Button->wasPressed()) {
          reset_ex_eeprom();

          ex1Button->setFunc(EEPROM.read(EEPROM_EX1));
          ex2Button->setFunc(EEPROM.read(EEPROM_EX2));
          ex3Button->setFunc(EEPROM.read(EEPROM_EX3));
          ex4Button->setFunc(EEPROM.read(EEPROM_EX4));
          ex5Button->setFunc(EEPROM.read(EEPROM_EX5));
          ex6Button->setFunc(EEPROM.read(EEPROM_EX6));
          bigButton->setFunc(EEPROM.read(EEPROM_EXBB));

          print_message_2("EX Buttons", "All Buttons", "Reset and Saved!");
          delay(1000);
      
        } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
          done2 = true;
        };
      };

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
    #endif

  };
  return;
};
