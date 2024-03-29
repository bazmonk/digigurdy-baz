#ifndef PITCHKNOB_H
#define PITCHKNOB_H

#include <ADC.h>

#include "common.h"

extern ADC* adc;

class PitchKnob{
  private:
    int voltage_pin;
    int max_voltage = 1023;
    int voltage;

    elapsedMillis pitch_knob_timer;

  public:
    PitchKnob(int v_pin);

    void update();
    int getVoltage();
    int getPitch();
};
#endif
