// Digigurdy-Baz
// VERSION: v0.3
// AUTHOR: Basil Lalli
// DESCRIPTION: Digigurdy-Baz is a fork of the Digigurdy code by John Dingley.  See his page:
//   https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy
// REPOSITORY: https://github.com/bazmonk/digigurdy-baz

// ##############
// CONFIG SECTION
// ##############

// USERS!!! Uncomment one of these lines depending on what kind of OLED screen you have.
#define WHITE_OLED
//#define BLUE_OLED

// ##################
// END CONFIG SECTION
// ##################

#include <Adafruit_GFX.h>
// https://www.pjrc.com/teensy/td_libs_Bounce.html
#include <Bounce.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
// https://www.pjrc.com/teensy/td_midi.html
// https://www.pjrc.com/teensy/td_libs_MIDI.html
#include <MIDI.h>
#include <string>

// The white OLED uses Adafruit SSD1306.  Blue uses SH1106.
#ifdef WHITE_OLED
  #include <Adafruit_SSD1306.h>
#endif
#ifdef BLUE_OLED
  #include <Adafruit_SH1106.h>
#endif

// These are found in the digigurdy-baz repository
#include "bitmaps.h"
#include "songs.h"

// Right not not using the std namespace is just impacting strings.  That's ok...
using namespace MIDI_NAMESPACE;

// #################
// CLASS DEFINITIONS
// #################

class HurdyGurdy {
  protected:

  public:
    HurdyGurdy(int pin_arr[]) {
    };

};

//  Button objects abstract the physical buttons.
class GurdyButton {
  protected:
    Bounce* bounce_obj;
  public:

    // Button() accepts two integers:
    //   pin - the teensy pin that goes with this button
    GurdyButton(int my_pin) {

      // The 5 is 5ms wait for events to complete.
      // Recommended from the Bounce webpage for "good" buttons.
      bounce_obj = new Bounce(my_pin, 5);

      // In John's code the non-keybox buttons weren't using Bounce.
      // I'm taking a guess that INPUT_PULLUP is what I want to use here but I think so.
      // Originally it was using DirectRead/Write and INPUT.
      pinMode(my_pin, INPUT_PULLUP);
    };

    // Bounce objects should only be update()-ed once per loop(),
    // so I'm not putting it in wasPressed()/wasReleased().
    void update() {
      bounce_obj->update();
    };

    bool wasPressed() {
      return bounce_obj->fallingEdge();
    }

    bool wasReleased() {
      return bounce_obj->risingEdge();
    }
};

//class ToggleButton is for the arcade-style drone on/off button
// It extends the GurdyButton class to keep track of being toggled.
class ToggleButton: public GurdyButton {
  private:
    bool toggled;
  public:
    ToggleButton(int my_pin, bool start_toggled = false) : GurdyButton(my_pin) {
      bounce_obj = new Bounce(my_pin, 5);
      pinMode(my_pin, INPUT_PULLUP);
      toggled = start_toggled;
    };

    // update() works a little differently and also checks the toggle status.
    void update() {
      bounce_obj->update();

      // We'll only look at the downpress to register the event.
      if(bounce_obj->fallingEdge()) {
        toggled = !toggled;
      };
    };

    bool toggleOn() {
      return toggled;
    };
};

// class KeyboxButton adds a note offset variable to the Button class.
class KeyboxButton: public GurdyButton {
  private:
    int note_offset;

  public:
    KeyboxButton(int my_pin, int my_offset) : GurdyButton(my_pin) {
      bounce_obj = new Bounce(my_pin, 5);
      pinMode(my_pin, INPUT_PULLUP);
      note_offset = my_offset;
    };

    int getOffset() {
      return note_offset;
    }

};

// GurdyString manages turning "strings" on and off and determining their note.
class GurdyString {
  private:
    int open_note;
    int midi_channel;
    int midi_volume;
    int note_being_played;
    MidiInterface<SerialMIDI<HardwareSerial>> *MIDI_obj;

  public:
    GurdyString(int my_channel, int my_note, midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *my_MIDI_obj, int my_vol = 56) {
      midi_channel = my_channel;
      open_note = my_note;
      midi_volume = my_vol;
      note_being_played = open_note;
      MIDI_obj = my_MIDI_obj;
    }

    // soundOn() sends sound on this string's channel at its notes
    // optionally with an additional offset (e.g. a key being pressed)
    //
    // I do not know why I can just access usbMIDI here, but even when
    // creating MIDI_obj globally, I could not access MIDI_obj the same way.
    // Bringing in a pointer and working with MIDI_obj in this manner is
    // because of that.
    void soundOn(int my_offset = 0) {
      note_being_played = open_note + my_offset;
      usbMIDI.sendNoteOn(note_being_played, midi_volume, midi_channel);
      MIDI_obj->sendNoteOn(note_being_played, midi_volume, midi_channel);
    };

    void soundOff() {
      usbMIDI.sendNoteOff(note_being_played, midi_volume, midi_channel);
      MIDI_obj->sendNoteOff(note_being_played, midi_volume, midi_channel);
    };

};

// class GurdyCrank controls the cranking mechanism.
class GurdyCrank {

};

class KeyClick {
};

class Buzz {
};

// ################
// GLOBAL VARIABLES
// ################

// enum Note maps absolute note names to MIDI note numbers (middle C4 = 60),
// which range from 0 to 127.
enum Note {
  c_1, c_1s, d_1, d_1s, e_1, f_1, f_1s, g_1, g_1s, a_1, a_1s, b_1,
  c0, c0s, d0, d0s, e0, f0, f0s, g0, g0s, a0, a0s, b0,
  c1, c1s, d1, d1s, e1, f1, f1s, g1, g1s, a1, a1s, b1,
  c2, c2s, d2, d2s, e2, f2, f2s, g2, g2s, a2, a2s, b2,
  c3, c3s, d3, d3s, e3, f3, f3s, g3, g3s, a3, a3s, b3,
  c4, c4s, d4, d4s, e4, f4, f4s, g4, g4s, a4, a4s, b4,
  c5, c5s, d5, d5s, e5, f5, f5s, g5, g5s, a5, a5s, b5,
  c6, c6s, d6, d6s, e6, f6, f6s, g6, g6s, a6, a6s, b6,
  c7, c7s, d7, d7s, e7, f7, f7s, g7, g7s, a7, a7s, b7,
  c8, c8s, d8, d8s, e8, f8, f8s, g8, g8s, a8, a8s, b8,
  c9, c9s, d9, d9s, e9, f9, f9s, g9
};

// string array NoteNum is the reverse of the above Note enum.  It maps MIDI note numbers to
// screen-friendly note names.
std::string NoteNum[] = {
  "C-1", "C-1#", "D-1", "D-1#", "E-1", "F-1", "F-1#", "G-1", "G-1#", "A-1", "A-1#", "B-1",
  "C0", "C0#", "D0", "D0#", "E0", "F0", "F0#", "G0", "G0#", "A0", "A0#", "B0",
  "C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1",
  "C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2",
  "C3", "C3#", "D3", "D3#", "E3", "F3", "F3#", "G3", "G3#", "A3", "A3#", "B3",
  "C4", "C4#", "D4", "D4#", "E4", "F4", "F4#", "G4", "G4#", "A4", "A4#", "B4",
  "C5", "C5#", "D5", "D5#", "E5", "F5", "F5#", "G5", "G5#", "A5", "A5#", "B5",
  "C6", "C6#", "D6", "D6#", "E6", "F6", "F6#", "G6", "G6#", "A6", "A6#", "B6",
  "C7", "C7#", "D7", "D7#", "E7", "F7", "F7#", "G7", "G7#", "A7", "A7#", "B7",
  "C8", "C8#", "D8", "D8#", "E8", "F8", "F8#", "G8", "G8#", "A8", "A8#", "B8",
  "C9", "C9#", "D9", "D9#", "E9", "F9", "F9#", "G9"
};

// Eventually I'll move this to a header, but the array index here represents
// the note offset, and the value is the corresponding teensy pin.
// Modifying this adds extra keys.
int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
                   7, 31, 8, 32, 33, 18, 34, 19, 35, 36, 20, 37};

// See https://www.pjrc.com/teensy/td_libs_MIDI.html
//
// Create the MIDI instance here.
// This used to be `MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);`, which would
// create a MidiInterface object name "MIDI" for you.  Doing it this way gives me pointer access
// to the object and lets me pass it to the GurdyString objects later.
//
// I am just doing what MIDI_CREATE_INSTANCE does behind the scenes, but not behind the scenes.

// Create the serial interface object
SerialMIDI<HardwareSerial> mySerialMIDI(Serial1);
// Create a new MidiInterface object using that serial interface
MidiInterface<SerialMIDI<HardwareSerial>> *myMIDI = new MidiInterface<SerialMIDI<HardwareSerial>>((SerialMIDI<HardwareSerial>&)mySerialMIDI);

// Just showing that it works.  Create a button object for pin 24 (this is the 1st bottom key)
GurdyButton *mybutton;
ToggleButton *bigbutton;
// Create two strings
GurdyString *mystring;
GurdyString *mylowstring;
GurdyString *mydrone;

void setup() {
  // Testing setup: initialize the button and peg it to "key 1".
  // Initialize two strings a perfect fifth apart.
  mybutton = new GurdyButton(24);
  bigbutton = new ToggleButton(39);
  mystring = new GurdyString(1, Note(g4), myMIDI);
  mylowstring = new GurdyString(2, Note(c4), myMIDI);
  mydrone = new GurdyString(3, Note(g3), myMIDI);

  myMIDI->begin();
};

void loop() {

  // Just for testing: make the note when the button is pressed.
  mybutton->update();
  bigbutton->update();

  if(bigbutton->toggleOn() && bigbutton->wasPressed()) {
    mydrone->soundOn();
  } else if (!(bigbutton->toggleOn()) && bigbutton->wasPressed()) {
    mydrone->soundOff();
  };

  if(mybutton->wasPressed()) {
    mystring->soundOn();
    mylowstring->soundOn();
  };
  if(mybutton->wasReleased()) {
    mystring->soundOff();
    mylowstring->soundOff();
  };
};
