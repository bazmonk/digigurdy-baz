#include "gurdybutton.h"

// GurdyButton objects abstract the physical buttons.
//   This class is for generic buttons like the octave/tpose ones, which are
//   press-on, release-off but otherwise generic in purpose.

GurdyButton::GurdyButton(int my_pin, int interval) {
  // The 5 is 5ms wait for events to complete.
  // Recommended from the Bounce webpage for "good" buttons.
  bounce_obj = new Bounce(my_pin, interval);

  // In John's code the non-keybox buttons weren't using Bounce.
  // I'm taking a guess that INPUT_PULLUP is what I want to use here but I think so.
  // Originally it was using DirectRead/Write and INPUT.
  pinMode(my_pin, INPUT_PULLUP);

  being_pressed = false;
};

// Bounce objects should only be update()-ed once per loop(),
// so I'm not putting it in wasPressed()/wasReleased().
void GurdyButton::update() {

  bounce_obj->update();

  // If button was pressed or released this cycle, record that.
  if (bounce_obj->fallingEdge()) {
    being_pressed = true;
  } else if (bounce_obj->risingEdge()) {
    being_pressed = false;
  };
};

// This returns true if the button is being pressed.
bool GurdyButton::beingPressed() {
  return being_pressed;
};

// This returns true ONLY if the button was pressed this particular loop cycle.
bool GurdyButton::wasPressed() {
  return bounce_obj->fallingEdge();
};

// This returns true ONLY if the button was released this particular loop cycle.
bool GurdyButton::wasReleased() {
  return bounce_obj->risingEdge();
};
