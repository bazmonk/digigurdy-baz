#include "togglebutton.h"

/// @brief Constructor.  This extends GurdyButton with a toggle on/off feature.
/// @param my_pin The digitial voltage pin to be used.
/// @param interval The debounce interval to be used.
ToggleButton::ToggleButton(int my_pin, int interval) : GurdyButton(my_pin, interval) {
  toggled = false;
};

/// @brief Samples the button and determines toggle status.
void ToggleButton::update() {
  bounce_obj->update();

  // We'll only look at the downpress to register the toggle.
  if(bounce_obj->fallingEdge()) {
    toggled = !toggled;
  };
};

/// @brief Returns the toggle status of the button.
/// @return True if toggled on, false otherwise
bool ToggleButton::toggleOn() {
  return toggled;
};

/// @brief Forcibly sets the toggle status of the button.
/// @param new_toggle True = toggled on, false = toggled off
void ToggleButton::setToggle(bool new_toggle) {
  toggled = new_toggle;
};
