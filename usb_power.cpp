#include "usb_power.h"

/// @brief Enables power to the USBHost controller pin.
void usb_power_on() {
  #ifdef ARDUINO_TEENSY41
  GPIO8_GDIR |= 1<<26;
  GPIO8_DR_SET = 1<<26;
  #endif

  #ifdef ARDUINO_TEENSY36
  PORTE_PCR6 = PORT_PCR_MUX(1);
  GPIOE_PDDR |= (1<<6);
  GPIOE_PSOR = (1<<6); // turn on USB host power
  #endif
}

/// @brief Cuts power to the USBHost controller pin.
void usb_power_off() {
  #ifdef ARDUINO_TEENSY41
  GPIO8_DR_CLEAR = 1<<26;
  #endif

  #ifdef ARDUINO_TEENSY36
  GPIOE_PCOR = (1<<6); // turn on USB host power
  #endif
}
