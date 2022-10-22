#include "simpleled.h"
#include <Arduino.h> // Need this for access to digitalWrite, pinMode, etc. outside of main file.

SimpleLED::SimpleLED(int pin) {
  led_pin = pin;
  pinMode(led_pin, OUTPUT);
  // Not sure if this is needed, but let's make sure it's off:
  digitalWrite(led_pin, LOW);
};

void SimpleLED::on() {
  digitalWrite(led_pin, HIGH);
};

void SimpleLED::off() {
  digitalWrite(led_pin, LOW);
};
