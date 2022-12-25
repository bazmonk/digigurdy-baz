#include "keyboxbutton.h"

// class KeyboxButton adds a note offset variable to the GurdyButton class.
//   This class is meant for use with the keybox keys, where:
//   * Button type is press-on, release-off
//   * Button has an offset that it raises notes to on a string.

/// @brief Constructor.  KeyboxButton extends the GurdyButton class for use as a keybox key.
/// @param my_pin The digital voltage pin for this key/switch/button
/// @param my_offset The number of semitones this key raises the note beign played
KeyboxButton::KeyboxButton(int my_pin, int my_offset) : GurdyButton(my_pin, 5) {
  note_offset = my_offset;
};

/// @brief Return the key's note offset.
/// @return The key's offset in semitones, 0-127 in theory (0-24 realistically)
int KeyboxButton::getOffset() {
  return note_offset;
};
