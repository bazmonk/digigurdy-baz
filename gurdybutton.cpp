#include "gurdybutton.h"

/// @brief Constructor.  This class handles simple push-on, release-off buttons.
/// @param my_pin The digital pin this button is connected to
/// @param interval The debounce interval for this button
/// @details This class assumes the button is wired up as an active-low button.  It applies the internal pullup resistor.
GurdyButton::GurdyButton(int my_pin, int interval) {

  bounce_obj = new Bounce(my_pin, interval);

  // In John's code the non-keybox buttons weren't using Bounce.
  // I'm taking a guess that INPUT_PULLUP is what I want to use here but I think so.
  // Originally it was using DirectRead/Write and INPUT.
  pinMode(my_pin, INPUT_PULLUP);

  being_pressed = false;
};

/// @brief Polls the button and updates its state.
/// @details This should be run every loop() even if the results are not used.
void GurdyButton::update() {

  bounce_obj->update();

  // If button was pressed or released this cycle, record that.

  if (bounce_obj->fallingEdge()) {
    being_pressed = true;
  } else if (bounce_obj->risingEdge()) {
    being_pressed = false;
  };
};

/// @brief Reports if the button is being pressed this update() cycle.
/// @return True if button is actve, false otherwise.
bool GurdyButton::beingPressed() {
  return being_pressed;
};

/// @brief Reports if the button was pressed down this update() cycle.
/// @return True if button was just pressed down, false otherwise.
bool GurdyButton::wasPressed() {
  return bounce_obj->fallingEdge();
};

/// @brief Reports if the button was released this update() cycle.
/// @return True if button was just released, false otherwise.
bool GurdyButton::wasReleased() {
  return bounce_obj->risingEdge();
};
