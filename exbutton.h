#ifndef EXBUTTON_H
#define EXBUTTON_H

#include <Arduino.h>
#include <EEPROM.h>

#include "gurdybutton.h"
#include "exfunctions.h"
#include "display.h"
#include "eeprom_values.h"
//#include "common.h"

class ExButton: public GurdyButton {
  private:
    int my_func;

  public:
    ExButton(int my_pin, int func, int interval);

    int getFunc();

    void setFunc(int func);

    void doFunc(bool playing);

    String printFunc();

    void fn_choice_screen(int but_num);

    bool fn_choice_screen_2(int but_num);
};

#endif
