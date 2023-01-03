#include "gurdystring.h"

/// @brief Constructor.  GurdyString manages turning "strings" on and off, determining its note, and interacting with the MIDI layer.
/// @param my_channel The MIDI channel to communicate over
/// @param my_note The base MIDI note of this string (0-127)
/// @param my_name A text label for this string (e.g. "Drone")
/// @param my_mode The secondary output mode (see setOutputMode() for more info)
/// @param my_vol The volume of this string (0-127)
GurdyString::GurdyString(int my_channel, int my_note, String my_name, int my_mode, int my_vol) {
  midi_channel = my_channel;
  name = my_name;
  open_note = my_note;
  midi_volume = my_vol;
  trigger_volume = int((my_vol)/128.0 * 80 - 70);
  note_being_played = open_note;
  gros_mode = 0;

  setOutputMode(my_mode);
};

// soundOn() sends sound on this string's channel at its notes
// optionally with an additional offset (e.g. a key being pressed)
//
// I DO NOT KNOW why I can just access usbMIDI here, but even when
// creating MIDI_obj globally, I could not access MIDI_obj the same way.
// Bringing in a pointer and working with MIDI_obj in this manner is
// because of that.

/// @brief Turns on sound over this string's MIDI channel at its current volume
/// @param my_offset The offset from the string's base note to make sound
/// @param my_modulation The amount of optional modulation (0-127) to apply to the sound.  This is MIDI CC1.  0 == no modulation.
/// @warning The way this is currently written, only one note may be playing per string object.  Don't call this twice in a row without calling soundOff() first.
void GurdyString::soundOn(int my_offset, int my_modulation) {
  note_being_played = open_note + my_offset;

  // If user has one of the second-drone options enabled, trigger them here.
  if (gros_mode == 1) {
    soundOn(0, 0, note_being_played - 5);
  } else if (gros_mode == 2) {
    soundOn(0, 0, note_being_played - 7);
  } else if (gros_mode == 3) {
    soundOn(0, 0, note_being_played - 12);
  };

  if (!mute_on) {

    usbMIDI.sendNoteOn(note_being_played, midi_volume, midi_channel);

    if (output_mode != 1) {
      MIDI.sendNoteOn(note_being_played, midi_volume, midi_channel);
    };

    if (output_mode > 0) {
      #if defined(USE_TRIGGER)
        trigger_obj.trackGain(note_being_played + (128 * (midi_channel - 1)), trigger_volume);
        trigger_obj.trackPlayPoly(note_being_played + (128 * (midi_channel - 1)), true);
        trigger_obj.trackLoop(note_being_played + (128 * (midi_channel - 1)), true);
      #elif defined(USE_TSUNAMI)
        trigger_obj.trackGain(note_being_played + (128 * (midi_channel - 1)), trigger_volume);
        trigger_obj.trackPlayPoly(note_being_played + (128 * (midi_channel - 1)), TSUNAMI_OUT, true);
        trigger_obj.trackLoop(note_being_played + (128 * (midi_channel - 1)), true);
      #endif
    };

    // If modulation isn't zero, send that as a MIDI CC for this channel
    // This is meant to be configured to create a gentle vibrato.
    if (my_modulation > 0) {
      usbMIDI.sendControlChange(1, my_modulation, midi_channel);
      
      if (output_mode != 1) {
        MIDI.sendControlChange(1, my_modulation, midi_channel);
      };
    };
  };

  is_playing = true;
};

/// @brief Turns on a specific note over this string's MIDI channel at its current volume.
/// @details This is meant to be for the second drone option, and bypasses the note_being_played logic.
/// @param my_offset The offset from the string's base note to make sound.  Unused here.
/// @param my_modulation The amount of optional modulation (0-127) to apply to the sound.  This is MIDI CC1.  0 == no modulation.  Unused here.
/// @param note The specific note to sound.
/// @warning my_modulation and my_offset only exist in this incarnation of the method because it keeps it from being ambiguous compared the other one.  Maybe I should change this.
void GurdyString::soundOn(int my_offset, int my_modulation, int note) {
  int note_to_play = note;
  if (!mute_on) {

    usbMIDI.sendNoteOn(note_to_play, midi_volume, midi_channel);

    if (output_mode != 1) {
      MIDI.sendNoteOn(note_to_play, midi_volume, midi_channel);
    };

    if (output_mode > 0) {
      #if defined(USE_TRIGGER)
        trigger_obj.trackGain(note_to_play + (128 * (midi_channel - 1)), trigger_volume);
        trigger_obj.trackPlayPoly(note_to_play + (128 * (midi_channel - 1)), true);
        trigger_obj.trackLoop(note_to_play + (128 * (midi_channel - 1)), true);
      #elif defined(USE_TSUNAMI)
        trigger_obj.trackGain(note_to_play + (128 * (midi_channel - 1)), trigger_volume);
        trigger_obj.trackPlayPoly(note_to_play + (128 * (midi_channel - 1)), TSUNAMI_OUT, true);
        trigger_obj.trackLoop(note_to_play + (128 * (midi_channel - 1)), true);
      #endif
    };
  };
};

/// @brief  Turns off the sound currently playing for this string, nicely.
void GurdyString::soundOff() {

  // If user has one of the second-drone options enabled, trigger them here.
  if (gros_mode == 1) {
    soundOff(note_being_played - 5);
  } else if (gros_mode == 2) {
    soundOff(note_being_played - 7);
  } else if (gros_mode == 3) {
    soundOff(note_being_played - 12);
  };

  usbMIDI.sendNoteOff(note_being_played, midi_volume, midi_channel);

  if (output_mode != 1) {
    MIDI.sendNoteOff(note_being_played, midi_volume, midi_channel);
  };

  if (output_mode > 0) {
    if (trigger_volume > -60) {
      trigger_obj.trackFade(note_being_played + (128 * (midi_channel - 1)), trigger_volume - 10, 200, true);
    } else {
      trigger_obj.trackFade(note_being_played + (128 * (midi_channel - 1)), -70, 200, true);
    }
    //trigger_obj.trackStop(note_being_played + (128 * (midi_channel - 1)));
  };

  is_playing = false;
};

/// @brief Turns off sound to a specific note.
/// @param note The specific note to stop
void GurdyString::soundOff(int note) {

  usbMIDI.sendNoteOff(note, midi_volume, midi_channel);

  if (output_mode != 1) {
    MIDI.sendNoteOff(note, midi_volume, midi_channel);
  };

  if (output_mode > 0) {
    if (trigger_volume > -60) {
      trigger_obj.trackFade(note + (128 * (midi_channel - 1)), trigger_volume - 10, 200, true);
    } else {
      trigger_obj.trackFade(note + (128 * (midi_channel - 1)), -70, 200, true);
    };
  };
};

/// @brief Issues a MIDI CC123 to the string's MIDI channel, killing all sound on it.
/// @note On Tsunami/Trigger units, this kills *all* tracks playing.  This is not meant be the regular way to turn off sound, see soundOff() which does it more gently.
void GurdyString::soundKill() {

  usbMIDI.sendControlChange(123, 0, midi_channel);

  if (output_mode != 1) {
    MIDI.sendControlChange(123, 0, midi_channel);
  };

  if (output_mode > 0) {
    trigger_obj.stopAllTracks();
  };

  is_playing = false;
};

/// @brief Returns the string's open (base) note.
/// @return The string's base note as a MIDI note number 0-127
int GurdyString::getOpenNote() {
  return open_note;
};

/// @brief Sets a new base note for this string.
/// @param new_note The new base MIDI note (0-127) for this string
void GurdyString::setOpenNote(int new_note) {
  open_note = new_note;
};

// MIDI volume is an integer between 0 (off) and 127 (full volume).

/// @brief Sets a new volume for this string.
/// @param vol The new MIDI volume for this string.  0 = silent, 127 = full volume.
/// @note On Tsunami/Trigger units, this is translated to the -70 to +10 scale used on those internally.
/// * MIDI volume 112 = line-out volume level on Tsunami/Trigger units.
void GurdyString::setVolume(int vol) {
  midi_volume = vol;
  trigger_volume = int((vol)/128.0 * 80 - 70);
};

/// @brief Returns the string's MIDI volume.
/// @return The string's volume, 0-127
int GurdyString::getVolume() {
  return midi_volume;
};

/// @brief Mutes/unmutes the string.
/// @param mute True = mute, false = unmute
/// @note While muted, soundOn/Off events do nothing.  This exists for programming ease (calling sounOn() on all strings without checking if they are muted).
void GurdyString::setMute(bool mute) {
  mute_on = mute;
};

/// @brief Returns the string's mute/unmute status.
/// @return True if muted, false if unmuted
bool GurdyString::getMute() {
  return mute_on;
};

/// @brief Reports if the string is currently playing a note.
/// @return True if playing a note, false otherwise.
bool GurdyString::isPlaying() {
  return is_playing;
};

/// @brief Send a MIDI Program Change to this string's MIDI channel.
/// @param program The program change value, 0-127.
/// @note This has no effect on Tsunami/Trigger units.
void GurdyString::setProgram(uint8_t program) {

  usbMIDI.sendProgramChange(program, midi_channel);

  if (output_mode != 1) {
    MIDI.sendProgramChange(program, midi_channel);
  };
};

/// @brief Sends a MIDI CC11 (Expression) value to this string's MIDI channel.
/// @param exp The expression value, 0-127.
/// @note This has no effect on Tsunami/Trigger units.
void GurdyString::setExpression(int exp) {

  usbMIDI.sendControlChange(11, exp, midi_channel);

  if (output_mode != 1) {
    MIDI.sendControlChange(11, exp, midi_channel);
  };
};

/// @brief Bends this string's sound to the specified amount.
/// @param bend The amount of pitch bend.  0 to 16383, where 8192 = no bend.
/// @note This has no effect on Tsunami/Trigger units.
void GurdyString::setPitchBend(int bend) {
  usbMIDI.sendPitchBend(bend, midi_channel);

  if (output_mode != 1) {
    MIDI.sendPitchBend(bend, midi_channel);
  };
};

/// @brief Sets the amount of modulation (vibrato) on this string.
/// @param vib The amount of modulation, 0-127.
/// @note This is MIDI CC1, the "mod wheel".  Intended to used for a vibrato effect.
void GurdyString::setVibrato(int vib) {
  usbMIDI.sendControlChange(1, vib, midi_channel);

  if (output_mode != 1) {
    MIDI.sendControlChange(1, vib, midi_channel);
  };
};

/// @brief Returns the text name of this string.
/// @return The string's display name
String GurdyString::getName() {
  return name;
};

/// @brief Sets the secondary output mode for this string
/// @param my_mode 0-2
/// @details Modes accepted:
/// * 0 - MIDI-OUT socket
/// * 1 - A Trigger/Tsunami device
/// * 2 - Both
void GurdyString::setOutputMode(int my_mode) {
  output_mode = my_mode;
};

/// @brief Sets the "gros-mode" for this string.
/// @details Value of this should be 0-3:
/// * 0 = no additional sound
/// * 1 = a perfect fourth below (-5 semitones)
/// * 2 = a perfect fifth below (-7 semitones)
/// * 3 = an octave below (-12 semitones)
/// @param my_gros_mode 
void GurdyString::setGrosMode(int my_gros_mode) {
  gros_mode = my_gros_mode;
};

int GurdyString::getGrosMode() {
  return gros_mode;
};

String GurdyString::getGrosString() {
  if (gros_mode == 0) {
    return String("None");
  };
  if (gros_mode == 1) {
    return getLongNoteNum(getOpenNote() - 5);
  };
  if (gros_mode == 2) {
    return getLongNoteNum(getOpenNote() - 7);
  };
  if (gros_mode == 3) {
    return getLongNoteNum(getOpenNote() - 12);
  };
};