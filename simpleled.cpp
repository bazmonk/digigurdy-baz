#include "simpleled.h"
#include <Arduino.h> // Need this for access to digitalWrite, pinMode, etc. outside of main file.

/// @brief Cosntructor.  SimpleLED is meant for controlling a LED connected to a given pin.
/// @param pin The digital pin to use
SimpleLED::SimpleLED(int pin) {
  led_pin = pin;
  enabled = EEPROM.read(EEPROM_BUZZ_LED);
  pinMode(led_pin, OUTPUT);
  // Not sure if this is needed, but let's make sure it's off:
  digitalWrite(led_pin, LOW);
};

/// @brief Turns the LED on.
void SimpleLED::on() {
  if (enabled == 1 && (mytromp->getMute() == false)) {
    digitalWrite(led_pin, HIGH);
  };
};

/// @brief Turns the LED off.
void SimpleLED::off() {
  if (enabled == 1) {
    digitalWrite(led_pin, LOW);
  };
};

/// @brief Disabled the LED.  If disabled, on() and off() will not respond.
void SimpleLED::disable() {
  enabled = 0;
};

/// @brief Enables the LED.
void SimpleLED::enable() {
  enabled = 1;
};
