#ifndef KEYBOXBUTTON_H
#define KEYBOXBUTTON_H

#include <Arduino.h>

#include "gurdybutton.h"

class KeyboxButton: public GurdyButton {
  private:
    int note_offset;

  public:
    KeyboxButton(int my_pin, int my_offset);

    int getOffset();
};

#endif
