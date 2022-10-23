// Digigurdy-Baz

// AUTHOR: Basil Lalli
// DESCRIPTION: Digigurdy-Baz is a fork of the Digigurdy code by John Dingley.  See his page:
//   https://digigurdy.com/
//   https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy
// REPOSITORY: https://github.com/bazmonk/digigurdy-baz

#include <Adafruit_GFX.h>
// https://www.pjrc.com/teensy/td_libs_Bounce.html
#include <Bounce.h>
#include <EEPROM.h>
// https://www.pjrc.com/teensy/td_midi.html
// https://www.pjrc.com/teensy/td_libs_MIDI.html
#include <MIDI.h>
#include <ADC.h>

// These are found in the digigurdy-baz repository
#include "notes.h"           // Note and note string variables
#include "bitmaps.h"         // Pretty pictures
#include "eeprom_values.h"   // Save-slot memory addresses
#include "default_tunings.h" // Preset tunings.
#include "config.h"          // Configuration variables

#include "common.h"

// These are all about the display
#include "display.h"         // Intializes our display object
#include "startup_screens.h" // Startup-related screens.
#include "play_screens.h"    // The screens mid-play (notes and staffs)

// Right now not using the std namespace is just impacting strings.  That's ok...
using namespace MIDI_NAMESPACE;

// ##################
// GLOBAL SETUP STUFF
// ##################

// Create the serial interface object
SerialMIDI<HardwareSerial> mySerialMIDI(Serial1);
// Create a new MidiInterface object using that serial interface
MidiInterface<SerialMIDI<HardwareSerial>> *myMIDI;

// This is for the crank, the audio-to-digital chip
ADC* adc;

// Declare the "keybox" and buttons.
extern HurdyGurdy *mygurdy;
extern ToggleButton *bigbutton;
extern GurdyCrank *mycrank;

// As musical keys, these are referred to in the mygurdy object above.
// This declaration of them is specifically for their use as navigational
// buttons in the menu screens.  ok = O, back = X.
extern KeyboxButton *myAButton;
extern KeyboxButton *myXButton;
extern KeyboxButton *my1Button;
extern KeyboxButton *my2Button;
extern KeyboxButton *my3Button;
extern KeyboxButton *my4Button;
extern KeyboxButton *my5Button;
extern KeyboxButton *my6Button;

// For legacy button-combo support:
extern KeyboxButton *myAltTposeButton;
extern KeyboxButton *myAltTposeUp;
extern KeyboxButton *myAltTposeDown;
extern KeyboxButton *myBButton;

// Note that there aren't special classes for melody, drone, even the keyclick.
// They are differentiated in the main loop():
// * A melody string is one that changes with the keybox offset.
// * A drone/trompette is one that doesn't change.
// * The keyclick "string" is just a drone that comes on and off at particular times.
// * The buzz "string" is also just a drone that comes on/off at other particular times.
extern GurdyString *mystring;
extern GurdyString *mylowstring;
extern GurdyString *mykeyclick;
extern GurdyString *mytromp;
extern GurdyString *mydrone;
extern GurdyString *mybuzz;

// These are the dedicated transpose/capo buttons
extern GurdyButton *tpose_up;
extern GurdyButton *tpose_down;
extern GurdyButton *capo;

// These are the "extra" buttons, new on the rev3.0 gurdies
extern GurdyButton *ex1Button;
extern GurdyButton *ex2Button;
extern GurdyButton *ex3Button;

// This defines the +/- one octave transpose range.
const int max_tpose = 12;
int tpose_offset;

// This defines the 0, +2, +4 capo range.
const int max_capo = 4;
int capo_offset;

// The offset is given when we update the buttons each cycle.
// Buttons should only be updated once per cycle.  So we need this in the main loop()
// to refer to it several times.
int myoffset;

// true = G/C tuning, false = D/G.  For the menus.
bool gc_or_dg;

// this tracks the drone/trompette mute status.  Starts all-on.
// 0 = all on
// 1 = all off
// 2 = drone on, tromp off
// 3 = drone off, tromp on
int drone_mode = 0;

// similarly to drone_mode...
// 0 = all on
// 1 = high on, low off
// 2 = high off, low on
int mel_mode = 0;

int play_screen_type = 0;
uint8_t scene_signal_type = 0;

// Teensy and Arduino units start by running setup() once after powering up.
// Here we establish how the "gurdy" is setup, what strings to use, and we also
// start the MIDI communication.
void setup() {

  start_display();
  startup_screen_sqeuence();

  // Un-comment to print yourself debugging messages to the Teensyduino
  // serial console.
  Serial.begin(115200);
  delay(500);
  Serial.println("Hello.");

  myMIDI = new MidiInterface<SerialMIDI<HardwareSerial>>((SerialMIDI<HardwareSerial>&)mySerialMIDI);
  myMIDI->begin();

  adc = new ADC();
  mycrank = new GurdyCrank(15, A2, adc, LED_PIN);

  // The keybox arrangement is decided by pin_array, which is up in the CONFIG SECTION
  // of this file.  Make adjustments there.
  mygurdy = new HurdyGurdy(pin_array, num_keys);
  bigbutton = new ToggleButton(39);

  // These indices are defined in config.h
  myXButton = mygurdy->keybox[X_INDEX];
  myAButton = mygurdy->keybox[A_INDEX];
  myBButton = mygurdy->keybox[B_INDEX];

  my1Button = mygurdy->keybox[BUTTON_1_INDEX];
  my2Button = mygurdy->keybox[BUTTON_2_INDEX];
  my3Button = mygurdy->keybox[BUTTON_3_INDEX];
  my4Button = mygurdy->keybox[BUTTON_4_INDEX];
  my5Button = mygurdy->keybox[BUTTON_5_INDEX];
  my6Button = mygurdy->keybox[BUTTON_6_INDEX];

  myAltTposeUp = mygurdy->keybox[TPOSE_UP_INDEX];
  myAltTposeDown = mygurdy->keybox[TPOSE_DN_INDEX];


  // Which channel is which doesn't really matter, but I'm sticking with
  // John's channels so his videos on setting it up with a tablet/phone still work.
  //
  // This starting tuning here is replaced immediately upon startup with either a
  // preset, a saved tuning, or a created one.  The menu shouldn't let you actually
  // end up with this, but I have to initialize them with something, so might as well
  // be a working tuning.
  mystring = new GurdyString(1, Note(g4), myMIDI);
  mylowstring = new GurdyString(2, Note(g3), myMIDI);
  mytromp = new GurdyString(3, Note(c3), myMIDI);
  mydrone = new GurdyString(4, Note(c2), myMIDI);
  mybuzz = new GurdyString(5,Note(c3), myMIDI);
  mykeyclick = new GurdyString(6, Note(b5), myMIDI);


  tpose_up = new GurdyButton(22);   // A.k.a. the button formerly known as octave-up
  tpose_down = new GurdyButton(21); // A.k.a. the button formerly known as octave-down
  tpose_offset = 0;

  capo = new GurdyButton(23); // The capo button
  capo_offset = 0;

  ex1Button = new GurdyButton(41);
  ex2Button = new GurdyButton(17);
  ex3Button = new GurdyButton(14);

  scene_signal_type = EEPROM.read(EEPROM_SCENE_SIGNALLING);
};

void signal_scene_change(int scene_idx) {
  if (scene_signal_type == 1) {
    // Signal as a Program Control message on Channel 1
    mystring->setProgram(scene_idx);
  } else {
    // 0 = Do nothing
    // While this should be 0, if there is bad data we'll just ignore it and do nothing.
  }
};

// ##############
// MENU FUNCTIONS
// ##############
//
// Functions here drive the main behavior of the digigurdy.
//
// First-time hackers of this code: the loop() at the end of this is the main()
// of ardruino/teensy programs.  It runs in an endless loop.  The welcome_screen()
// is the first screen that comes after startup if you're trying to track down the flow.
// The pause_screen() is what comes up when X+A is pressed.  The rest of the functions/screens
// get called from them.  Hope that helps you find the part you care about!

// load_preset_tunings accepts an int between 1-4 and sets the appropriate preset.
// See the default_tunings.h file to modify what the presets actually are.
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

// load_saved_tunings requires one argument: the "save slot" which
// should be one of the EEPROM_SLOT[1-4] values in eeprom_values.h.
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

};

// save_tunings accepts one argument, which should be one of the
// EEPROM_SLOT[1-4] values defined in eeprom_values.h.
void save_tunings(int slot) {

  EEPROM.write(slot + EEPROM_HI_MEL, mystring->getOpenNote());
  EEPROM.write(slot + EEPROM_LO_MEL, mylowstring->getOpenNote());
  EEPROM.write(slot + EEPROM_DRONE, mydrone->getOpenNote());
  EEPROM.write(slot + EEPROM_TROMP, mytromp->getOpenNote());
  EEPROM.write(slot + EEPROM_BUZZ, mybuzz->getOpenNote());
  EEPROM.write(slot + EEPROM_TPOSE, tpose_offset + 12);
  EEPROM.write(slot + EEPROM_CAPO, capo_offset);
  EEPROM.write(slot + EEPROM_HI_MEL_VOL, mystring->getVolume());
  EEPROM.write(slot + EEPROM_LOW_MEL_VOL, mylowstring->getVolume());
  EEPROM.write(slot + EEPROM_DRONE_VOL, mydrone->getVolume());
  EEPROM.write(slot + EEPROM_TROMP_VOL, mytromp->getVolume());
  EEPROM.write(slot + EEPROM_BUZZ_VOL, mybuzz->getVolume());
  EEPROM.write(slot + EEPROM_KEYCLICK_VOL, mykeyclick->getVolume());

};

// This clears the EEPROM and overwrites it all with zeroes
void clear_eeprom() {
  // Not much to say here... write 0 everywhere:
  for (int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);
};

void tuning_hi_melody() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  String disp_str = "         ";
  if (gc_or_dg) {
    base_note = Note(g4);
    disp_str = disp_str + "G/C         \n";
  } else {
    base_note = Note(d4);
    disp_str = disp_str + "D/G         \n";
  };

  choice1 = base_note + 12;
  choice2 = base_note + 17;
  choice3 = base_note;
  choice4 = base_note + 5;

  disp_str = disp_str + \
  " Choose High Melody: \n"
  "                     \n"
  " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
  "                     \n"
  "                     \n"
  " A) Default (**)     \n"
  "                     \n";

  display.print(disp_str.c_str());
  display.display();

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mystring->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mystring->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mystring->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mystring->setOpenNote(choice4);
      done = true;
    };
  };
};

void tuning_low_melody() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;

  if (gc_or_dg) {
    base_note = Note(g4);
  } else {
    base_note = Note(d4);
  };

  choice1 = base_note;
  choice2 = base_note - 7;
  choice3 = base_note - 12;
  choice4 = base_note - 19;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  String disp_str = " High Melody:   " + NoteNum[mystring->getOpenNote()] + "   \n"
  " Choose Low Melody:  \n"
  "                     \n"
  " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
  "                     \n"
  "                     \n"
  " A) Default (**)     \n"
  "                     \n";

  display.print(disp_str.c_str());
  display.display();

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mylowstring->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mylowstring->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mylowstring->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mylowstring->setOpenNote(choice4);
      done = true;
    };
  };
};

void tuning_drone() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;
  int choice5;
  int choice6;

  if (gc_or_dg) {
    base_note = Note(g4);
    choice1 = base_note - 31;
    choice2 = base_note - 24;
    choice3 = base_note - 19;
    choice4 = base_note - 12;
    choice5 = base_note - 7;
    choice6 = base_note;

  } else {
    base_note = Note(d4);
    choice1 = base_note - 19;
    choice2 = base_note - 24;
    choice3 = base_note - 7;
    choice4 = base_note - 12;
    choice5 = base_note + 5;
    choice6 = base_note;
  };

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  String disp_str = " Hi/Lo Melody: " + NoteNum[mystring->getOpenNote()] + "/" + NoteNum[mylowstring->getOpenNote()] + " \n"
  "    Choose Drone:    \n"
  " 1) " + NoteNum[choice1] + "     2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + " **  4) " + NoteNum[choice4] + "     \n"
  " 5) " + NoteNum[choice5] + "     6) " + NoteNum[choice6] + "     \n"
  "                     \n"
  "                     \n"
  " A) Default (**)     \n";

  display.print(disp_str.c_str());
  display.display();

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myAButton->update();

    if (my1Button->wasPressed()) {
      mydrone->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mydrone->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed() || myAButton->wasPressed()) {
      mydrone->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mydrone->setOpenNote(choice4);
      done = true;
    } else if (my5Button->wasPressed()) {
      mydrone->setOpenNote(choice5);
      done = true;
    } else if (my6Button->wasPressed()) {
      mydrone->setOpenNote(choice6);
      done = true;
    };
  };
};

void tuning_tromp() {

  int base_note;
  int choice1;
  int choice2;
  int choice3;
  int choice4;
  int choice5;
  int choice6;

  if (gc_or_dg) {
    base_note = Note(g4);
  } else {
    base_note = Note(d4);
  };

  choice1 = base_note + 5;
  choice2 = base_note + 12;
  choice3 = base_note - 7;
  choice4 = base_note;
  choice5 = base_note - 19;
  choice6 = base_note - 12;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  String disp_str = " Hi/Lo Melody: " + NoteNum[mystring->getOpenNote()] + "/" + NoteNum[mylowstring->getOpenNote()] + " \n"
  " Drone: " + NoteNum[mydrone->getOpenNote()] + "           \n"
  "  Choose Trompette:  \n"
  " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
  " 5) " + NoteNum[choice5] + "     6) " + NoteNum[choice6] + "     \n"
  "                     \n"
  " A) Default (**)     \n";

  display.print(disp_str.c_str());
  display.display();

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myAButton->update();

    if (my1Button->wasPressed() || myAButton->wasPressed()) {
      mytromp->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mytromp->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed()) {
      mytromp->setOpenNote(choice3);
      done = true;
    } else if (my4Button->wasPressed()) {
      mytromp->setOpenNote(choice4);
      done = true;
    } else if (my5Button->wasPressed()) {
      mytromp->setOpenNote(choice5);
      done = true;
    } else if (my6Button->wasPressed()) {
      mytromp->setOpenNote(choice6);
      done = true;
    };

    // I think the buzz sounds best between D3 and D4, so this pops
    // the buzz tone up and down octaves until it lands there.
    int buzz_note = mytromp->getOpenNote();
    while (buzz_note > Note(d4)) {
      buzz_note -= 12;
    };
    while (buzz_note < Note(d3)) {
      buzz_note += 12;
    };
    mybuzz->setOpenNote(buzz_note);
  };
};

// This screen is for viewing a save slot's settings.
// It returns true if the user wants to use it, false
// otherwise.
// Accepts integer slot: the save slot number.
bool view_slot_screen(int slot_num) {
  int slot;
  if (slot_num == 1) { slot = EEPROM_SLOT1; };
  if (slot_num == 2) { slot = EEPROM_SLOT2; };
  if (slot_num == 3) { slot = EEPROM_SLOT3; };
  if (slot_num == 4) { slot = EEPROM_SLOT4; };

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(" -Saved Slot Tuning- \n");
  display.print(" Hi Melody: ");
  display.print(LongNoteNum[EEPROM.read(slot + EEPROM_HI_MEL)].c_str());
  display.print("  \n");
  display.print(" Lo Melody: ");
  display.print(LongNoteNum[EEPROM.read(slot + EEPROM_LO_MEL)].c_str());
  display.print("  \n");
  display.print(" Drone:     ");
  display.print(LongNoteNum[EEPROM.read(slot + EEPROM_DRONE)].c_str());
  display.print("  \n");
  display.print(" Trompette: ");
  display.print(LongNoteNum[EEPROM.read(slot + EEPROM_TROMP)].c_str());
  display.print("  \n");
  display.print(" Tpose: ");
  if (EEPROM.read(slot + EEPROM_TPOSE) > 12) { display.print("+"); };
  display.print((EEPROM.read(slot + EEPROM_TPOSE))-12);
  display.print("  Capo: ");
  if (EEPROM.read(slot + EEPROM_CAPO) > 0) { display.print("+"); };
  display.print(EEPROM.read(slot + EEPROM_CAPO));
  display.print("\n");
  display.print(" A or 1) Accept \n");
  display.print(" X or 2) Go Back  \n");

  display.display();

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

// This screen previews a preset's settings.
// Returns true if users wants to use it, false otherwise.
// Accept integer preset_num: the preset number.
bool view_preset_screen(int preset) {
  const int *tunings;
  if (preset == 1) { tunings = PRESET1; };
  if (preset == 2) { tunings = PRESET2; };
  if (preset == 3) { tunings = PRESET3; };
  if (preset == 4) { tunings = PRESET4; };

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(" ---Preset Tuning--- \n");
  display.print(" Hi Melody: ");
  display.print(LongNoteNum[tunings[0]].c_str());
  display.print("  \n");
  display.print(" Lo Melody: ");
  display.print(LongNoteNum[tunings[1]].c_str());
  display.print("  \n");
  display.print(" Drone:     ");
  display.print(LongNoteNum[tunings[2]].c_str());
  display.print("  \n");
  display.print(" Trompette: ");
  display.print(LongNoteNum[tunings[3]].c_str());
  display.print("  \n");
  display.print(" Tpose: ");
  if (tunings[5] > 12) { display.print("+"); };
  display.print(tunings[5]);
  display.print("  Capo: ");
  if (tunings[6] > 0) { display.print("+"); };
  display.print(tunings[6]);
  display.print("\n");
  display.print(" A or 1) Accept  \n");
  display.print(" X or 2) Go Back  \n");

  display.display();

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

// This screen asks the user to select a save slot for
// preview and optionally choosing.
bool load_saved_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " --Load Saved Slot-- \n"
    " Select for Preview: \n"
    "                     \n"
    "     1) Slot 1       \n"
    "     2) Slot 2       \n"
    "     3) Slot 3       \n"
    "     4) Slot 4       \n"
    "X or 5) Go Back      \n";

    display.print(disp_str.c_str());
    display.display();

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (view_slot_screen(1)) { done = true; };

    } else if (my2Button->wasPressed()) {
      if (view_slot_screen(2)) { done = true; };

    } else if (my3Button->wasPressed()) {
      if (view_slot_screen(3)) { done = true; };

    } else if (my4Button->wasPressed()) {
      if (view_slot_screen(4)) { done = true; };

    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

// This screen lets the user choose a preset.
bool load_preset_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ---Load A Preset--- \n"
    " Select a Preset:    \n"
    "                     \n"
    " 1) " + PRESET1_NAME + "\n"
    " 2) " + PRESET2_NAME + "\n"
    " 3) " + PRESET3_NAME + "\n"
    " 4) " + PRESET4_NAME + "\n"
    " X or 5) Go Back     \n";

    display.print(disp_str.c_str());
    display.display();

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (view_preset_screen(1)) {done = true;};

    } else if (my2Button->wasPressed()) {
      if (view_preset_screen(2)) {done = true;};

    } else if (my3Button->wasPressed()) {
      if (view_preset_screen(3)) {done = true;};
    } else if (my4Button->wasPressed()) {
      if (view_preset_screen(4)) {done = true;};
    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };
  return true;
};

// This screen lets you choose how the gurdy indicates the turning to the controller
void scene_options_screen() {
  bool done = false;
  while (!done) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " -Scene Signalliing- \n"
    " Select an Option:   \n"
    "                     \n"
    " 1) None             \n"
    " 2) Prog Chg, Ch. 1  \n"
    "                     \n"
    " X) Go Back          \n";

    display.print(disp_str.c_str());
    display.display();


    // Check the buttons
    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      scene_signal_type = 0;
      EEPROM.write(EEPROM_SCENE_SIGNALLING, scene_signal_type);
      done = true;
    } else if (my2Button->wasPressed()) {
      scene_signal_type = 1;
      EEPROM.write(EEPROM_SCENE_SIGNALLING, scene_signal_type);
      done = true;
    } else if (myXButton->wasPressed()) {
      done = true;
    }
  }
}

// This screen lets you choose what kind of display you want.
void playing_options_screen() {
  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " -Choose Play Screen-\n"
    " Select an Option:   \n"
    "                     \n"
    " 1) Big Note + Staff \n"
    "                     \n"
    " 2) Big Note         \n"
    "                     \n"
    " X) Go Back          \n";

    display.print(disp_str.c_str());
    display.display();


    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      play_screen_type = 0;
      EEPROM.write(EEPROM_DISPLY_TYPE, 0);

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      String disp_str = ""
      "  DISPLAY  \n"
      "  \n"
      "  SAVED \n";

      display.print(disp_str.c_str());
      display.display();
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {

      play_screen_type = 1;
      EEPROM.write(EEPROM_DISPLY_TYPE, 1);

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      String disp_str = ""
      "  DISPLAY  \n"
      "  \n"
      "   SAVED  \n";

      display.print(disp_str.c_str());
      display.display();
      delay(750);
      done = true;
    } else if (myXButton->wasPressed()) {
      done = true;
    };
  };
};

// This screen is for other setup options.  Currently, that's
// just an option to clear the EEPROM.
void options_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ------Options------ \n"
    " Select an Option:   \n"
    "                     \n"
    " 1) Clear EEPROM     \n"
    " 2) Playing Screen   \n"
    " 3) Scene Control    \n"
    "                     \n"
    " X) Go Back          \n";

    display.print(disp_str.c_str());
    display.display();


    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {

      clear_eeprom();

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      String disp_str = ""
      "  EEPROM  \n"
      "  \n"
      "  CLEARED \n";

      display.print(disp_str.c_str());
      display.display();
      delay(750);
      done = true;

    } else if (my2Button->wasPressed()) {
      playing_options_screen();
      done = true;
    } else if (my3Button->wasPressed()) {
      scene_options_screen();
      done = true;
    } else if (myXButton->wasPressed()) {
      done = true;
    };
  };
};

// This is the first screen after the credits n' stuff.
void welcome_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " -----DigiGurdy----- \n"
    " Select an Option:   \n"
    "                     \n"
    " 1) Load Preset      \n"
    " 2) Load Save Slot   \n"
    " 3) New Tuning Setup \n"
    " 4) Other Options    \n"
    "                     \n";

    display.print(disp_str.c_str());
    display.display();

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();

    if (my1Button->wasPressed()) {
      if (load_preset_screen()) {done = true;};

    } else if (my2Button->wasPressed()) {
      if (load_saved_screen()) {done = true;};

    } else if (my3Button->wasPressed()) {
      if (tuning()) {done = true;};

    } else if (my4Button->wasPressed()) {
      options_screen();
      // Not done = true here, we'd want to get prompted again.
    };
  };
};

// This is the tuning screen for an individual string.  Single argument is a GurdyString pointer.
void tune_string_screen(GurdyString *this_string) {
  bool done = false;
  int new_note = this_string->getOpenNote();
  delay(300);
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ---String Tuning--- \n"
    " 1) Tune Down        \n"
    " 2) Tune Up          \n\n";

    display.print(disp_str.c_str());

    display.setTextSize(2);
    disp_str = ""
    "  " + LongNoteNum[new_note] + "\n";

    display.print(disp_str.c_str());

    display.setTextSize(1);
    disp_str = "\n"
    " X) Done / Go Back   \n";

    display.print(disp_str.c_str());

    display.display();

    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (new_note > 24) {
        new_note -= 1;
        delay(300);
      };
    } else if (my1Button->beingPressed()) {
      if (new_note > 24) {
        new_note -= 1;
        delay(150);
      };
    } else if (my2Button->wasPressed()) {
      if (new_note < 111) {
        new_note += 1;
        delay(300);
      };
    } else if (my2Button->beingPressed()) {
      if (new_note < 111) {
        new_note += 1;
        delay(150);
      };
    } else if (myXButton->wasPressed()) {
      this_string->setOpenNote(new_note);
      done = true;
    };
  };
};

// This screen allows the user to make manual changes to each string.
void manual_tuning_screen() {

  while (true) {

    String disp_str = " ---Manual Tuning--- \n"
                      " Choose a string:    \n"
                      " 1) Hi Mel.- " + NoteNum[mystring->getOpenNote()] + " \n"
                      " 2) Lo Mel.- " + NoteNum[mylowstring->getOpenNote()] + " \n"
                      " 3) Drone. - " + NoteNum[mydrone->getOpenNote()] + " \n"
                      " 4) Tromp. - " + NoteNum[mytromp->getOpenNote()] + " \n"
                      " 5) Buzz   - " + NoteNum[mybuzz->getOpenNote()] + "  \n"
                      " X or 6) Go Back     \n";

    print_screen(disp_str);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      tune_string_screen(mystring);

    } else if (my2Button->wasPressed()) {
      tune_string_screen(mylowstring);

    } else if (my3Button->wasPressed()) {
      tune_string_screen(mydrone);

    } else if (my4Button->wasPressed()) {
      tune_string_screen(mytromp);

    } else if (my5Button->wasPressed()) {
      tune_string_screen(mybuzz);

    } else if (my6Button->wasPressed() || myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

// This is because Teensydruino GCC is old as hell, and to_string() doesn't work.
String vol_to_str(int my_vol) {
  char vol_char[3];
  sprintf(vol_char, "%3d", my_vol);
  int i;
  String s = "";
  for (i = 0; i < 3; i++) {
      s = s + vol_char[i];
  };
  return s;
};

void change_volume_screen(GurdyString *this_string) {
  bool done = false;
  int new_vol = this_string->getVolume();
  delay(300);
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ---String Volume--- \n"
    " 1) Volume Down      \n"
    " 2) Volume Up        \n\n";

    display.print(disp_str.c_str());

    display.setTextSize(2);
    disp_str = ""
    "   " + vol_to_str(new_vol) + "\n";

    display.print(disp_str.c_str());

    display.setTextSize(1);
    disp_str = "\n"
    " X) Done / Go Back   \n";

    display.print(disp_str.c_str());

    display.display();

    my1Button->update();
    my2Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (new_vol > 0) {
        new_vol -= 1;
        delay(300);
      };
    } else if (my1Button->beingPressed()) {
      if (new_vol > 0) {
        new_vol -= 1;
        delay(100);
      };
    } else if (my2Button->wasPressed()) {
      if (new_vol < 127) {
        new_vol += 1;
        delay(300);
      };
    } else if (my2Button->beingPressed()) {
      if (new_vol < 127) {
        new_vol += 1;
        delay(100);
      };
    } else if (myXButton->wasPressed()) {
      this_string->setVolume(new_vol);
      done = true;
    };
  };
};

// This screen allows the user to make manual changes to each string's volume.
void volume_screen() {

  while (true) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ------Volume------- \n"
    " 1) Hi Mel.- " + vol_to_str(mystring->getVolume()) + " \n"
    " 2) Lo Mel.- " + vol_to_str(mylowstring->getVolume()) + " \n"
    " 3) Drone. - " + vol_to_str(mydrone->getVolume()) + " \n"
    " 4) Tromp. - " + vol_to_str(mytromp->getVolume()) + " \n"
    " 5) Buzz   - " + vol_to_str(mybuzz->getVolume()) + " \n"
    " 6) Click  - " + vol_to_str(mykeyclick->getVolume()) + " \n"
    " X) Go Back     \n";

    display.print(disp_str.c_str());
    display.display();

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    my6Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      change_volume_screen(mystring);

    } else if (my2Button->wasPressed()) {
      change_volume_screen(mylowstring);

    } else if (my3Button->wasPressed()) {
      change_volume_screen(mydrone);

    } else if (my4Button->wasPressed()) {
      change_volume_screen(mytromp);

    } else if (my5Button->wasPressed()) {
      change_volume_screen(mybuzz);

    } else if (my6Button->wasPressed()) {
      change_volume_screen(mykeyclick);

    } else if (myXButton->wasPressed()) {
      return;
    };
  };

  return;
};

bool tuning() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ----Tuning Menu---- \n"
    "  1) G/C, Guided     \n"
    "  2) D/G, Guided     \n"
    "  3) Manual Setup    \n"
    "                     \n"
    "  4) Volume Control  \n"
    "                     \n"
    "  X or 5) Go Back    \n";

    display.print(disp_str.c_str());
    display.display();

    delay(250);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      gc_or_dg = true;
      done = true;

    } else if (my2Button->wasPressed()) {
      gc_or_dg = false;
      done = true;

    } else if (my3Button->wasPressed()) {
      manual_tuning_screen();
    } else if (my4Button->wasPressed()) {
      volume_screen();
    } else if (my5Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  tuning_hi_melody();
  tuning_low_melody();
  tuning_drone();
  tuning_tromp();

  // SUMMARY
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  String disp_str = ""
  " -----Summary:------ \n"
  "  High Melody:   " + NoteNum[mystring->getOpenNote()] + "  \n"
  "   Low Melody:   " + NoteNum[mylowstring->getOpenNote()] + "  \n"
  "        Drone:   " + NoteNum[mydrone->getOpenNote()] + "  \n"
  "    Trompette:   " + NoteNum[mytromp->getOpenNote()] + "  \n"
  "                     \n"
  "A or 1) Accept     \n"
  "X or 2) Go Back    \n";

  display.print(disp_str.c_str());
  display.display();

  delay(250);

  done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    myAButton->update();
    myXButton->update();

    if (myAButton->wasPressed() || my1Button->wasPressed()) {
      done = true;
      return true;

    } else if (myXButton->wasPressed() || my2Button->wasPressed()) {
      return false;
      done = true;
    };
  };
  return true;
};

// This screen prompts which kind of tuning to load.
bool load_tuning_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ----Load Tuning---- \n"
    "                     \n"
    " 1) Preset Tuning    \n"
    "                     \n"
    " 2) Saved Tuning     \n\n"
    " X or 3) Go Back     \n";

    display.print(disp_str.c_str());
    display.display();

    delay(250);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (load_preset_screen()) {
        done = true;
      };

    } else if (my2Button->wasPressed()) {
      if (load_saved_screen()) {
        done = true;
      };

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};

// Checks if save slot is occupied and prompts user to overwrite if necessary.
// Returns true if slot is empty or OK to overwrite.
bool check_save_tuning(int slot) {

  if (EEPROM.read(slot) == 0) {
    return true;
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ----Save Tuning---- \n"
    "                     \n"
    " Save slot is full,  \n"
    "    Save anyway?     \n"
    "                     \n"
    " 1) Overwrite        \n"
    "                     \n"
    " X or 2) Go Back     \n";

    display.print(disp_str.c_str());
    display.display();

    bool done = false;
    while (!done) {

      my1Button->update();
      my2Button->update();
      myXButton->update();

      if (my1Button->wasPressed()) {
        return true;

      } else if (my2Button->wasPressed() || myXButton->wasPressed()) {
        return false;
      };
    };
    return false;
  };
};

// This is the screen for saving to a save slot
void save_tuning_screen() {

  bool done = false;
  int slot = 0;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ----Save Tuning---- \n"
    " Choose a Save Slot: \n"
    "                     \n"
    "      1) Slot 1      \n"
    "      2) Slot 2      \n"
    "      3) Slot 3      \n"
    "      4) Slot 4      \n"
    "      X) Go Back     \n";

    display.print(disp_str.c_str());
    display.display();

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT1)) {
        save_tunings(EEPROM_SLOT1);
        slot = 1;
        done = true;
      };

    } else if (my2Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT2)) {
        save_tunings(EEPROM_SLOT2);
        slot = 2;
        done = true;
      };

    } else if (my3Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT3)) {
        save_tunings(EEPROM_SLOT3);
        slot = 3;
        done = true;
      };

    } else if (my4Button->wasPressed()) {
      if (check_save_tuning(EEPROM_SLOT4)) {
        save_tunings(EEPROM_SLOT4);
        slot = 4;
        done = true;
      };

    } else if (myXButton->wasPressed()) {
      // Just return.
      return;
    };
  };

  // Display a confirmation message
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("\n  Slot ");
  display.print(slot);
  display.print("\n  Saved!");
  display.display();
  delay(500);
};

void redetect_crank_screen() {

  // In case the user has already tried removing it and it's freaking out, let's kill the sound.
  mystring->soundKill();
  mylowstring->soundKill();
  mykeyclick->soundKill();
  mytromp->soundKill();
  mydrone->soundKill();
  mybuzz->soundKill();

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    "                     \n"
    "                     \n"
    " Remove/Attach Crank \n"
    " And press 1...      \n";

    display.print(disp_str.c_str());
    display.display();

    my1Button->update();

    if (my1Button->wasPressed()) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(" DigiGurdy");
      display.setTextSize(1);
      display.println(" --------------------");
      display.setTextSize(2);
      display.println("   Crank  ");

      if (mycrank->isDetected()) {
        display.println(" Detected ");
      } else {
        display.println("   Absent ");
      };

      display.display();
      delay(1000);

      done = true;
    };
  };
};

void options_about_screen() {

  about_screen();

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("      DigiGurdy      ");
  display.println("---------------------");
  display.println("Special Thanks:      ");
  display.println("                     ");
  display.println("John Dingley         ");
  display.println("David Jacobs         ");
  display.println("lune36400            ");
  display.println("SalusaSecondus       ");
  display.display();

  while (true) {
    myXButton->update();

    if(myXButton->wasPressed()) {
      break;
    };
  };
};

bool other_options_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ---Other Options--- \n"
    "                     \n"
    " 1) Remove/Attach    \n"
    "      Crank          \n\n"
    " 2) About DigiGurdy  \n\n"
    " X or 3) Go Back     \n";

    display.print(disp_str.c_str());
    display.display();

    my1Button->update();
    my2Button->update();
    my3Button->update();
    myXButton->update();

    if (my1Button->wasPressed()) {
      redetect_crank_screen();
      done = true;

    } else if (my2Button->wasPressed()) {
      options_about_screen();

    } else if (my3Button->wasPressed() || myXButton->wasPressed()) {
      return false;
    };
  };

  return true;
};

// This is the screen that X+A gets you.
void pause_screen() {

  bool done = false;
  while (!done) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    String disp_str = ""
    " ----Pause  Menu---- \n"
    " 1) Load    2) Save  \n"
    " 3) Tuning  4) Other \n\n"
    " X, 5 or ex1) Go Back\n\n";

    if (drone_mode == 0) {
      disp_str = disp_str + "A) Drone:On ,Trmp:On \n";
    } else if (drone_mode == 1) {
      disp_str = disp_str + "A) Drone:Off,Trmp:Off\n";
    } else if (drone_mode == 2) {
      disp_str = disp_str + "A) Drone:On, Trmp:Off\n";
    } else if (drone_mode == 3) {
      disp_str = disp_str + "A) Drone:Off,Trmp:On \n";
    };

    if (mel_mode == 0) {
      disp_str = disp_str + "B) High:On ,  Low:On \n";
    } else if (mel_mode == 1) {
      disp_str = disp_str + "B) High:On ,  Low:Off\n";
    } else if (mel_mode == 2) {
      disp_str = disp_str + "B) High:Off,  Low:On \n";
    };

    display.print(disp_str.c_str());
    display.display();

    delay(150);

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    my5Button->update();
    myXButton->update();
    myAButton->update();
    myBButton->update();
    ex1Button->update();

    if (my1Button->wasPressed()) {
      if (load_tuning_screen()) {
        done = true;
      };

    } else if (my2Button->wasPressed()) {
      save_tuning_screen();
      done = true;

    } else if (my3Button->wasPressed()) {
      if (tuning()) {
        done = true;
      };

    } else if (my4Button->wasPressed()) {
      if (other_options_screen()) {
        done = true;
      };

    } else if (my5Button->wasPressed() || myXButton->wasPressed() || ex1Button->wasPressed()) {
      done = true;

    } else if (myAButton->wasPressed()) {
      if (drone_mode == 0) {
        drone_mode = 1; // 1 == both off
        mydrone->setMute(true);
        mytromp->setMute(true);
      } else if (drone_mode == 1) {
        drone_mode = 2; // 2 == drone on, tromp off
        mydrone->setMute(false);
        mytromp->setMute(true);
      } else if (drone_mode == 2) {
        drone_mode = 3; // 3 == drone off, tromp on
        mydrone->setMute(true);
        mytromp->setMute(false);
      } else if (drone_mode == 3) {
        drone_mode = 0; // 0 == both on
        mydrone->setMute(false);
        mytromp->setMute(false);
      };
    } else if (myBButton->wasPressed()) {
      if (mel_mode == 0) {
        mel_mode = 1; // 1 == high on, low off
        mystring->setMute(false);
        mylowstring->setMute(true);
      } else if (mel_mode == 1) {
        mel_mode = 2; // 2 == high off, low on
        mystring->setMute(true);
        mylowstring->setMute(false);
      } else if (mel_mode == 2) {
        mel_mode = 0; // 0 == high on, low on
        mystring->setMute(false);
        mylowstring->setMute(false);
      };
    };
  };

  // Crank On! for half a sec.
  display.clearDisplay();
  display.drawBitmap(0, 0, crank_on_logo, 128, 64, 1);
  display.display();
  delay(750);
};

// ###########
//  MAIN LOOP
// ###########

bool first_loop = true;

int test_count = 0;
int start_time = millis();

int stopped_playing_time = 0;
bool note_display_off = true;

// The loop() function is repeatedly run by the Teensy unit after setup() completes.
// This is the main logic of the program and defines how the strings, keys, click, buzz,
// and buttons acutally behave during play.
void loop() {
  // loop() actually runs too fast and gets ahead of hardware calls if it's allowed to run freely.
  // This was noticeable in older versions when the crank got "stuck" and would not buzz and took
  // oddly long to stop playing when the crank stopped moving.
  //
  // A microsecond delay here lets everything keep up with itself.  The exactly delay is set at
  // the top in the config section.
  //delayMicroseconds(LOOP_DELAY);

  if (first_loop) {
    welcome_screen();

    // This might have been reset above, so grab it now.
    play_screen_type = EEPROM.read(EEPROM_DISPLY_TYPE);

    // Crank On! for half a sec.
    display.clearDisplay();
    display.drawBitmap(0, 0, crank_on_logo, 128, 64, 1);
    display.display();
    delay(750);

    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, 0, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    first_loop = false;
  };

  // Update the keys, buttons, and crank status (which includes the buzz knob)
  myoffset = mygurdy->getMaxOffset();  // This covers the keybox buttons.
  bigbutton->update();
  mycrank->update();
  tpose_up->update();
  tpose_down->update();
  capo->update();

  myAButton->update();
  myXButton->update();

  ex1Button->update();
  ex2Button->update();
  ex3Button->update();

  // If the "X" and "O" buttons are both down, or if the first extra button is pressed,
  // trigger the tuning menu
  if ((myAButton->beingPressed() && myXButton->beingPressed()) || ex1Button->wasPressed()) {

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

  // Check for a capo shift.
  if (capo->wasPressed() ||
      (myXButton->beingPressed() && myBButton->wasPressed())) {

    capo_offset += 2;

    if (capo_offset > max_capo ) {
      capo_offset = 0;
    };

    if (mycrank->isSpinning() || bigbutton->toggleOn()) {
      mytromp->soundOff();
      mydrone->soundOff();
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mylowstring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                 tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };

  // As long as we're in playing mode--acutally playing or not--
  // check for a tpose shift.
  if ((tpose_up->wasPressed() ||
      (my1Button->beingPressed() && myAltTposeUp->wasPressed())) && (tpose_offset < max_tpose)) {
    tpose_offset += 1;

    if (mycrank->isSpinning() || bigbutton->toggleOn()) {
      mytromp->soundOff();
      mydrone->soundOff();
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mylowstring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                   tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };
  if ((tpose_down->wasPressed() ||
      (my1Button->beingPressed() && myAltTposeDown->wasPressed())) && (max_tpose + tpose_offset > 0)) {
    tpose_offset -= 1;

    if (mycrank->isSpinning() || bigbutton->toggleOn()) {
      mytromp->soundOff();
      mydrone->soundOff();
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mylowstring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);

      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
    } else {
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                   tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };

  // If ex2 is pressed during play, cycle through the melody string on/off options.
  if (ex2Button->wasPressed()) {
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

  // If ex3 is pressed during play, cycle thought the drone/trompette on/off options.
  if (ex3Button->wasPressed()) {
    if (drone_mode == 0) {
      drone_mode = 1; // 1 == both off
      mydrone->setMute(true);
      mytromp->setMute(true);
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
      if (mydrone->isPlaying()) {
        mydrone->soundOff();
        mydrone->soundOn();
      };
    } else if (drone_mode == 2) {
      drone_mode = 3; // 3 == drone off, tromp on
      mydrone->setMute(true);
      mytromp->setMute(false);
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

  // NOTE:
  // We don't actually do anything if nothing changed this cycle.  Strings stay on/off automatically,
  // and the click sound goes away because of the sound in the soundfont, not the length of the
  // MIDI note itself.  We just turn that on and off like the other strings.

  // If the big button is toggled on or the crank is active (i.e., if we're making noise):
  if (bigbutton->toggleOn() || mycrank->isSpinning()) {

    // Turn on the strings without a click if:
    // * We just hit the button and we weren't cranking, OR
    // * We just started cranking and we hadn't hit the button.
    if (bigbutton->wasPressed() && !mycrank->isSpinning()) {
      mystring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mylowstring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);

    } else if (mycrank->startedSpinning() && !bigbutton->toggleOn()) {
      mystring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mylowstring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);

    // Turn off the previous notes and turn on the new one with a click if new key this cycle.
    // NOTE: I'm not touching the drone/trompette.  Just leave it on if it's a key change.
    } else if (mygurdy->higherKeyPressed() || mygurdy->lowerKeyPressed()) {
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mylowstring->soundOn(myoffset + tpose_offset, MELODY_VIBRATO);
      mykeyclick->soundOn(tpose_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type);
    };

    // Whenever we're playing, check for buzz.
    if (mycrank->startedBuzzing()) {
      mybuzz->soundOn(tpose_offset + capo_offset);
    };

    if (mycrank->stoppedBuzzing()) {
      mybuzz->soundOff();
    };

  // If the toggle came off and the crank is off, turn off sound.
  } else if (bigbutton->wasReleased() && !mycrank->isSpinning()) {
    mystring->soundOff();
    mylowstring->soundOff();
    mykeyclick->soundOff();  // Not sure if this is necessary... but it feels right.
    mytromp->soundOff();
    mydrone->soundOff();
    mybuzz->soundOff();

    // Send a CC 123 (all notes off) to be sure.  This causes turning off sound via the big
    // button to basically be a MIDI kill button.
    mystring->soundKill();
    mylowstring->soundKill();
    mykeyclick->soundKill();
    mytromp->soundKill();
    mydrone->soundKill();
    mybuzz->soundKill();

    // Mark the time we stopped playing and trip the turn-off-the-display flag
    stopped_playing_time = millis();
    note_display_off = false;

  // If the crank stops and the toggle was off, turn off sound.
  } else if (mycrank->stoppedSpinning() && !bigbutton->toggleOn()) {
    mystring->soundOff();
    mylowstring->soundOff();
    mykeyclick->soundOff();
    mytromp->soundOff();
    mydrone->soundOff();
    mybuzz->soundOff();

    stopped_playing_time = millis();
    note_display_off = false;
  };

  // Once the sound stops, track the time until a fifth of a second has passed, then go back to the
  // non-playing display.  This just makes the display looks a bit nicer even if the sound jitters a
  // little.  It's subtle but I like the touch.
  if (!note_display_off && !bigbutton->toggleOn() && !mycrank->isSpinning()) {
    if ((millis() - stopped_playing_time) > 200) {
      note_display_off = true;
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };

  // Apparently we need to do this to discard incoming data.
  while (myMIDI->read()) {
  };
  while (usbMIDI.read()) {
  };

  test_count +=1;
  if (test_count > 100000) {
    test_count = 0;
    Serial.print("100,000 loop()s took: ");
    Serial.print(millis() - start_time);
    Serial.print("ms.  Avg Velocity: ");
    Serial.print(mycrank->getVAvg());
    Serial.print("rpm. Transitions: ");
    Serial.print(mycrank->getCount());
    Serial.print(", est. rev: ");
    Serial.println(mycrank->getRev());
    start_time = millis();
  }
};
