#include "hurdygurdy.h"

// class HurdyGurdy is basically a virtual keybox for buttons that control
// notes.  It manages updating and detecting the button actions and determines
// ultimately which note the "keybox" is producing.
HurdyGurdy::HurdyGurdy(const int pin_arr[], int key_size) {
  keybox_size = key_size;
  max_offset = 0;

  // Run through the array from the top of this file and create all the keyboxbutton
  // objects
  for(int x = 1; x < key_size + 1; x++) {
    keybox[x-1] = new KeyboxButton(pin_arr[x], x);
  };
};

// This method both updates all the keys, and returns the highest offset/notes
// being pressed this cycle.
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

bool HurdyGurdy::higherKeyPressed() {
  return higher_key_pressed;
};

bool HurdyGurdy::lowerKeyPressed() {
  return lower_key_pressed;
};
