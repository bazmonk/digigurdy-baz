#include "simpleled.h"
#include <Arduino.h> // Need this for access to digitalWrite, pinMode, etc. outside of main file.

SimpleLED::SimpleLED(int pin) {
  led_pin = pin;
  enabled = EEPROM.read(EEPROM_BUZZ_LED);
  pinMode(led_pin, OUTPUT);
  // Not sure if this is needed, but let's make sure it's off:
  digitalWrite(led_pin, LOW);
};

void SimpleLED::on() {
  if (enabled == 1 && (mytromp->getMute() == false)) {
    digitalWrite(led_pin, HIGH);
  };
};

void SimpleLED::off() {
  if (enabled == 1) {
    digitalWrite(led_pin, LOW);
  };
};

void SimpleLED::disable() {
  enabled = 0;
};

void SimpleLED::enable() {
  enabled = 1;
};
