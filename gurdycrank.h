#ifndef GURDYCRANK_H
#define GURDYCRANK_H

#include "buzzknob.h"
#include "common.h"
#include "simpleled.h"

// class GurdyCrank controls the cranking mechanism, including the buzz triggers.
//   * This version is for optical (IR gate) sensors.  The digital pin readings are expected to
//     oscillate between 0 and 1 only.
//   * NUM_SPOKES in config.h needs to be defined as the number of "spokes" (dark lines) on your
//     wheel, not the number of dark+light bars.  Your RPMs will be half-speed if you do that.
class GurdyCrank {
  private:
    int sensor_pin;
    double spoke_width = 1.0 / (NUM_SPOKES * 2.0);
    double v_inst = 0.0;
    double v_last = 0.0;
    double v_smooth = 0.0;
    double v_2 = 0.0;
    double v_3 = 0.0;
    double v_4 = 0.0;
    double v_5 = 0.0;
    double v_6 = 0.0;
    double v_7 = 0.0;
    double v_8 = 0.0;
    double v_avg = 0.0;

    unsigned int this_time;
    unsigned int lt_1, lt_2, lt_3, lt_4, lt_5, lt_6, lt_7, lt_8 = 0;
    float lt_avg;
    float lt_stdev;

    bool last_event;
    bool this_event;
    bool was_spinning = false;
    bool was_buzzing = false;

    int expression;

    BuzzKnob* myKnob;

    #ifdef LED_KNOB
      SimpleLED* myLED;
    #endif

    int trans_count = 0;
    float rev_count = 0;

    elapsedMicros the_timer;
    elapsedMicros the_spoke_timer;
    elapsedMicros the_stop_timer;
    elapsedMillis the_buzz_timer;
    elapsedMillis the_expression_timer;

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
    int getCount();
    double getRev();
    void disableLED();
    void enableLED();
};

#endif
