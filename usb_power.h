#ifndef USB_POWER_HH
#define USB_POWER_HH

#include "config.h"

#ifdef ARDUINO_TEENSY41
#include <imxrt.h>
#endif

#ifdef ARDUINO_TEENSY36
#include <kinetis.h>
#endif

void usb_power_on();
void usb_power_off();

#endif
