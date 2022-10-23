#ifndef GURDYBUTTON_H
#define GURDYBUTTON_H

#include <ADC.h>
#include <Bounce.h>

class GurdyButton {
  protected:
    Bounce* bounce_obj;
    bool being_pressed;
  public:
    GurdyButton(int my_pin);

    void update();
    bool beingPressed();
    bool wasPressed();
    bool wasReleased();
};

#endif
