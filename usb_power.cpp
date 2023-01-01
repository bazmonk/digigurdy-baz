#include "usb_power.h"

/// @brief Enables power to the USBHost controller pin.
void usb_power_on() {
  #ifndef USE_TEENSY35
  GPIO8_GDIR |= 1<<26;
  GPIO8_DR_SET = 1<<26;
  #endif
}

/// @brief Cuts power to the USBHost controller pin.
void usb_power_off() {
  #ifndef USE_TEENSY35
  GPIO8_DR_CLEAR = 1<<26;
  #endif
}
