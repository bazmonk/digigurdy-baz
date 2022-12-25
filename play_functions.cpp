#include "play_functions.h"

/// @ingroup play
/// @{

/// @brief Turns the volume of all strings up by 10, if possible.
/// @details 
/// * Checks volume of all strings
/// * If the loudest string is more than 10 from full volume, increases the volume of all strings by 10.
/// * If the loudest string is less than 10 from full, do nothing.
/// @version *New in 2.3.4*
void vol_up() {
  int highest_vol = 0;

  if (mystring->getVolume() > highest_vol) { highest_vol = mystring->getVolume(); };
  if (mylowstring->getVolume() > highest_vol) { highest_vol = mylowstring->getVolume(); };
  if (mytromp->getVolume() > highest_vol) { highest_vol = mytromp->getVolume(); };
  if (mydrone->getVolume() > highest_vol) { highest_vol = mydrone->getVolume(); };
  if (mybuzz->getVolume() > highest_vol) { highest_vol = mybuzz->getVolume(); };
  if (mykeyclick->getVolume() > highest_vol) { highest_vol = mykeyclick->getVolume(); };

  if (highest_vol < 117) {
    mystring->setVolume(mystring->getVolume() + 10);
    mylowstring->setVolume(mylowstring->getVolume() + 10);
    mytromp->setVolume(mytromp->getVolume() + 10);
    mydrone->setVolume(mydrone->getVolume() + 10);
    mybuzz->setVolume(mybuzz->getVolume() + 10);
    mykeyclick->setVolume(mykeyclick->getVolume() + 10);
  };

  if (mystring->isPlaying()) {
      mystring->soundOff();
      mystring->soundOn();
  };
  if (mylowstring->isPlaying()) {
    mylowstring->soundOff();
    mylowstring->soundOn();
  };
  if (mytromp->isPlaying()) {
    mytromp->soundOff();
    mytromp->soundOn();
  };
  if (mydrone->isPlaying()) {
    mydrone->soundOff();
    mydrone->soundOn();
  };
};

/// @brief Turns the volume of all strings down by 10, if possible.
/// @details 
/// * Checks volume of all strings
/// * If the quietest string is less than 10 from min volume, decreases the volume of all strings by 10.
/// * If the quietest string is more than 10 from min volume, do nothing.
/// @version *New in 2.3.4*
void vol_down() {

  int lowest_vol = 127;

  if (mystring->getVolume() < lowest_vol) { lowest_vol = mystring->getVolume(); };
  if (mylowstring->getVolume() < lowest_vol) { lowest_vol = mylowstring->getVolume(); };
  if (mytromp->getVolume() < lowest_vol) { lowest_vol = mytromp->getVolume(); };
  if (mydrone->getVolume() < lowest_vol) { lowest_vol = mydrone->getVolume(); };
  if (mybuzz->getVolume() < lowest_vol) { lowest_vol = mybuzz->getVolume(); };
  if (mykeyclick->getVolume() < lowest_vol) { lowest_vol = mykeyclick->getVolume(); };

  if (lowest_vol > 10) {
    mystring->setVolume(mystring->getVolume() - 10);
    mylowstring->setVolume(mylowstring->getVolume() - 10);
    mytromp->setVolume(mytromp->getVolume() - 10);
    mydrone->setVolume(mydrone->getVolume() - 10);
    mybuzz->setVolume(mybuzz->getVolume() - 10);
    mykeyclick->setVolume(mykeyclick->getVolume() - 10);
  };

  if (mystring->isPlaying()) {
      mystring->soundOff();
      mystring->soundOn();
  };
  if (mylowstring->isPlaying()) {
    mylowstring->soundOff();
    mylowstring->soundOn();
  };
  if (mytromp->isPlaying()) {
    mytromp->soundOff();
    mytromp->soundOn();
  };
  if (mydrone->isPlaying()) {
    mydrone->soundOff();
    mydrone->soundOn();
  };
};

/// @}