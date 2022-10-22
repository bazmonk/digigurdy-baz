#ifndef BUZZKNOB_H
#define BUZZKNOB_H

#include <ADC.h>

class BuzzKnob {
  private:
    int voltage_pin;
    static const int poll_interval = 5000;
    int poll_counter;
    int knob_voltage;
    ADC* myadc;
    int last_poll_time;

    elapsedMillis the_knob_timer;

  public:
    BuzzKnob(int v_pin, ADC* adc_obj);

    void update();
    float getVoltage();
    float getThreshold();
};
#endif
