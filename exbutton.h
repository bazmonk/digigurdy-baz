#ifndef EXBUTTON_H
#define EXBUTTON_H

#include <Arduino.h>
#include <EEPROM.h>

#include "togglebutton.h"
#include "exfunctions.h"
#include "display.h"
#include "eeprom_values.h"
#include "default_tunings.h"
//#include "common.h"

class ExButton: public ToggleButton {
  private:
    int my_func;
    int eeprom_addr;
    int eeprom_step_addr;
    int eeprom_slot_addr;
    int t_toggle_steps;
    int slot;

  public:
    ExButton(int my_pin, int interval, int my_addr, int my_step_addr, int my_slot_addr);

    int getFunc();

    void setFunc(int func);

    void doFunc(bool playing);

    String printFunc();

    bool fn_choice_screen();

    bool fn_choice_actions();
    bool fn_choice_mutes();
    bool fn_choice_tpose();
    bool fn_choice_audio();
    bool fn_choice_tuning();
};

#endif
