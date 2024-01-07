#ifndef GURDYCRANK_H
#define GURDYCRANK_H

#include <ADC.h>
#include <Arduino.h>

#include "buzzknob.h"
#include "config.h"
#include "simpleled.h"

#ifdef USE_ENCODER
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#endif

extern volatile int num_events;
extern volatile int last_event;
extern elapsedMicros last_event_timer;
extern elapsedMicros debounce_timer;

class GurdyCrank {
  private:
    int sensor_pin;
    double spoke_width = 1.0 / (NUM_SPOKES * 2.0);
    double cur_vel;
    bool was_spinning = false;
    bool was_buzzing = false;

    #ifdef USE_ENCODER
    long pulse;
    long last_pulse;
    double new_vel;
    bool clockwise;
    int angle;
    Encoder *myEnc;
    #endif

    int expression;
    int buzz_expression;

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
    GurdyCrank(int s_pin, int s_pin2, int buzz_pin, int led_pin);

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
    
    #ifdef USE_ENCODER
    int getAngle();
    void resetAngle();
    #endif
};

#endif
