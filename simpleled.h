#ifndef SIMPLELED_H
#define SIMPLELED_H

#include <Arduino.h>
#include <EEPROM.h>

#include "common.h"
#include "eeprom_values.h"

// class SimpleLED is for controlling LED lights wired up to a given pin on the Teensy (and ground)
class SimpleLED {
  private:
    int led_pin;
    int enabled;

  public:
    SimpleLED(int pin);

    void on();
    void off();
    void disable();
    void enable();
};

#endif
