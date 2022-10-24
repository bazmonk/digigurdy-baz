#ifndef GURDYSTRING_H
#define GURDYSTRING_H

// https://www.pjrc.com/teensy/td_midi.html
// https://www.pjrc.com/teensy/td_libs_MIDI.html
#include <MIDI.h>

extern MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>> MIDI;

class GurdyString {
  private:
    int open_note;          // This string's base note
    int midi_channel;       // This string's MIDI channel (1-8)
    int midi_volume;        // 0-127, I'm using 56 everywhere right now
    bool mute_on = false;   // Controls the mute feature
    bool is_playing = false;
    int note_being_played;  // The note being sounded (base note + key offset)
                            // This is necessary to turn off notes before turning on new ones.

  public:
    GurdyString(int my_channel, int my_note, int my_vol = 70);
    void soundOn(int my_offset = 0, int my_modulation = 0);
    void soundOff();
    void soundKill();
    int getOpenNote();
    void setOpenNote(int new_note);
    void setVolume(int vol);
    int getVolume();
    void setMute(bool mute);
    bool getMute();
    bool isPlaying();
    void setProgram(uint8_t program);
};

#endif
