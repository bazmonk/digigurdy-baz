#include "pitchknob.h"

PitchKnob::PitchKnob(int v_pin) {
  voltage_pin = v_pin;
  pinMode(voltage_pin, INPUT);
  adc->adc0->startContinuous(voltage_pin);

  pitch_knob_timer = 0;
  voltage = 0;
  enabled = false;
};

void PitchKnob::update() {
  if (enabled && pitch_knob_timer > 100) {
    voltage = adc->adc0->analogReadContinuous();

    mystring->setPitchBend(getPitch());
    mylowstring->setPitchBend(getPitch());

    pitch_knob_timer = 0;
  };
};

int PitchKnob::getVoltage() {
  return voltage;
};

int PitchKnob::getPitch() {
  // By my calculations, each number in this voltage value is about 3mV
  // This makes sure it really zeroes out.
  if (voltage < 5) {
    return 0;
  }
  else {
    if (getVoltage() > PEDAL_MAX_V) {
      return 8191;
    } else {
    return int(8191.0 * getVoltage() / PEDAL_MAX_V);
    };
  };
};

void PitchKnob::enable() {
  enabled = true;
};

void PitchKnob::disable() {
  enabled = false;
};
