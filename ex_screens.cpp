#include "ex_screens.h"

/// @ingroup pause
/// @brief  Prompt user to choose which EX button to configure.
void ex_btn_choice_screen() {

  bool done = false;
  while (!done) {

    print_menu_6("EX Buttons",
                 "EX1 (L)",
                 "EX2",
                 "EX3",
                 "EX4 (a.k.a. Tpose Down",
                 "EX5 (a.k.a. Tpose Up",
                 "EX6 (a.k.a. Capo)");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex1Button->fn_choice_screen(1);

      print_message_2("EX Buttons", "EX1 Button", "Saved!");
      delay(1000);

    } else if (my2Button->wasPressed()) {
      ex2Button->fn_choice_screen(2);

      print_message_2("EX Buttons", "EX2 Button", "Saved!");
      delay(1000);

    } else if (my3Button->wasPressed()) {
      ex3Button->fn_choice_screen(3);

      print_message_2("EX Buttons", "EX3 Button", "Saved!");
      delay(1000);

    } else if (my4Button->wasPressed()) {
      ex4Button->fn_choice_screen(4);

      print_message_2("EX Buttons", "EX4 Button", "Saved!");
      delay(1000);

    } else if (my5Button->wasPressed()) {
      ex5Button->fn_choice_screen(5);

      print_message_2("EX Buttons", "EX5 Button", "Saved!");
      delay(1000);

    } else if (my6Button->wasPressed()) {
      ex6Button->fn_choice_screen(6);

      print_message_2("EX Buttons", "EX6 Button", "Saved!");
      delay(1000);

    } else if (myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
