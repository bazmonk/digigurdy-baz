#ifndef GEARCRANK_H
#define GEARCRANK_H

#include <ADC.h>

#include "buzzknob.h"
#include "config.h"

extern ADC* adc;

class GearCrank {
  private:
    int voltage_pin;
    static const int num_samples = 500;  // This number is from the original code
    int samples[num_samples];
    int sample_sum;
    float sample_mean;
    float squared_sum;
    float deviations;

    int crank_voltage;

    long int sample_total;
    int spin;
    bool started_spinning;
    bool stopped_spinning;
    bool is_spinning;

    BuzzKnob* myKnob;
    bool started_buzzing;
    bool stopped_buzzing;
    bool is_buzzing;
    int buzz_countdown;

  public:
    GearCrank(int v_pin, int buzz_pin);
    void beginPolling();
    void detect();
    bool isDetected();
    void refreshBuzz();
    void update();
    bool startedSpinning();
    bool stoppedSpinning();
    bool isSpinning();
    bool startedBuzzing();
    bool stoppedBuzzing();
};

#endif
