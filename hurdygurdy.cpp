#include "hurdygurdy.h"

/// @brief Constructor.  HurdyGurdy abstracts the keybox and keybox buttons.
/// @param pin_arr Array of keybox pins, ordered from lowest to highest starting with index 1
/// @param key_size Number of keybox pins, the len of pin_arr[] - 1
HurdyGurdy::HurdyGurdy(const int pin_arr[], int key_size) {
  keybox_size = key_size;
  max_offset = 0;

  // Run through the array from the top of this file and create all the keyboxbutton
  // objects
  for(int x = 1; x < key_size + 1; x++) {
    keybox[x-1] = new KeyboxButton(pin_arr[x], x);
  };
};

/// @brief Updates all keybox objects, returns the highest key being pressed on the keybox.
/// @return the index of the highest key being pressed
/// @note This is meant to be run every loop() cycle.
int HurdyGurdy::getMaxOffset() {

  higher_key_pressed = false;
  lower_key_pressed = false;

  // Save the last highest key
  prev_offset = max_offset;
  max_offset = 0;

  // Look at each key in order
  for(int x = 0; x < keybox_size; x++) {
    // Update the key
    keybox[x]->update();

    // if the key is being pressed, record that key's offset.  Array is in order,
    // so at the end we should have the max_offset.
    if (keybox[x]->beingPressed()) {
      max_offset = keybox[x]->getOffset();
    };
  };

  if (max_offset > prev_offset) {
    higher_key_pressed = true;
  };

  if (max_offset < prev_offset) {
    lower_key_pressed = true;
  };

  return max_offset;
};

/// @brief Returns whether or not a higher key is being pressed this cycle.
/// @return True if the current max_offset is greater than the max_offset last cycle, false otherwise.
/// @note This method is for determing if a keyclick sound needs to be made.
bool HurdyGurdy::higherKeyPressed() {
  return higher_key_pressed;
};

/// @brief Retruns whether or not a lower key is being pressed this cycle.
/// @return True if the current max_offset is strictly less than the offset last cycle. False otherwise.
bool HurdyGurdy::lowerKeyPressed() {
  return lower_key_pressed;
};
