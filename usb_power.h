#ifndef USB_POWER_HH
#define USB_POWER_HH

#include "config.h"

#ifndef USE_TEENSY35
#include <imxrt.h>
#endif

void usb_power_on();
void usb_power_off();

#endif
