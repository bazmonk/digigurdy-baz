// Digigurdy-Baz
// VERSION: v0.8
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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64

// Eventually I'll move this to a header, but the pin_array[] index here represents
// the MIDI note offset, and the value is the corresponding teensy pin.
// This defines which physical keys are part of the "keybox" and what order they're in.
//
// NOTE: There's no key that produces 0 offset (an "open" string),
// so the first element is bogus.  It gets skipped entirely.
int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
                   7, 31, 8, 32, 33, 18, 34, 19, 35, 36, 20, 37};

// This is literally just the size of the above array minus one.  I need this as a const to
// declare the KeyboxButton array later on... or I just don't know enough C++ to know how to
// *not* need it ;-)
const int num_keys = 24;

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

// These are the Teensy pins wired up for the OLED.
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13

// Initiate the correct kind of display object based on OLED type
#ifdef WHITE_OLED
  Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif
#ifdef BLUE_OLED
  Adafruit_SH1106 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif

// These are found in the digigurdy-baz repository
#include "bitmaps.h"

// Right now not using the std namespace is just impacting strings.  That's ok...
using namespace MIDI_NAMESPACE;

// #################
// CLASS DEFINITIONS
// #################

// Button objects abstract the physical buttons.
//   This class is for generic buttons like the octave/tpose ones, which are
//   press-on, release-off but otherwise generic in purpose.
class GurdyButton {
  protected:
    Bounce* bounce_obj;
    bool being_pressed;
  public:

    // my_pin corresponds to the physical Teensy pin.
    GurdyButton(int my_pin) {

      // The 5 is 5ms wait for events to complete.
      // Recommended from the Bounce webpage for "good" buttons.
      bounce_obj = new Bounce(my_pin, 5);

      // In John's code the non-keybox buttons weren't using Bounce.
      // I'm taking a guess that INPUT_PULLUP is what I want to use here but I think so.
      // Originally it was using DirectRead/Write and INPUT.
      pinMode(my_pin, INPUT_PULLUP);

      being_pressed = false;
    };

    // Bounce objects should only be update()-ed once per loop(),
    // so I'm not putting it in wasPressed()/wasReleased().
    void update() {

      bounce_obj->update();

      // If button was pressed or released this cycle, record that.
      if (bounce_obj->fallingEdge()) {
        being_pressed = true;
      } else if (bounce_obj->risingEdge()) {
        being_pressed = false;
      };
    };

    // This returns true if the button is being pressed.
    bool beingPressed() {
      return being_pressed;
    };

    // This returns true ONLY if the button was pressed this particular loop cycle.
    bool wasPressed() {
      return bounce_obj->fallingEdge();
    };

    // This returns true ONLY if the button was released this particular loop cycle.
    bool wasReleased() {
      return bounce_obj->risingEdge();
    };
};

// class ToggleButton adds a state toggle to the GurdyButton class
//   This class is meant for buttons where:
//   * Pressing and releasing once activates it.
//   * Pressing and releasing again deactivates it.
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

      // We'll only look at the downpress to register the toggle.
      if(bounce_obj->fallingEdge()) {
        toggled = !toggled;
      };
    };

    bool toggleOn() {
      return toggled;
    };
};

// class KeyboxButton adds a note offset variable to the GurdyButton class.
//   This class is meant for use with the keybox keys, where:
//   * Button type is press-on, release-off
//   * Button has an offset that it raises notes to on a string.
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
    };

};

// GurdyString manages turning "strings" on and off and determining their note.
// It abstracts the interactions with the MIDI layer.
class GurdyString {
  private:
    int open_note;          // This string's base note
    int midi_channel;       // This string's MIDI channel (1-8)
    int midi_volume;        // 0-127, I'm using 56 everywhere right now
    int note_being_played;  // The note being sounded (base note + key offset)
                            // This is necessary to turn off notes before turning on new ones.

    // This is a pointer to the MIDI object that powers the phsyical MIDI ports on the gurdy.
    // usbMIDI shows up with Arduino library magic somehow and we don't need this for it.
    MidiInterface<SerialMIDI<HardwareSerial>> *MIDI_obj;

  public:
    GurdyString(int my_channel, int my_note, midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *my_MIDI_obj, int my_vol = 56) {
      midi_channel = my_channel;
      open_note = my_note;
      midi_volume = my_vol;
      note_being_played = open_note;
      MIDI_obj = my_MIDI_obj;
    };

    // soundOn() sends sound on this string's channel at its notes
    // optionally with an additional offset (e.g. a key being pressed)
    //
    // I DO NOT KNOW why I can just access usbMIDI here, but even when
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

    int getOpenNote() {
      return open_note;
    };

    void setOpenNote(int new_note) {
      open_note = new_note;
    };
};

// BuzzKnob manages the potentiometer knob that adjusts the buzzing threshold.
// The GurdyCrank class below uses it to determine when buzzing happens.
class BuzzKnob {
  private:
    int voltage_pin;
    // loop() runs about 20,000x per sec, so this is checking for knob changes about every 1.5s.
    static const int poll_interval = 30000;
    int poll_counter;
    int knob_voltage;

  public:
    BuzzKnob(int v_pin) {
      voltage_pin = v_pin;
      pinMode(voltage_pin, INPUT);
      poll_counter = 0;
      knob_voltage = 0;
    };

    // This should be run every loop() during play.
    // Reads the knob voltage every poll_interval cycles.
    void update() {
      poll_counter += 1;
      if (poll_counter == poll_interval) {
        poll_counter = 0;
        knob_voltage = analogRead(voltage_pin);
      };
    };

    // Returns an adjusted voltage value suitible
    // for comparing with the crank's.
    float getVoltage() {
      return (float)(knob_voltage / 3);
    };
};

// class GurdyCrank controls the cranking mechanism, including the buzz triggers.
class GurdyCrank {
  private:
    int voltage_pin;
    static const int num_samples = 500;  // This number is from the original code
    int samples[num_samples];
    int sample_sum;
    float sample_mean;
    float squared_sum;
    float deviations;

    static const int crank_interval = 100;  // I.e. we check every 100 loop()s
    int crank_counter;
    int crank_voltage;

    // The original code uses a technique where a counter is incremented by a
    // large amount up to a max amount when voltage is detected.  When voltage
    // isn't detected, the counter decays more slowly (about a 5th as quickly).
    // This "smooths" over momentary fluctuations in the analog voltage from
    // the crank.  I'm using the same approach here.  To keep things straight,
    // I'm calling this smoothed value the "spin" of the crank.
    static const int max_spin = 3000;
    static const int spin_weight = 500;
    static const int spin_threshold = 50; // spin over 50 makes sound.
    int spin;
    bool started_spinning;
    bool stopped_spinning;
    bool is_spinning;
    static const int v_threshold = 25;

    BuzzKnob* myKnob;
    bool started_buzzing;
    bool stopped_buzzing;
    bool is_buzzing;

  public:

    // v_pin is the voltage pin of the crank.  This is A1 on a normal didigurdy.
    GurdyCrank(int v_pin, int buzz_pin) {

      myKnob = new BuzzKnob(buzz_pin);

      voltage_pin = v_pin;
      pinMode(voltage_pin, INPUT);
      crank_counter = 0;
      crank_voltage = 0;
      spin = 0;
      started_spinning = false;
      stopped_spinning = false;
      is_spinning = false;

      started_buzzing = false;
      stopped_buzzing = false;
      is_buzzing = false;
    };

    // Crank detection - this comes from John's code.  We sample the voltage
    // of the crank's voltage pin 500 times really quick (100/s for 5s).
    // With that, we calculate the standard deviation of the results.
    //
    // My understanding is this: if the motor is connected and at rest, it
    // will give a consistent very-low voltage.  If the pin is not connected
    // to anything, its voltage will wander around.  If the results are less than
    // 10 stDev from the mean, we consdier it detected.
    //
    // Moving the crank during the detection *does* throw it off.  Don't do that.
    void detect() {

      sample_sum = 0;
      sample_mean = 0;
      squared_sum = 0;
      deviations = 0;

      // Serial.println("Detecting...");

      // Read the crank 500 times real quick.
      for (int i = 0; i < num_samples; i++) {
        samples[i] = analogRead(voltage_pin);
        sample_sum += samples[i];

        delay(10);  // Why? Because John did and it worked.
      };

      // Get the average voltage
      sample_mean = sample_sum / float(num_samples);

      // We need the sum of the square of the difference of each value now.
      for (int i = 0; i < num_samples; i++) {
        squared_sum += pow((sample_mean - float(samples[i])), 2);
      };

      // The square root of the average of *that* is the stardard devitaion.
      deviations = sqrt(squared_sum / float(num_samples));
    };

    bool isDetected() {
      // Serial.print("deviations: ");
      // Serial.println(deviations);
      return (deviations < 10);
    };

    void refreshBuzz() {
      if (isDetected()) {
        // Buzzing happens if the crank generates more voltage than the
        // adjusted voltage from the knob.
        if (crank_voltage > myKnob->getVoltage()) {

          // If we weren't buzzing before this, we just started.
          if (!is_buzzing) {
            started_buzzing = true;
          };

          // If we were already buzzing and started_buzzing last cycle,
          // we didn't just start buzzing anymore.
          if (started_buzzing && is_buzzing) {
            started_buzzing = false;
          };

          // Now that we checked, we can update this...
          is_buzzing = true;

        } else {

          // If we were buzzing before, we just stopped.
          if (is_buzzing) {
            stopped_buzzing = true;
          };

          // If we stopped buzzing last cycle, we didn't just stop
          // anymore.
          if (stopped_buzzing && !is_buzzing) {
            stopped_buzzing = false;
          };

          // Now that we're done checking if we *were* buzzing, we can set this.
          is_buzzing = false;
        };

      // If the crank isn't *connected*, the pin will report phantom buzzing,
      // so if the crank isn't *detected*, don't buzz at all:
      } else {
        started_buzzing = false;
        is_buzzing = false;
        stopped_buzzing = false;
      };
    };

    // This is meant to be run every loop().
    void update() {
      if (isDetected()) {
        // Update the knob first.
        myKnob->update();
        refreshBuzz();

        // Every 100 loops() (I haven't measured this, but this is several times
        // per second), we update the crank_voltage.
        crank_counter += 1;
        if (crank_counter == crank_interval) {
          crank_counter = 0;
          crank_voltage = analogRead(voltage_pin);
        };

        // Based on that voltage, we either bump up the spin by the spin_weight,
        // or we let it decay based on the voltage.
        if (crank_voltage > v_threshold) {
          spin += spin_weight;
          if (spin > max_spin) {
            spin = max_spin;
          };
        } else {
          spin -= 1;
          if (spin < 0) {
            spin = 0;
          };
        };

        // The crank is considered spinning if the spin is over spin_threshold.
        if (spin > spin_threshold) {

          // If we weren't spinning before this, we just started.
          if (!is_spinning) {
            started_spinning = true;
          };

          // If we were already spinning and started_spinning last cycle,
          // we didn't just start spinning anymore.
          if (started_spinning && is_spinning) {
            started_spinning = false;
          };

          // Now that we checked, we can update this...
          is_spinning = true;

        } else {

          // If we were spinning before, we just stopped.
          if (is_spinning) {
            stopped_spinning = true;
          };

          // If we stopped spinning last cycle, we didn't just stop
          // anymore.
          if (stopped_spinning && !is_spinning) {
            stopped_spinning = false;
          };

          is_spinning = false;
        };

      // If the crank wasn't detected, it acts like a crank that never gets spun.
      } else {
        is_spinning = false;
        started_spinning = false;
        stopped_spinning = false;
      };
    };

    bool startedSpinning() {
      return started_spinning;
    };

    bool stoppedSpinning() {
      return stopped_spinning;
    };

    bool isSpinning() {
      return is_spinning;
    };

    bool startedBuzzing() {
      return started_buzzing;
    };

    bool stoppedBuzzing() {
      return stopped_buzzing;
    };
};

// class HurdyGurdy is basically a virtual keybox for buttons that control
// notes.  It manages updating and detecting the button actions and determines
// ultimately which note the "keybox" is producing.
class HurdyGurdy {
  private:
    int keybox_size;         // How many keys are in the keybox
    int max_offset;          // The currest highest key being pressed
    int prev_offset;         // The highest key from last loop() cycle
    bool higher_key_pressed;
    bool lower_key_pressed;

  public:
    KeyboxButton* keybox[num_keys];
    HurdyGurdy(int pin_arr[], int key_size) {
      keybox_size = key_size;
      max_offset = 0;

      // Run through the array from the top of this file and create all the keyboxbutton
      // objects
      for(int x = 1; x < key_size + 1; x++) {
        keybox[x-1] = new KeyboxButton(pin_arr[x], x);
      };
    };

    // This method both updates all the keys, and returns the highest offset/notes
    // being pressed this cycle.
    int getMaxOffset() {

      higher_key_pressed = false;
      lower_key_pressed = false;

      // Save the last highest key
      prev_offset = max_offset;
      max_offset = 0;

      // Look at each key in order
      for(int x = 0; x < keybox_size; x++) {
        // Update the key
        keybox[x]->update();

        // if the key is being pressed, record that key's offset.  Array is in order,
        // so at the end we should have the max_offset.
        if (keybox[x]->beingPressed()) {
          max_offset = keybox[x]->getOffset();
        };
      };

      if (max_offset > prev_offset) {
        higher_key_pressed = true;
      };

      if (max_offset < prev_offset) {
        lower_key_pressed = true;
      };

      return max_offset;
    };

    bool higherKeyPressed() {
      return higher_key_pressed;
    };

    bool lowerKeyPressed() {
      return lower_key_pressed;
    };

};

// enum Note maps absolute note names to MIDI note numbers (middle C4 = 60),
// which range from 0 to 127.
//
// This lets us specify MIDI notes by their name instead of having to refer to a table.
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
//
// This lets us recall string names for printing on the screen without having to refer to a table.
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

// This is a version of the above but with flats listed as well.
std::string LongNoteNum[] = {
  "  C-1  ", "C-1#D-1b", "  D-1  ", "D-1#/E-1b", "  E-1  ", "  F-1  ", "F-1#/G-1b", "  G-1  ", "G-1#/A-1b", "  A-1  ", "A-1#/B-1b", "  B-1  ",
  "  C0   ", "C0#/D0b", "  D0   ", "D0#/E0b", "  E0   ", "  F0   ", "F0#/G0b", "  G0   ", "G0#/A0b", "  A0   ", "A0#/B0b", "  B0   ",
  "  C1   ", "C1#/D1b", "  D1   ", "D1#/E1b", "  E1   ", "  F1   ", "F1#/G1b", "  G1   ", "G1#/A1b", "  A1   ", "A1#/B1b", "  B1   ",
  "  C2   ", "C2#/D2b", "  D2   ", "D2#/E2b", "  E2   ", "  F2   ", "F2#/G2b", "  G2   ", "G2#/A2b", "  A2   ", "A2#/B2b", "  B2   ",
  "  C3   ", "C3#/D3b", "  D3   ", "D3#/E3b", "  E3   ", "  F3   ", "F3#/G3b", "  G3   ", "G3#/A3b", "  A3   ", "A3#/B3b", "  B3   ",
  "  C4   ", "C4#/D4b", "  D4   ", "D4#/E4b", "  E4   ", "  F4   ", "F4#/G4b", "  G4   ", "G4#/A4b", "  A4   ", "A4#/B4b", "  B4   ",
  "  C5   ", "C5#/D5b", "  D5   ", "D5#/E5b", "  E5   ", "  F5   ", "F5#/G5b", "  G5   ", "G5#/A5b", "  A5   ", "A5#/B5b", "  B5   ",
  "  C6   ", "C6#/D6b", "  D6   ", "D6#/E6b", "  E6   ", "  F6   ", "F6#/G6b", "  G6   ", "G6#/A6b", "  A6   ", "A6#/B6b", "  B6   ",
  "  C7   ", "C7#/D7b", "  D7   ", "D7#/E7b", "  E7   ", "  F7   ", "F7#/G7b", "  G7   ", "G7#/A7b", "  A7   ", "A7#/B7b", "  B7   ",
  "  C8   ", "C8#/D8b", "  D8   ", "D8#/E8b", "  E8   ", "  F8   ", "F8#/G8b", "  G8   ", "G8#/A8b", "  A8   ", "A8#/B8b", "  B8   ",
  "  C9   ", "C9#/D9b", "  D9   ", "D9#/E9b", "  E9   ", "  F9   ", "F9#/G9b", "  G9   "
};

void printDisplay(int mel1, int mel2, int drone, int tromp, int tpose, int cap, int offset) {

  // This whole thing could be written more clearly...

  std::string disp_str0 = "";
  std::string disp_str = "";
  std::string disp_str2 = "";

  disp_str0 = " Tpose: ";
  disp_str = "\n"
             "  Hi Melody: " + LongNoteNum[mel1 + tpose] + "\n"
             " Low Melody: " + LongNoteNum[mel2 + tpose] + "\n"
             "      Drone: " + LongNoteNum[drone + tpose + cap] + "\n"
             "  Trompette: " + LongNoteNum[tromp + tpose + cap] + "\n\n";
  disp_str2 = "  " + LongNoteNum[mel1 + tpose + offset] + "\n";

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print(disp_str0.c_str());

  // This is because the version of gcc Teensy uses has a bug with std::to_string(int)...
  // I need to print the tpose separately here because the display object will aceept it if it's not
  // concatenated with a string.
  if (tpose > 0) { display.print("+"); };
  display.print(tpose);

  display.print("  Capo: ");

  if (cap > 0) { display.print("+"); };
  display.print(cap);

  display.print(disp_str.c_str());
  display.setTextSize(2);
  display.print(disp_str2.c_str());
  display.display();
};

// ##################
// GLOBAL SETUP STUFF
// ##################

// Create the serial interface object
SerialMIDI<HardwareSerial> mySerialMIDI(Serial1);
// Create a new MidiInterface object using that serial interface
MidiInterface<SerialMIDI<HardwareSerial>> *myMIDI;

// Declare the "keybox" and buttons.
HurdyGurdy *mygurdy;
ToggleButton *bigbutton;
GurdyCrank *mycrank;

KeyboxButton *myOkButton;
KeyboxButton *myBackButton;
KeyboxButton *my1Button;
KeyboxButton *my2Button;
KeyboxButton *my3Button;
KeyboxButton *my4Button;
KeyboxButton *my5Button;
KeyboxButton *my6Button;

// Note that there aren't special classes for melody, drone, even the keyclick.
// They are differentiated in the main loop():
// * A melody string is one that changes with the keybox offset.
// * A drone/trompette is one that doesn't change.
// * The keyclick "string" is just a drone that comes on and off at particular times.
// * The buzz "string" is also just a drone that comes on/off at other particular times.
GurdyString *mystring;
GurdyString *mylowstring;
GurdyString *mykeyclick;
GurdyString *mytromp;
GurdyString *mydrone;
GurdyString *mybuzz;

GurdyButton *tpose_up;
GurdyButton *tpose_down;

// This defines the +/- one octave transpose range.
const int max_tpose = 12;
int tpose_offset;

GurdyButton *capo;

// This defines the 0, +2, +4 capo range.
const int max_capo = 4;
int capo_offset;

// The offset is given when we update the buttons each cycle.
// Buttons should only be updated once per cycle.  So we need this in the main loop()
// to refer to it several times.
int myoffset;

// true = G/C tuning, false = D/G.  For the menus.
bool gc_or_dg;

// Teensy and Arduino units start by running setup() once after powering up.
// Here we establish how the "gurdy" is setup, what strings to use, and we also
// start the MIDI communication.
void setup() {

  #ifdef WHITE_OLED
    display.begin(SSD1306_SWITCHCAPVCC);
  #endif
  #ifdef BLUE_OLED
    display.begin(SH1106_SWITCHCAPVCC);
  #endif
  // The Adafruit default logo.  Let it ride a sec or two.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // Intro animated sequence
  // display.clearDisplay();
  display.drawBitmap(0, 0, logo47_glcd_bmp, 128, 64, 1);
  display.display();
  delay(1500);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo48_glcd_bmp, 128, 64, 1);
  display.display();
  delay(150);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo49_glcd_bmp, 128, 64, 1);
  display.display();
  delay(150);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo50_glcd_bmp, 128, 64, 1);
  display.display();
  delay(150);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo51_glcd_bmp, 128, 64, 1);
  display.display();
  delay(150);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo52_glcd_bmp, 128, 64, 1);
  display.display();
  delay(150);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo53_glcd_bmp, 128, 64, 1);
  display.display();
  delay(150);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo54_glcd_bmp, 128, 64, 1);
  display.display();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("DigiGurdy");
  display.setTextSize(1):
  display.println("By Basil Lalli,"):
  display.println("Concept By J. Dingley");
  display.println("5 Mar 2022, Ver. 0.8 ");
  display.println("                     ");
  display.println("  shorturl.at/tuDY1  ");
  display.display();
  delay(3000);

  // // Un-comment to print yourself debugging messages to the Teensyduino
  // // serial console.
  // Serial.begin(38400);
  // delay(3000);
  // Serial.println("Hello.");

  myMIDI = new MidiInterface<SerialMIDI<HardwareSerial>>((SerialMIDI<HardwareSerial>&)mySerialMIDI);
  myMIDI->begin();

  mycrank = new GurdyCrank(A1, A2);
  mycrank->detect();

  // The keybox arrangement is decided by pin_array, which is up in the CONFIG SECTION
  // of this file.  Make adjustments there.
  mygurdy = new HurdyGurdy(pin_array, num_keys);
  bigbutton = new ToggleButton(39);

  // Grab the keys we use for menu nav out of the keybox:
  myBackButton = mygurdy->keybox[0];
  myOkButton = mygurdy->keybox[num_keys - 2];

  // 1-6 keys are the first six bottom keys of the keybox
  my1Button = mygurdy->keybox[1];
  my2Button = mygurdy->keybox[3];
  my3Button = mygurdy->keybox[4];
  my4Button = mygurdy->keybox[6];
  my5Button = mygurdy->keybox[8];
  my6Button = mygurdy->keybox[9];

  // Which channel is which doesn't really matter, but I'm sticking with
  // John's channels so his videos on setting it up with a tablet/phone still work.
  mystring = new GurdyString(1, Note(g4), myMIDI);
  mylowstring = new GurdyString(2, Note(g3), myMIDI);
  mytromp = new GurdyString(3, Note(c3), myMIDI);
  mydrone = new GurdyString(4, Note(c2), myMIDI);
  mybuzz = new GurdyString(5,Note(c3), myMIDI);
  mykeyclick = new GurdyString(6, Note(b5), myMIDI);


  tpose_up = new GurdyButton(21);   // A.k.a. the button formerly known as octave-up
  tpose_down = new GurdyButton(22); // A.k.a. the button formerly known as octave-down
  tpose_offset = 0;

  capo = new GurdyButton(23); // The capo button
  capo_offset = 0;

  printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, 0);
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

  std::string disp_str = "         ";
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
  "  'O') Default (**)  \n"
  "                     \n";

  display.print(disp_str.c_str());
  display.display();

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myOkButton->update();

    if (my1Button->wasPressed() || myOkButton->wasPressed()) {
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

  std::string disp_str = " High Melody:   " + NoteNum[mystring->getOpenNote()] + "   \n"
  " Choose Low Melody:  \n"
  "                     \n"
  " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
  "                     \n"
  "  'O') Default (**)  \n"
  "                     \n";

  display.print(disp_str.c_str());
  display.display();

  bool done = false;

  while (!done) {

    my1Button->update();
    my2Button->update();
    my3Button->update();
    my4Button->update();
    myOkButton->update();

    if (my1Button->wasPressed() || myOkButton->wasPressed()) {
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
  } else {
    base_note = Note(d4);
  };

  choice1 = base_note - 31;
  choice2 = base_note - 24;
  choice3 = base_note - 19;
  choice4 = base_note - 12;
  choice5 = base_note - 7;
  choice6 = base_note;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  std::string disp_str = " Hi/Lo Melody: " + NoteNum[mystring->getOpenNote()] + "/" + NoteNum[mylowstring->getOpenNote()] + " \n"
  "    Choose Drone:    \n"
  " 1) " + NoteNum[choice1] + "     2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + " **  4) " + NoteNum[choice4] + "     \n"
  " 5) " + NoteNum[choice5] + "     6) " + NoteNum[choice6] + "     \n"
  "                     \n"
  "  'O') Default (**)  \n"
  "                     \n";

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
    myOkButton->update();

    if (my1Button->wasPressed()) {
      mydrone->setOpenNote(choice1);
      done = true;
    } else if (my2Button->wasPressed()) {
      mydrone->setOpenNote(choice2);
      done = true;
    } else if (my3Button->wasPressed() || myOkButton->wasPressed()) {
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

  std::string disp_str = " Hi/Lo Melody: " + NoteNum[mystring->getOpenNote()] + "/" + NoteNum[mylowstring->getOpenNote()] + " \n"
  " Drone: " + NoteNum[mydrone->getOpenNote()] + "           \n"
  "  Choose Trompette:  \n"
  " 1) " + NoteNum[choice1] + " **  2) " + NoteNum[choice2] + "     \n"
  " 3) " + NoteNum[choice3] + "     4) " + NoteNum[choice4] + "     \n"
  " 5) " + NoteNum[choice5] + "     6) " + NoteNum[choice6] + "     \n"
  "                     \n"
  "                     \n"
  "  'O') Default (**)  \n";

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
    myOkButton->update();

    if (my1Button->wasPressed() || myOkButton->wasPressed()) {
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
  };
    };

void tuning() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  std::string disp_str = "\n"
  "     TUNING MENU     \n"
  "                     \n"
  " Choose Base Tuning: \n"
  "                     \n"
  "       1) G/C        \n"
  "                     \n"
  "       2) D/G        \n"
  "                     \n";

  display.print(disp_str.c_str());
  display.display();

  // Give the user a chance to figure out it's happening...
  delay(1500);

  bool done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    my1Button->update();
    my2Button->update();

    if (my1Button->wasPressed()) {
      gc_or_dg = true;
      done = true;

    } else if (my2Button->wasPressed()) {
      gc_or_dg = false;
      done = true;
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
  disp_str = ""
  "      SUMMARY:       \n"
  "  High Melody:   " + NoteNum[mystring->getOpenNote()] + "  \n"
  "   Low Melody:   " + NoteNum[mylowstring->getOpenNote()] + "  \n"
  "        Drone:   " + NoteNum[mydrone->getOpenNote()] + "  \n"
  "    Trompette:   " + NoteNum[mytromp->getOpenNote()] + "  \n"
  "                     \n"
  "  'O') Save & Go     \n"
  "  'X') Start Over    \n";

  display.print(disp_str.c_str());
  display.display();

  done = false;
  while (!done) {

    // Check the 1 and 2 buttons
    myOkButton->update();
    myBackButton->update();

    if (myOkButton->wasPressed()) {
      done = true;

    } else if (myBackButton->wasPressed()) {
      tuning();
      done = true;
    };
  };

};

bool first_loop = true;

// The loop() function is repeatedly run by the Teensy unit after setup() completes.
// This is the main logic of the program and defines how the strings, keys, click, buzz,
// and buttons acutally behave during play.
void loop() {

  if (first_loop) {
    tuning();
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, 0);
    first_loop = false;
  };

  // Update the keys, buttons, and crank status (which includes the buzz knob)
  myoffset = mygurdy->getMaxOffset();  // This covers the keybox buttons.
  bigbutton->update();
  mycrank->update();
  tpose_up->update();
  tpose_down->update();
  capo->update();

  myOkButton->update();
  myBackButton->update();

  // If the "X" and "O" buttons are both down, trigger the tuning menu
  if (myOkButton->beingPressed() && myBackButton->beingPressed()) {
    tuning();
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, 0);
  };

  // Check for a capo shift.
  if (capo->wasPressed()) {

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

      mystring->soundOn(myoffset + tpose_offset);
      mylowstring->soundOn(myoffset + tpose_offset);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
    };
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);
  };

  // As long as we're in playing mode--acutally playing or not--
  // check for a tpose shift.
  if (tpose_up->wasPressed() && (tpose_offset < max_tpose)) {
    tpose_offset += 1;

    if (mycrank->isSpinning() || bigbutton->toggleOn()) {
      mytromp->soundOff();
      mydrone->soundOff();
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset);
      mylowstring->soundOn(myoffset + tpose_offset);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
    };
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);
  };
  if (tpose_down->wasPressed() && (max_tpose + tpose_offset > 0)) {
    tpose_offset -= 1;

    if (mycrank->isSpinning() || bigbutton->toggleOn()) {
      mytromp->soundOff();
      mydrone->soundOff();
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset);
      mylowstring->soundOn(myoffset + tpose_offset);
      mykeyclick->soundOn(tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
    };
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);
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
      mystring->soundOn(myoffset + tpose_offset);
      mylowstring->soundOn(myoffset + tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
      printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);

    } else if (mycrank->startedSpinning() && !bigbutton->toggleOn()) {
      mystring->soundOn(myoffset + tpose_offset);
      mylowstring->soundOn(myoffset + tpose_offset);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
      printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);

    // Turn off the previous notes and turn on the new one with a click if new key this cycle.
    // NOTE: I'm not touching the drone/trompette.  Just leave it on if it's a key change.
    } else if (mygurdy->higherKeyPressed() || mygurdy->lowerKeyPressed()) {
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset);
      mylowstring->soundOn(myoffset + tpose_offset);
      mykeyclick->soundOn(tpose_offset);
      printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);
    };

    // Whenever we're playing, check for buzz.
    if (mycrank->startedBuzzing()) {
      mybuzz->soundOn(tpose_offset);
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
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);

  // If the crank stops and the toggle was off, turn off sound.
  } else if (mycrank->stoppedSpinning() && !bigbutton->toggleOn()) {
    mystring->soundOff();
    mylowstring->soundOff();
    mykeyclick->soundOff();
    mytromp->soundOff();
    mydrone->soundOff();
    mybuzz->soundOff();
    printDisplay(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset);
  };

  // Apparently we need to do this to discard incoming data.
  while (myMIDI->read()) {
  }
  while (usbMIDI.read()) {
  }
};
