#include "vibknob.h"

VibKnob::VibKnob(int v_pin) {
  voltage_pin = v_pin;
  pinMode(voltage_pin, INPUT);
  adc->adc0->startContinuous(voltage_pin);

  vib_knob_timer = 0;
  voltage = 0;
};

void VibKnob::update() {
  if (vib_knob_timer > 100) {
    voltage = adc->adc0->analogReadContinuous();

    mystring->setVibrato(getVibrato());
    mylowstring->setVibrato(getVibrato());

    vib_knob_timer = 0;
  };
};

int VibKnob::getVoltage() {
  return voltage;
};

int VibKnob::getVibrato() {
  float adj_value = (getVoltage() / max_voltage) * 127;

  if (adj_value < MELODY_VIBRATO) {
    return MELODY_VIBRATO;
  } else if (adj_value > 127) {
    return 127;
  } else {
    return (int)(adj_value);
  };
};
