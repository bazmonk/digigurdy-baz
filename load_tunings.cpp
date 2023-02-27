#include "load_tunings.h"

/// @brief Loads the given tuning preset.
/// @param preset 1-4, representing one of the EEPROM tuning preset slots to load.
/// @details see `default_tunings.h` for the actual presets themselves.
void load_preset_tunings(int preset) {
  const int *tunings;
  if (preset == 1) { tunings = PRESET1; };
  if (preset == 2) { tunings = PRESET2; };
  if (preset == 3) { tunings = PRESET3; };
  if (preset == 4) { tunings = PRESET4; };

  mystring->setOpenNote(tunings[0]);
  mylowstring->setOpenNote(tunings[1]);
  mydrone->setOpenNote(tunings[2]);
  mytromp->setOpenNote(tunings[3]);
  mybuzz->setOpenNote(tunings[4]);
  tpose_offset = tunings[5];
  capo_offset = tunings[6];
}

/// @brief Loads the given saved tuning slot.
/// @param slot 1-4, representing one of the EEPROM tuning save slots to load.
/// @details Sets tuning of all strings, tpose, capo, and volume.
void load_saved_tunings(int slot) {
  byte value;

  // Notes
  value = EEPROM.read(slot + EEPROM_HI_MEL);
  mystring->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_LO_MEL);
  mylowstring->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_DRONE);
  mydrone->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_TROMP);
  mytromp->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_BUZZ);
  mybuzz->setOpenNote(value);
  value = EEPROM.read(slot + EEPROM_TPOSE);
  tpose_offset = value - 12;
  value = EEPROM.read(slot + EEPROM_CAPO);
  capo_offset = value;

  // Volumes
  value = EEPROM.read(slot + EEPROM_HI_MEL_VOL);
  mystring->setVolume(value);
  value = EEPROM.read(slot + EEPROM_LOW_MEL_VOL);
  mylowstring->setVolume(value);
  value = EEPROM.read(slot + EEPROM_DRONE_VOL);
  mydrone->setVolume(value);
  value = EEPROM.read(slot + EEPROM_TROMP_VOL);
  mytromp->setVolume(value);
  value = EEPROM.read(slot + EEPROM_BUZZ_VOL);
  mybuzz->setVolume(value);
  value = EEPROM.read(slot + EEPROM_KEYCLICK_VOL);
  mykeyclick->setVolume(value);

  // Gros Modes
  mystring->setGrosMode(EEPROM.read(slot + EEPROM_HI_MEL_GROS));
  mylowstring->setGrosMode(EEPROM.read(slot + EEPROM_LOW_MEL_GROS));
  mytromp->setGrosMode(EEPROM.read(slot + EEPROM_TROMP_GROS));
  mydrone->setGrosMode(EEPROM.read(slot + EEPROM_DRONE_GROS));
  mybuzz->setGrosMode(EEPROM.read(slot + EEPROM_BUZZ_GROS));

};

/// @brief Displays a given saved slot tuning and prompts user to accept.
/// @param slot_num 1-4, the EEPROM tuning save slot to display and possibly load.
/// @return True if the user loaded the tuning, false if the user rejected it.
bool view_slot_screen(int slot_num) {
  int slot;
  if (slot_num == 1) { slot = EEPROM_SLOT1; };
  if (slot_num == 2) { slot = EEPROM_SLOT2; };
  if (slot_num == 3) { slot = EEPROM_SLOT3; };
  if (slot_num == 4) { slot = EEPROM_SLOT4; };

  String t_str = "";
  if (EEPROM.read(slot + EEPROM_TPOSE) > 12) { t_str += "+"; };
  t_str = t_str + ((EEPROM.read(slot + EEPROM_TPOSE))-12);

  String cap_str = "";
  if (EEPROM.read(slot + EEPROM_CAPO) > 0) { cap_str += "+"; };
  cap_str = cap_str + (EEPROM.read(slot + EEPROM_CAPO));

  print_tuning("Saved Slot Tuning",
               getLongNoteNum(EEPROM.read(slot + EEPROM_HI_MEL)),
               getLongNoteNum(EEPROM.read(slot + EEPROM_LO_MEL)),
               getLongNoteNum(EEPROM.read(slot + EEPROM_DRONE)),
               getLongNoteNum(EEPROM.read(slot + EEPROM_TROMP)),
               t_str, cap_str);
  delay(150);

  bool done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myAButton->update();
    myXButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      load_saved_tunings(slot);
      signal_scene_change(slot_num + 3); // Zero indexed and first 4 are reserved for presets
      done = true;

    } else if (my2Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

/// @brief Displays a given preset tuning slot and prompts user to accept.
/// @param preset 1-4, the preset tuning slot to display and possibly load.
/// @return True if the user loaded the tuning, false if the user rejected it.
bool view_preset_screen(int preset) {
  const int *tunings;
  if (preset == 1) { tunings = PRESET1; };
  if (preset == 2) { tunings = PRESET2; };
  if (preset == 3) { tunings = PRESET3; };
  if (preset == 4) { tunings = PRESET4; };

  String t_str = "";
  if (tunings[5] > 12) { t_str += "+"; };
  t_str = t_str + tunings[5];

  String cap_str = "";
  if (tunings[6] > 0) { cap_str += "+"; };
  cap_str = cap_str + tunings[6];

  print_tuning("Preset Tuning",
               getLongNoteNum(tunings[0]),
               getLongNoteNum(tunings[1]),
               getLongNoteNum(tunings[2]),
               getLongNoteNum(tunings[3]),
               t_str, cap_str);
  delay(150);

  bool done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myAButton->update();
    myXButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      load_preset_tunings(preset);
      signal_scene_change(preset - 1); // Zero indexed
      done = true;

    } else if (my2Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

/// @brief Send a scene-change signal (program change) on MIDI channel 1
/// @param scene_idx 0-127, representing the scene/program being changed to.
/// @details This is intended to be used with BS-16i, which will change its soundfonts and settings in reaction to these signals.
void signal_scene_change(int scene_idx) {
  if (scene_signal_type == 1) {
    // Signal as a Program Control message on Channel 1
    mystring->setProgram(scene_idx);
  } else {
    // 0 = Do nothing
    // While this should be 0, if there is bad data we'll just ignore it and do nothing.
  }
};