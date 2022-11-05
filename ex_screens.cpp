#include "ex_screens.h"

void ex_btn_choice_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " -----EX Buttons---- \n"
                      " Choose Button:      \n"
                      "                     \n"
                      "   1)   EX1 (Left)   \n"
                      "   2)   EX2          \n"
                      "   3)   EX3 (Right)  \n"
                      "                     \n"
                      " X or 4) Go Back     \n";

    print_screen(disp_str);
    delay(150);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      ex1Button->fn_choice_screen(1);

      String disp_str = " -----EX Buttons---- \n"
                        "                     \n"
                        "                     \n"
                        "      EX1 Button     \n"
                        "        Saved!       \n"
                        "                     \n"
                        "                     \n"
                        "                     \n";

      print_screen(disp_str);
      delay(1000);

    } else if (my2Button->wasPressed()) {
      ex2Button->fn_choice_screen(2);

      String disp_str = " -----EX Buttons---- \n"
                        "                     \n"
                        "                     \n"
                        "      EX1 Button     \n"
                        "        Saved!       \n"
                        "                     \n"
                        "                     \n"
                        "                     \n";

      print_screen(disp_str);
      delay(1000);

    } else if (my3Button->wasPressed()) {
      ex3Button->fn_choice_screen(3);


      String disp_str = " -----EX Buttons---- \n"
                        "                     \n"
                        "                     \n"
                        "      EX1 Button     \n"
                        "        Saved!       \n"
                        "                     \n"
                        "                     \n"
                        "                     \n";

      print_screen(disp_str);
      delay(1000);

    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
