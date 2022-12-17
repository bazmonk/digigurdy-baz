#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "gurdybutton.h"

class ToggleButton: public GurdyButton {
  private:
    bool toggled;

  public:
    ToggleButton(int my_pin, int interval);

    void update();

    bool toggleOn();

    void setToggle(bool new_toggle);
};

#endif
