#ifndef VIBKNOB_H
#define VIBKNOB_H

#include <ADC.h>

#include "common.h"
#include "config.h"

extern ADC* adc;

class VibKnob{
  private:
    int voltage_pin;
    float max_voltage = 1022.0;
    int voltage;

    elapsedMillis vib_knob_timer;

  public:
    VibKnob(int v_pin);

    void update();
    int getVoltage();
    int getVibrato();
};
#endif
