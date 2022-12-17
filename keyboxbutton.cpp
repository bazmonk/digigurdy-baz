#include "keyboxbutton.h"

// class KeyboxButton adds a note offset variable to the GurdyButton class.
//   This class is meant for use with the keybox keys, where:
//   * Button type is press-on, release-off
//   * Button has an offset that it raises notes to on a string.

KeyboxButton::KeyboxButton(int my_pin, int my_offset) : GurdyButton(my_pin, 5) {
  note_offset = my_offset;
};

int KeyboxButton::getOffset() {
  return note_offset;
};
