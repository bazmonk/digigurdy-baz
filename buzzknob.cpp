#include "buzzknob.h"

// BuzzKnob manages the potentiometer knob that adjusts the buzzing threshold.
// The GurdyCrank class below uses it to determine when buzzing happens.
BuzzKnob::BuzzKnob(int v_pin) {
      voltage_pin = v_pin;
      pinMode(voltage_pin, INPUT);
      adc->adc1->startContinuous(voltage_pin);
      poll_counter = 0;
      knob_voltage = 0;
};

// This should be run every loop() during play.
// Reads the knob voltage second.
void BuzzKnob::update() {
  if (the_knob_timer > 1000) {
    the_knob_timer = 0;
    knob_voltage = adc->adc1->analogReadContinuous();
  };
};

// Returns an a raw voltage between 0 and 1023.
float BuzzKnob::getVoltage() {
  return (float)(knob_voltage);
};

// This returns a weighted value based off the voltage between 60 and ~250,
//  except at the top end it ramps way up as a "buzz off"
float BuzzKnob::getThreshold() {
  if (getVoltage() > 975) {
    return (60 + (getVoltage() / 2));
  }
  return (60 + (getVoltage() / 6));
};
