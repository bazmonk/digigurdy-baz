#include "exfunctions.h"

void trigger_pause_menu() {
  // Turn off the sound :-)
  mystring->soundOff();
  mylowstring->soundOff();
  mykeyclick->soundOff();  // Not sure if this is necessary... but it feels right.
  mytromp->soundOff();
  mydrone->soundOff();
  mybuzz->soundOff();

  // If I don't do this, it comes on afterwards.
  bigbutton->setToggle(false);

  pause_screen();
  print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
               tpose_offset, capo_offset, 0, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());

};
