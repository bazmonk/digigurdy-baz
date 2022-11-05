#ifndef BUZZKNOB_H
#define BUZZKNOB_H

#include <ADC.h>

extern ADC* adc;

class BuzzKnob {
  private:
    int voltage_pin;
    static const int poll_interval = 5000;
    int poll_counter;
    int knob_voltage;
    int last_poll_time;

    elapsedMillis the_knob_timer;

  public:
    BuzzKnob(int v_pin);

    void update();
    float getVoltage();
    float getThreshold();
};
#endif
