#include "gurdystring.h"

// GurdyString manages turning "strings" on and off and determining their note.
// It abstracts the interactions with the MIDI layer.
GurdyString::GurdyString(int my_channel, int my_note, int my_vol) {
  midi_channel = my_channel;
  open_note = my_note;
  midi_volume = my_vol;
  note_being_played = open_note;
};

// soundOn() sends sound on this string's channel at its notes
// optionally with an additional offset (e.g. a key being pressed)
//
// I DO NOT KNOW why I can just access usbMIDI here, but even when
// creating MIDI_obj globally, I could not access MIDI_obj the same way.
// Bringing in a pointer and working with MIDI_obj in this manner is
// because of that.
void GurdyString::soundOn(int my_offset, int my_modulation) {
  note_being_played = open_note + my_offset;
  if (!mute_on) {
    usbMIDI.sendNoteOn(note_being_played, midi_volume, midi_channel);
    MIDI.sendNoteOn(note_being_played, midi_volume, midi_channel);

    // If modulation isn't zero, send that as a MIDI CC for this channel
    // This is meant to be configured to create a gentle vibrato.
    if (my_modulation > 0) {
      usbMIDI.sendControlChange(1, my_modulation, midi_channel);
      MIDI.sendControlChange(1, my_modulation, midi_channel);
    }
  }
  is_playing = true;
};

// soundOff gracefully turns off the playing note on the string.
void GurdyString::soundOff() {
  usbMIDI.sendNoteOff(note_being_played, midi_volume, midi_channel);
  MIDI.sendNoteOff(note_being_played, midi_volume, midi_channel);
  is_playing = false;
};

// soundKill is a nuclear version of soundOff() that kills sound on the channel.
// It does not need to know the note being played as it kills all of them.
void GurdyString::soundKill() {
  usbMIDI.sendControlChange(123, 0, midi_channel);
  MIDI.sendControlChange(123, 0, midi_channel);
  is_playing = false;
};

int GurdyString::getOpenNote() {
  return open_note;
};

void GurdyString::setOpenNote(int new_note) {
  open_note = new_note;
};

// MIDI volume is an integer between 0 (off) and 127 (full volume).
void GurdyString::setVolume(int vol) {
  midi_volume = vol;
};

int GurdyString::getVolume() {
  return midi_volume;
};

void GurdyString::setMute(bool mute) {
  mute_on = mute;
};

bool GurdyString::getMute() {
  return mute_on;
};

bool GurdyString::isPlaying() {
  return is_playing;
};

void GurdyString::setProgram(uint8_t program) {
  usbMIDI.sendProgramChange(program, midi_channel);
  MIDI.sendProgramChange(program, midi_channel);
};

void GurdyString::setExpression(int exp) {
      usbMIDI.sendControlChange(11, exp, midi_channel);
      MIDI.sendControlChange(11, exp, midi_channel);
};
