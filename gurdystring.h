#ifndef GURDYSTRING_H
#define GURDYSTRING_H

#include <MIDI.h>

using namespace MIDI_NAMESPACE;

class GurdyString {
  private:
    int open_note;          // This string's base note
    int midi_channel;       // This string's MIDI channel (1-8)
    int midi_volume;        // 0-127, I'm using 56 everywhere right now
    bool mute_on = false;   // Controls the mute feature
    int note_being_played;  // The note being sounded (base note + key offset)
                            // This is necessary to turn off notes before turning on new ones.

    // This is a pointer to the MIDI object that powers the phsyical MIDI ports on the gurdy.
    // usbMIDI shows up with Arduino library magic somehow and we don't need this for it.
    MidiInterface<SerialMIDI<HardwareSerial>> *MIDI_obj;

  public:
    GurdyString(int my_channel, int my_note, midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *my_MIDI_obj, int my_vol = 70);
    void soundOn(int my_offset = 0, int my_modulation = 0);
    void soundOff();
    void soundKill();
    int getOpenNote();
    void setOpenNote(int new_note);
    void setVolume(int vol);
    int getVolume();
    void setMute(bool mute);
    bool getMute();
    void setProgram(uint8_t program);
};

#endif
