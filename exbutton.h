#ifndef EXBUTTON_H
#define EXBUTTON_H

#include <Arduino.h>
#include <EEPROM.h>

#include "togglebutton.h"
#include "exfunctions.h"
#include "display.h"
#include "eeprom_values.h"
//#include "common.h"

class ExButton: public ToggleButton {
  private:
    int my_func;
    int eeprom_addr;

  public:
    ExButton(int my_pin, int func, int interval, int my_addr);

    int getFunc();

    void setFunc(int func);

    void doFunc(bool playing);

    String printFunc();

    void fn_choice_screen();

    bool fn_choice_screen_2();
};

#endif
