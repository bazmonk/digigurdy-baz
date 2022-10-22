#include <Bounce.h>
#include <Arduino.h>

#include "gurdybutton.h"
#include "togglebutton.h"

// class ToggleButton adds a state toggle to the GurdyButton class
//   This class is meant for buttons where:
//   * Pressing and releasing once activates it.
//   * Pressing and releasing again deactivates it.

ToggleButton::ToggleButton(int my_pin) : GurdyButton(my_pin) {
  bounce_obj = new Bounce(my_pin, 10);
  pinMode(my_pin, INPUT_PULLUP);
  toggled = false;
};

// update() works a little differently and also checks the toggle status.
void ToggleButton::update() {
  bounce_obj->update();

  // We'll only look at the downpress to register the toggle.
  if(bounce_obj->fallingEdge()) {
    toggled = !toggled;
  };
};

bool ToggleButton::toggleOn() {
  return toggled;
};

// This is to forcibly turn the toggle off after a menu press.
void ToggleButton::setToggle(bool new_toggle) {
  toggled = new_toggle;
};
