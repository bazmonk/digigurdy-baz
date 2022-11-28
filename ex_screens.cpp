#include "ex_screens.h"

void ex_btn_choice_screen() {

  bool done = false;
  while (!done) {

    print_menu_3("EX Buttons",
                 "EX1 (Left)",
                 "EX2",
                 "EX3 (Right)");
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
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

      print_message_2("EX Buttons", "EX2 Button", "Saved!");
      delay(1000);

    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
