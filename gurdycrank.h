#ifndef GURDYCRANK_H
#define GURDYCRANK_H

#include <ADC.h>
#include <Arduino.h>

#include "buzzknob.h"
#include "config.h"
#include "simpleled.h"

extern volatile int num_events;
extern volatile int last_event;
extern elapsedMicros last_event_timer;
extern elapsedMicros debounce_timer;

class GurdyCrank {
  private:
    int sensor_pin;
    double spoke_width = 1.0 / (NUM_SPOKES * 2.0);
    double cur_vel;
    double smoothing_factor = 0.2;
    bool was_spinning = false;
    bool was_buzzing = false;

    int expression;

    elapsedMicros eval_timer;
    elapsedMicros decay_timer;
    elapsedMillis the_expression_timer;
    elapsedMillis the_buzz_timer;

    BuzzKnob* myKnob;

    #ifdef LED_KNOB
      SimpleLED* myLED;
    #endif

  public:
    GurdyCrank(int s_pin, int buzz_pin, int led_pin);

    bool isDetected();
    void update();
    void updateExpression();
    bool startedSpinning();
    bool stoppedSpinning();
    bool isSpinning();
    bool startedBuzzing();
    bool stoppedBuzzing();
    double getVAvg();
    void disableLED();
    void enableLED();
};

#endif
