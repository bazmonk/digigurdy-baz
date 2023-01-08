#include "buzzknob.h"

/// @brief Contructor.  BuzzKnob handles the potentiometer knob that adjusts buzzing thresholds.
/// @param v_pin The active pin the potentiometer is attached to.
/// @warning 3.3V MAX!
BuzzKnob::BuzzKnob(int v_pin) {
      voltage_pin = v_pin;
      pinMode(voltage_pin, INPUT);
      adc->adc1->startContinuous(voltage_pin);
      poll_counter = 0;
      knob_voltage = 0;
};

/// @brief Checks knob for new reading.  Self-timing: should be run every loop() cycle.
void BuzzKnob::update() {
  if (the_knob_timer > 1000) {
    the_knob_timer = 0;
    knob_voltage = adc->adc1->analogReadContinuous();
  };
};

/// @brief Gets the current voltage of the knob.
/// @return The voltage, 0 = 0V, 1023 = 3.3V
float BuzzKnob::getVoltage() {
  return (float)(knob_voltage);
};

/// @brief Gets the calculated velocity threshold based off the current voltage.
/// @return The buzzing velocity threshold.
float BuzzKnob::getThreshold() {
  if (getVoltage() > 975) {
    return (60 + (getVoltage() / 2));
  }
  return (60 + (getVoltage() / 6));
};
