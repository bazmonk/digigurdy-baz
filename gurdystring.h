#ifndef GURDYSTRING_H
#define GURDYSTRING_H

#include "config.h"
#include "notes.h"

// https://www.pjrc.com/teensy/td_midi.html
// https://www.pjrc.com/teensy/td_libs_MIDI.html
#include <MIDI.h>

#ifdef USE_TRIGGER
  #include "wavTrigger.h"
  extern wavTrigger trigger_obj;
#endif

#ifdef USE_TSUNAMI
  #include "Tsunami.h"
  extern Tsunami trigger_obj;
#endif

extern MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>> MIDI;


class GurdyString {
  private:
    String name;
    int open_note;          // This string's base note
    int midi_channel;       // This string's MIDI channel (1-8)
    int midi_volume;        // 0-127, I'm using 56 everywhere right now
    int trigger_volume;     // For Tsunami/Trigger
    bool mute_on = false;   // Controls the mute feature
    bool is_playing = false;
    int note_being_played;  // The note being sounded (base note + key offset)
                            // This is necessary to turn off notes before turning on new ones.
    int output_mode;
    int gros_mode;
    int vol_array[128];

  public:
    GurdyString(int my_channel, int my_note, String my_name, int my_mode, int my_vol = 70);
    void soundOn(int my_offset = 0, int my_modulation = 0);
    void soundOn(int my_offset, int my_modulation, int note);
    void soundOff();
    void soundOff(int note);
    void soundKill();
    int getOpenNote();
    void setOpenNote(int new_note);
    void setVolume(int vol);
    int getVolume();
    void setMute(bool mute);
    bool getMute();
    bool isPlaying();
    void setProgram(uint8_t program);
    void setExpression(int exp);
    void setPitchBend(int bend);
    void setVibrato(int vib);
    String getName();
    void setOutputMode(int my_mode);
    void setGrosMode(int my_gros_mode);
    int getGrosMode();
    String getGrosString();
    void setTrackLoops();
    void clearVolArray();
};

#endif
