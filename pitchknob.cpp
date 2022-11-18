#include "pitchknob.h"

PitchKnob::PitchKnob(int v_pin) {
  voltage_pin = v_pin;
  pinMode(voltage_pin, INPUT);
  adc->adc0->startContinuous(voltage_pin);

  pitch_knob_timer = 0;
  voltage = 0;
};

void PitchKnob::update() {
  if (pitch_knob_timer > 50) {
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
    return 16384;
  }
  else {
  //John: for 2.2v, change the multiplier from 8 to 11 or 12
    return 16384 + (8 * voltage);
  };
};
