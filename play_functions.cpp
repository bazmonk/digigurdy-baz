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

/// @brief Sends a soundOff() to all six string objects.
void all_soundOff() {
  mystring->soundOff();
  mylowstring->soundOff();
  mykeyclick->soundOff();
  mytromp->soundOff();
  mydrone->soundOff();
  mybuzz->soundOff();
};

/// @brief Sends a soundOff() to all strings but the buzz
void no_buzz_soundOff() {
  mystring->soundOff();
  mylowstring->soundOff();
  mykeyclick->soundOff();
  mytromp->soundOff();
  mydrone->soundOff();
};

/// @brief Sends a soundKill() to all six string objects.
void all_soundKill() {
  mystring->soundKill();
  mylowstring->soundKill();
  mykeyclick->soundKill();
  mytromp->soundKill();
  mydrone->soundKill();
  mybuzz->soundKill();
};

/// @brief Clears the volume array of all six string objects.
void all_clearVolArray() {
  mystring->clearVolArray();
  mylowstring->clearVolArray();
  mykeyclick->clearVolArray();
  mytromp->clearVolArray();
  mydrone->clearVolArray();
  mybuzz->clearVolArray();
};

/// @brief Increases the transpose by 1 semitone, and adjusts any playing notes.
/// @param playing True if currently playing sound, false otherwise.
/// @version *New in  2.3.7*
void tpose_up_1(bool playing) {
  if (tpose_offset < max_tpose) {
      tpose_offset += 1;

    if (playing) {
      no_buzz_soundOff();

      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                  tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };
};

/// @brief Decreases the transpose by 1 semitone, and adjusts any playing notes.
/// @param playing True if currently playing sound, false otherwise.
/// @version *New in  2.3.7*
void tpose_down_1(bool playing) {
  if (max_tpose + tpose_offset > 0) {
      tpose_offset -= 1;

    if (playing) {
      no_buzz_soundOff();

      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                  tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };
};

/// @brief Cycles the capo, and adjusts any playing notes.
/// @param playing True if currently playing sound, false otherwise.
/// @version *New in  2.3.7*
void cycle_capo(bool playing) {
  capo_offset += 2;

    if (capo_offset > max_capo ) {
      capo_offset = 0;
    };

    if (playing) {
      no_buzz_soundOff();

      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                 tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
};

/// @brief Adjusts the transpose by a given number of steps
/// @param playing True if currently playing sound, false otherwise.
/// @param steps Number of semitones to move (signed)
/// @version *New in  2.5.7*
void tpose_up_x(bool playing, int steps) {
  if (max_tpose + steps >= 0) {
      tpose_offset = steps;

    if (playing) {
      no_buzz_soundOff();

      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                  tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };
};

/// @}
