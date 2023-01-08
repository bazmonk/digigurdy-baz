#include "usb_power.h"

/// @brief Enables power to the USBHost controller pin.
void usb_power_on() {
  #if !defined(USE_TEENSY35) && !defined(USE_TEENSY_36)
  GPIO8_GDIR |= 1<<26;
  GPIO8_DR_SET = 1<<26;
  #endif

  #ifdef USE_TEENSY_36
  PORTE_PCR6 = PORT_PCR_MUX(1);
  GPIOE_PDDR |= (1<<6);
  GPIOE_PSOR = (1<<6); // turn on USB host power
  #endif
}

/// @brief Cuts power to the USBHost controller pin.
void usb_power_off() {
  #if !defined(USE_TEENSY35) && !defined(USE_TEENSY_36)
  GPIO8_DR_CLEAR = 1<<26;
  #endif

  #ifdef USE_TEENSY_36
  GPIOE_PCOR = (1<<6); // turn on USB host power
  #endif
}
