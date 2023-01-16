#include "vibknob.h"

/// @brief Cosntructor.  VibKnob abstracts a knob or pedal that produces a vibrato effect.
/// @param v_pin The *analog* voltage pin to be used.
VibKnob::VibKnob(int v_pin) {
  voltage_pin = v_pin;
  pinMode(voltage_pin, INPUT);
  adc->adc0->startContinuous(voltage_pin);

  vib_knob_timer = 0;
  voltage = 0;
  enabled = false;
};

/// @brief Sample the voltage pin and set the appropriate amount of vibrato on the melody strings.
/// @warning This class/method expects certain GurdyString objects to exist.
void VibKnob::update() {
  if (enabled && vib_knob_timer > 100) {
    voltage = adc->adc0->analogReadContinuous();

    mystring->setVibrato(getVibrato());
    mylowstring->setVibrato(getVibrato());

    vib_knob_timer = 0;
  };
};

/// @brief Reports the current pin voltage.
/// @return 0-1023, 0 = 0V, 1023 = 3.3V
int VibKnob::getVoltage() {
  return voltage;
};

/// @brief Report the current vibrato value, calculated based off the current voltage.
/// @return 0-127, the current vibrato/modulation value to be used.
int VibKnob::getVibrato() {
  float adj_value = (getVoltage() / max_voltage) * 127;

  if (adj_value < mel_vibrato) {
    return mel_vibrato;
  } else if (adj_value > 127) {
    return 127;
  } else {
    return (int)(adj_value);
  };
};

/// @brief Sets the enable flag for this object.
void VibKnob::enable() {
  enabled = true;
};

/// @brief Disabled this object.  update() will not respond if the object is disabled.
void VibKnob::disable() {
  enabled = false;
};
