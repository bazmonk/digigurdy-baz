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
  return;
};
