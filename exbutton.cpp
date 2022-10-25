#include "exbutton.h"

ExButton::ExButton(int my_pin, int func) : GurdyButton(my_pin) {
  my_func = func;
};

int ExButton::getFunc() {
  return my_func;
};

void ExButton::setFunc(int func) {
  my_func = func;
};

void ExButton::doFunc() {
  if (my_func == 1) {
    return;
  } else if (my_func == 2) {
    cycle_mel_mute();
  } else if (my_func == 3) {
    cycle_drone_tromp_mute();
  }
  return;
};

// Trying to put this as a function with the menu screens was a big problem because
// this class calls those functions and there was a circular definition problem.
void ExButton::fn_choice_screen() {

  bool done = false;
  while (!done) {

    String disp_str = " --EX Button Func.-- \n"
                      " Choose EX  Function:\n"
                      " 1) Open Pause Menu  \n"
                      " 2) Cycle Mel. Mute  \n"
                      " 3) Cycle Dn/Tr. Mute\n"
                      "                     \n"
                      "                     \n"
                      " X or 4) Go Back     \n";

    print_screen(disp_str);
    delay(200);

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      setFunc(1);

      done = true;
    } else if (my2Button->wasPressed()) {
      setFunc(2);

      done = true;
    } else if (my3Button->wasPressed()) {
      setFunc(3);

      done = true;
    } else if (my4Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};
