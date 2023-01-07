#ifndef USB_POWER_HH
#define USB_POWER_HH

#include "config.h"

#if !defined(USE_TEENSY35) && !defined(USE_TEENSY36)
#include <imxrt.h>
#endif

#ifdef USE_TEENSY36
#include <kinetis.h>
#endif

void usb_power_on();
void usb_power_off();

#endif
