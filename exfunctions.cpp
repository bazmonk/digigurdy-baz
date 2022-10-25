#include "exfunctions.h"

void cycle_mel_mute() {
  if (mel_mode == 0) {
    mel_mode = 1; // 1 == high on, low off
    mystring->setMute(false);
    mylowstring->setMute(true);
    if (mylowstring->isPlaying()) {
      mylowstring->soundOff();
      mylowstring->soundOn();
    };
  } else if (mel_mode == 1) {
    mel_mode = 2; // 2 == high off, low on
    mystring->setMute(true);
    mylowstring->setMute(false);
    if (mystring->isPlaying()) {
      mystring->soundOff();
      mystring->soundOn();
      mylowstring->soundOff();
      mylowstring->soundOn();
    };
  } else if (mel_mode == 2) {
    mel_mode = 0; // 0 == high on, low on
    mystring->setMute(false);
    mylowstring->setMute(false);
    if (mystring->isPlaying()) {
      mystring->soundOff();
      mystring->soundOn();
    };
  };
  if (mystring->isPlaying()) {
    draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
  } else {
    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
  };
};

void cycle_drone_tromp_mute() {
  if (drone_mode == 0) {
    drone_mode = 1; // 1 == both off
    mydrone->setMute(true);
    mytromp->setMute(true);
    mybuzz->setMute(true);
    if (mydrone->isPlaying()) {
      mydrone->soundOff();
      mydrone->soundOn();
      mytromp->soundOff();
      mytromp->soundOn();
    };
  } else if (drone_mode == 1) {
    drone_mode = 2; // 2 == drone on, tromp off
    mydrone->setMute(false);
    mytromp->setMute(true);
    mybuzz->setMute(true);
    if (mydrone->isPlaying()) {
      mydrone->soundOff();
      mydrone->soundOn();
    };
  } else if (drone_mode == 2) {
    drone_mode = 3; // 3 == drone off, tromp on
    mydrone->setMute(true);
    mytromp->setMute(false);
    mybuzz->setMute(false);
    if (mydrone->isPlaying()) {
      mydrone->soundOff();
      mydrone->soundOn();
      mytromp->soundOff();
      mytromp->soundOn();
    };
  } else if (drone_mode == 3) {
    drone_mode = 0; // 0 == both on
    mydrone->setMute(false);
    mytromp->setMute(false);
    mybuzz->setMute(false);
    if (mydrone->isPlaying()) {
      mydrone->soundOff();
      mydrone->soundOn();
    };
  };
  if (mystring->isPlaying()) {
    draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
  } else {
    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
  };
};

void cycle_drone_mute() {
  if (drone_mode == 0) {
    drone_mode = 1; // 1 == off
    mydrone->setMute(true);
    if (mydrone->isPlaying()) {
      mydrone->soundOff();
      mydrone->soundOn();
    };
  } else if (drone_mode == 1) {
    drone_mode = 0; // 0 = On
    mydrone->setMute(false);
    if (mydrone->isPlaying()) {
      mydrone->soundOff();
      mydrone->soundOn();
    };
  };
  if (mystring->isPlaying()) {
    draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
  } else {
    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
  };
};

void cycle_tromp_mute() {
  if (t_mode == 0) {
    t_mode = 1;
    mytromp->setMute(true);
    mybuzz->setMute(true);
    if (mytromp->isPlaying()) {
      mytromp->soundOff();
      mytromp->soundOn();
    };
  } else if (t_mode == 1) {
    t_mode = 0;
    mytromp->setMute(false);
    mybuzz->setMute(false);
    if (mytromp->isPlaying()) {
      mytromp->soundOff();
      mytromp->soundOn();
    };
  };
  if (mystring->isPlaying()) {
    draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
  } else {
    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
  };
};
