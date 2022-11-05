#ifndef HURDYGURDY_H
#define HURDYGURDY_H

#include "config.h"
#include "keyboxbutton.h"

class HurdyGurdy {
  private:
    int keybox_size;         // How many keys are in the keybox
    int max_offset;          // The currest highest key being pressed
    int prev_offset;         // The highest key from last loop() cycle
    bool higher_key_pressed;
    bool lower_key_pressed;

  public:
    KeyboxButton* keybox[num_keys];
    HurdyGurdy(const int pin_arr[], int key_size);

    int getMaxOffset();
    bool higherKeyPressed();
    bool lowerKeyPressed();
};

#endif
