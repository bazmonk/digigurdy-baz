// Digigurdy-Baz

// AUTHOR: Basil Lalli
// DESCRIPTION: Digigurdy-Baz is a fork of the Digigurdy code by John Dingley.  See his page:
//   https://digigurdy.com/
//   https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy
// REPOSITORY: https://github.com/bazmonk/digigurdy-baz

// These are found in the digigurdy-baz repository
#include "notes.h"           // Note and note string variables
#include "bitmaps.h"         // Pretty pictures
#include "eeprom_values.h"   // Save-slot memory addresses
#include "config.h"          // Configuration variables

#include "exbutton.h"
#include "common.h"
#include "usb_power.h"

#ifdef USE_GEARED_CRANK
  #include "gearcrank.h"
#else
  #include "gurdycrank.h"
#endif

#include "hurdygurdy.h"
#include "vibknob.h"

// These are all about the display
#include "display.h"         // Intializes our display object
#include "startup_screens.h" // Startup-related screens.
#include "play_screens.h"    // The screens mid-play (notes and staffs)
#include "pause_screens.h"   // The pause screen menus

// As far as I can tell, this *has* to be done here or else you get spooooky runtime problems.
MIDI_CREATE_DEFAULT_INSTANCE();

#ifdef USE_TRIGGER
  wavTrigger  trigger_obj;
#endif

#ifdef USE_TSUNAMI
  Tsunami trigger_obj;
#endif

//
// GLOBAL OBJECTS/VARIABLES
//

// This is for the crank, the audio-to-digital chip
ADC* adc;

// Declare the "keybox" and buttons.
HurdyGurdy *mygurdy;
ExButton *bigButton;

#ifdef USE_GEARED_CRANK
  GearCrank *mycrank;
#else
  GurdyCrank *mycrank;
  volatile int num_events = 0;
  volatile int last_event = 0;
  elapsedMicros last_event_timer;
  elapsedMicros debounce_timer;
#endif

VibKnob *myvibknob;

// As musical keys, these are referred to in the mygurdy object above.
// This declaration of them is specifically for their use as navigational
// buttons in the menu screens.  ok = O, back = X.
KeyboxButton *myAButton;
KeyboxButton *myXButton;
KeyboxButton *my1Button;
KeyboxButton *my2Button;
KeyboxButton *my3Button;
KeyboxButton *my4Button;
KeyboxButton *my5Button;
KeyboxButton *my6Button;

// For legacy button-combo support:
KeyboxButton *myAltTposeButton;
KeyboxButton *myAltTposeUp;
KeyboxButton *myAltTposeDown;
KeyboxButton *myBButton;

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

// These are the "extra" buttons, new on the rev3.0 gurdies
ExButton *ex1Button;
ExButton *ex2Button;
ExButton *ex3Button;
ExButton *ex4Button;
ExButton *ex5Button;
ExButton *ex6Button;

#ifdef REV4_MODE

ExButton *ex7Button;
ExButton *ex8Button;
ExButton *ex9Button;
ExButton *ex10Button;
#endif

// This defines the +/- one octave transpose range.
int max_tpose;
int tpose_offset;

// This defines the 0, +2, +4 capo range.
int max_capo;
int capo_offset;

// The offset is given when we update the buttons each cycle.
// Buttons should only be updated once per cycle.  So we need this in the main loop()
// to refer to it several times.
int myoffset;

// this tracks the drone/trompette mute status.  Starts all-on.
// 0 = all on
// 1 = all off
// 2 = drone on, tromp off
// 3 = drone off, tromp on
int drone_mode;

// similarly to drone_mode...
// 0 = all on
// 1 = high on, low off
// 2 = high off, low on
int mel_mode;

int d_mode;
int t_mode;

int mel_vibrato;

int play_screen_type;
uint8_t scene_signal_type;

bool gc_or_dg;

int use_solfege;

bool autocrank_toggle_on = false;

/// @brief The main setup function.
/// @details Arduinio/Teensy sketches run this function once upon startup.
/// * Initializes display, runs startup animation.
/// * Initializes MIDI/Tsunami/Serial objects.
/// * Initializes gurdy button/string/crank/knob objects.
/// * Pin assignments which no one tends to change around (crank, bigButton) are hardcoded here.
/// * The MIDI channel assignments of the strings are hardcoded here.
void setup() {

  // Display some startup animations for the user.
  start_display();
  startup_screen_sequence();

  // Un-comment to print yourself debugging messages to the Teensyduino
  // serial console.
   Serial.begin(115200);
   delay(300);
   Serial.println("Hello.");

  // Start the Serial MIDI object (like for a bluetooth transmitter).
  // The usbMIDI object is available by Teensyduino magic that I don't know about.
  if (EEPROM.read(EEPROM_SEC_OUT) != 1) {
    MIDI.begin(MIDI_CHANNEL_OMNI);
  }
  
  #if defined(USE_TRIGGER)
    #ifdef BAZ_MODE
    Serial5.setTX(47); // I can just do this for everyone until it bugs someone...
    Serial5.setRX(46);
    #endif
    if (EEPROM.read(EEPROM_SEC_OUT) > 0) {
      trigger_obj.start();
      delay(10);

      // Send a stop-all command and reset the sample-rate offset, in case we have
      //  reset while the WAV Trigger was already playing.
      trigger_obj.stopAllTracks();
      trigger_obj.samplerateOffset(0);
    };

  #elif defined(USE_TSUNAMI)
    if (EEPROM.read(EEPROM_SEC_OUT) > 0) {
      trigger_obj.start();
      delay(10);

      // Send a stop-all command and reset the sample-rate offset, in case we have
      //  reset while the WAV Trigger was already playing.
      trigger_obj.stopAllTracks();
      trigger_obj.samplerateOffset(1, 0);
    };
  #endif

  // Initialize the ADC object and the crank that will use it.
  adc = new ADC();

  #ifdef USE_GEARED_CRANK
    mycrank = new GearCrank(CRANK_PIN, BUZZ_PIN);
    mycrank->beginPolling();
    
    print_message_2("Crank Detection", "Crank is detecting,", "Please wait...");
    mycrank->detect();
    if (mycrank->isDetected()) {
      print_message_2("Crank Detection", "Crank is detecting,", "CRANK DETECTED!");
      delay(1000);
    } else {
      print_message_2("Crank Detection", "Crank is detecting,", "CRANK NOT FOUND.");
      delay(1000);
    };

  #else
    #ifdef USE_ENCODER
      mycrank = new GurdyCrank(CRANK_PIN, CRANK_PIN2, BUZZ_PIN, LED_PIN);
    #else
      mycrank = new GurdyCrank(CRANK_PIN, BUZZ_PIN, LED_PIN);
    #endif
  #endif

  #ifdef USE_PEDAL
    myvibknob = new VibKnob(PEDAL_PIN);
  #endif

  // The keybox arrangement is decided by pin_array, which is up in the CONFIG SECTION
  // of this file.  Make adjustments there.
  mygurdy = new HurdyGurdy(pin_array, num_keys);

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

  // Turn on the power if we're using it.
  #ifdef USB_ALWAYS_ON
  usb_power_on();
  delay(100);
  #else
  if (EEPROM.read(EEPROM_SEC_OUT) > 0) {
    usb_power_on();
    delay(100);
  };
  #endif
  
  mystring = new GurdyString(1, Note(g4), "Hi Melody", EEPROM.read(EEPROM_SEC_OUT));
  mylowstring = new GurdyString(2, Note(g3), "Low Melody", EEPROM.read(EEPROM_SEC_OUT));
  mytromp = new GurdyString(3, Note(c3), "Trompette", EEPROM.read(EEPROM_SEC_OUT));
  mydrone = new GurdyString(4, Note(c2), "Drone", EEPROM.read(EEPROM_SEC_OUT));
  mybuzz = new GurdyString(5,Note(c3), "Buzz", EEPROM.read(EEPROM_SEC_OUT));
  mykeyclick = new GurdyString(6, Note(b5), "Key Click", EEPROM.read(EEPROM_SEC_OUT));

  if (EEPROM.read(EEPROM_SEC_OUT) > 0) {
    mystring->setTrackLoops();
    mylowstring->setTrackLoops();
    mytromp->setTrackLoops();
    mydrone->setTrackLoops();
    mybuzz->setTrackLoops();
    mykeyclick->setTrackLoops();
  };
 
  tpose_offset = 0;
  capo_offset = 0;

  ex1Button = new ExButton(EX1_PIN, 200, EEPROM_EX1, EEPROM_EX1_TSTEP);
  ex2Button = new ExButton(EX2_PIN, 200, EEPROM_EX2, EEPROM_EX2_TSTEP);
  ex3Button = new ExButton(EX3_PIN, 200, EEPROM_EX3, EEPROM_EX3_TSTEP);
  ex4Button = new ExButton(EX4_PIN, 200, EEPROM_EX4, EEPROM_EX4_TSTEP);
  ex5Button = new ExButton(EX5_PIN, 200, EEPROM_EX5, EEPROM_EX5_TSTEP);
  ex6Button = new ExButton(EX6_PIN, 200, EEPROM_EX6, EEPROM_EX6_TSTEP);

  #ifdef REV4_MODE
  ex7Button = new ExButton(EX7_PIN, 200, EEPROM_EX7, EEPROM_EX7_TSTEP);
  ex8Button = new ExButton(EX8_PIN, 200, EEPROM_EX8, EEPROM_EX8_TSTEP);
  ex9Button = new ExButton(EX9_PIN, 200, EEPROM_EX9, EEPROM_EX9_TSTEP);
  ex10Button = new ExButton(EX10_PIN, 200, EEPROM_EX10, EEPROM_EX10_TSTEP);
  #endif

  bigButton = new ExButton(BIG_BUTTON_PIN, 250, EEPROM_EXBB, EEPROM_EXBB_TSTEP);

  scene_signal_type = EEPROM.read(EEPROM_SCENE_SIGNALLING);

  // Default to all strings un-muted
  drone_mode = 0;
  mel_mode = 0;

  d_mode = 0;
  t_mode = 0;

  play_screen_type = 0;
  scene_signal_type = 0;

  max_tpose = 12;
  max_capo = 4;

  use_solfege = EEPROM.read(EEPROM_USE_SOLFEGE);

  mel_vibrato = EEPROM.read(EEPROM_MEL_VIBRATO);
};

//
// MAIN LOOP
//

bool first_loop = true;

// This is for dev stuff when it breaks.
int test_count = 0;
int start_time = millis();

int stopped_playing_time = 0;
bool note_display_off = true;

// The loop() function is repeatedly run by the Teensy unit after setup() completes.
// This is the main logic of the program and defines how the strings, keys, click, buzz,
// and buttons acutally behave during play.
/// @brief The main loop function.
/// @details Arduino/Teensy sketches run this function in a continuous loop.
/// * The overall high-level logic (e.g. when crank->isSpinning(), make sound) is contained here.
void loop() {

  if (first_loop) {
    welcome_screen();

    // This might have been reset above, so grab it now.
    play_screen_type = EEPROM.read(EEPROM_DISPLY_TYPE);

    // First time users will find their stuff doesn't work if they cleared their EEPROM if we don't read it again.
    ex1Button->setFunc(EEPROM.read(EEPROM_EX1));
    ex2Button->setFunc(EEPROM.read(EEPROM_EX2));
    ex3Button->setFunc(EEPROM.read(EEPROM_EX3));
    ex4Button->setFunc(EEPROM.read(EEPROM_EX4));
    ex5Button->setFunc(EEPROM.read(EEPROM_EX5));
    ex6Button->setFunc(EEPROM.read(EEPROM_EX6));

    #ifdef REV4_MODE
    ex7Button->setFunc(EEPROM.read(EEPROM_EX7));
    ex8Button->setFunc(EEPROM.read(EEPROM_EX8));
    ex9Button->setFunc(EEPROM.read(EEPROM_EX9));
    ex10Button->setFunc(EEPROM.read(EEPROM_EX10));
    #endif

    bigButton->setFunc(EEPROM.read(EEPROM_EXBB));

    use_solfege = EEPROM.read(EEPROM_USE_SOLFEGE);

    mel_vibrato = EEPROM.read(EEPROM_MEL_VIBRATO);

    // LED may have been reset, too... thanks John!
    if (EEPROM.read(EEPROM_BUZZ_LED) == 1) {
      #ifdef LED_KNOB
        mycrank->enableLED();
      #endif
    };

    // Crank On! for half a sec.
    u8g2.clearBuffer();
    u8g2.drawBitmap(0, 0, 16, 64, crank_on_logo);
    u8g2.sendBuffer();
    delay(750);

    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, 0, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    first_loop = false;
  };

  // Update the keys, buttons, and crank status (which includes the buzz knob)
  myoffset = mygurdy->getMaxOffset();  // This covers the keybox buttons.
  mycrank->update();

  #ifdef USE_PEDAL
    myvibknob->update();
  #endif

  myAButton->update();
  myXButton->update();

  #ifndef USE_GEARED_CRANK
  ex1Button->update();
  ex2Button->update();
  ex3Button->update();
  #endif
  bigButton->update();
  ex4Button->update();
  ex5Button->update();
  ex6Button->update();

  #ifdef REV4_MODE
  ex7Button->update();
  ex8Button->update();
  ex9Button->update();
  ex10Button->update();
  #endif

  bool go_menu = false;

  // Check if any pause manu EX buttons were pressed.
  #ifdef USE_GEARED_CRANK
  if ((bigButton->wasPressed() && bigButton->getFunc() == 1) ||
      (ex4Button->wasPressed() && ex4Button->getFunc() == 1) ||
      (ex5Button->wasPressed() && ex5Button->getFunc() == 1) ||
      (ex6Button->wasPressed() && ex6Button->getFunc() == 1)) {
    go_menu = true;
  }
  #else
  if (
      (ex1Button->wasPressed() && ex1Button->getFunc() == 1) ||
      (ex2Button->wasPressed() && ex2Button->getFunc() == 1) ||
      (ex3Button->wasPressed() && ex3Button->getFunc() == 1) ||
      (ex4Button->wasPressed() && ex4Button->getFunc() == 1) ||
      (ex5Button->wasPressed() && ex5Button->getFunc() == 1) ||
      (ex6Button->wasPressed() && ex6Button->getFunc() == 1) ||

      #ifdef REV4_MODE
      (ex7Button->wasPressed() && ex7Button->getFunc() == 1) ||
      (ex8Button->wasPressed() && ex8Button->getFunc() == 1) ||
      (ex9Button->wasPressed() && ex9Button->getFunc() == 1) ||
      (ex10Button->wasPressed() && ex10Button->getFunc() == 1) ||
      #endif

      (bigButton->wasPressed() && bigButton->getFunc() == 1)) {
    go_menu = true;
  }
  #endif

  // If the "X" and "O" buttons are both down, or if the first extra button is pressed,
  // trigger the tuning menu
  if ((myAButton->beingPressed() && myXButton->beingPressed()) || go_menu) {

    // Turn off the sound :-)
    all_soundOff();

    pause_screen();
    print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(),
                 tpose_offset, capo_offset, 0, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
  };

  if (myXButton->beingPressed() && myAltTposeUp->wasPressed()) {
    vol_up();
  };
  if (myXButton->beingPressed() && myAltTposeDown->wasPressed()) {
    vol_down();
  };

  bool any_newly_pressed = false;
  bool any_newly_released = false;

  // Check if any autocrank EX buttons were pressed
  #ifndef USE_GEARED_CRANK
  if (ex1Button->getFunc() == 11 && ex1Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex2Button->getFunc() == 11 && ex2Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex3Button->getFunc() == 11 && ex3Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else
  #endif

  if (bigButton->getFunc() == 11 && bigButton->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex4Button->getFunc() == 11 && ex4Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex5Button->getFunc() == 11 && ex5Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex6Button->getFunc() == 11 && ex6Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;

  #ifdef REV4_MODE
  } else if (ex7Button->getFunc() == 11 && ex7Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex8Button->getFunc() == 11 && ex8Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex9Button->getFunc() == 11 && ex9Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  } else if (ex10Button->getFunc() == 11 && ex10Button->wasPressed()) { 
    autocrank_toggle_on = !autocrank_toggle_on;
    any_newly_pressed = true;
  #endif
  };

  // Check if any autocrank EX buttons were released
  #ifndef USE_GEARED_CRANK
  if (ex1Button->getFunc() == 11 && ex1Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex2Button->getFunc() == 11 && ex2Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex3Button->getFunc() == 11 && ex3Button->wasReleased()) { 
    any_newly_released = true;
  } else
  #endif

  if (bigButton->getFunc() == 11 && bigButton->wasReleased()) { 
    any_newly_released = true;
  } else if (ex4Button->getFunc() == 11 && ex4Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex5Button->getFunc() == 11 && ex5Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex6Button->getFunc() == 11 && ex6Button->wasReleased()) { 
    any_newly_released = true;

  #ifdef REV4_MODE
  } else if (ex7Button->getFunc() == 11 && ex7Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex8Button->getFunc() == 11 && ex8Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex9Button->getFunc() == 11 && ex9Button->wasReleased()) { 
    any_newly_released = true;
  } else if (ex10Button->getFunc() == 11 && ex10Button->wasReleased()) { 
    any_newly_released = true;
  #endif

  };


  // Run EX functions (other than open-pause-menu and auto-crank)
  #ifndef USE_GEARED_CRANK
  if (ex1Button->wasPressed()) { ex1Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex2Button->wasPressed()) { ex2Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex3Button->wasPressed()) { ex3Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  #endif
  if (bigButton->wasPressed()) { bigButton->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex4Button->wasPressed()) { ex4Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex5Button->wasPressed()) { ex5Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex6Button->wasPressed()) { ex6Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };

  #ifdef REV4_MODE
  if (ex7Button->wasPressed()) { ex7Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex8Button->wasPressed()) { ex8Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex9Button->wasPressed()) { ex9Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  if (ex10Button->wasPressed()) { ex10Button->doFunc(mycrank->isSpinning() || autocrank_toggle_on); };
  #endif
  

  // NOTE:
  // We don't actually do anything if nothing changed this cycle.  Strings stay on/off automatically,
  // and the click sound goes away because of the sound in the soundfont, not the length of the
  // MIDI note itself.  We just turn that on and off like the other strings.

  // If the big button is toggled on or the crank is active (i.e., if we're making noise):
  if (autocrank_toggle_on || mycrank->isSpinning()) {

    // Turn on the strings without a click if:
    // * We just hit the button and we weren't cranking, OR
    // * We just started cranking and we hadn't hit the button.

    if (any_newly_pressed && !mycrank->isSpinning()) {
      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);

    } else if (mycrank->startedSpinning() && !autocrank_toggle_on) {
      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mytromp->soundOn(tpose_offset + capo_offset);
      mydrone->soundOn(tpose_offset + capo_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);

    // Turn off the previous notes and turn on the new one with a click if new key this cycle.
    // NOTE: I'm not touching the drone/trompette.  Just leave it on if it's a key change.
    } else if (mygurdy->higherKeyPressed() || mygurdy->lowerKeyPressed()) {
      mystring->soundOff();
      mylowstring->soundOff();
      mykeyclick->soundOff();

      mystring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mylowstring->soundOn(myoffset + tpose_offset, mel_vibrato);
      mykeyclick->soundOn(tpose_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);
    };

    // Whenever we're playing, check for buzz.
    if (mycrank->startedBuzzing()) {
      mybuzz->soundOn(tpose_offset + capo_offset);
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, true);
    };

    if (mycrank->stoppedBuzzing()) {
      mybuzz->soundOff();
      draw_play_screen(mystring->getOpenNote() + tpose_offset + myoffset, play_screen_type, false);
    };

  // If the toggle came off and the crank is off, turn off sound.
  } else if (any_newly_released && !mycrank->isSpinning()) {
    all_soundOff();

    // Just to be sure...
    all_soundKill();

    // Mark the time we stopped playing and trip the turn-off-the-display flag
    stopped_playing_time = millis();
    note_display_off = false;

  // If the crank stops and the toggle was off, turn off sound.
  } else if (mycrank->stoppedSpinning() && !autocrank_toggle_on) {
    all_soundOff();

    stopped_playing_time = millis();
    note_display_off = false;
  };

  // Once the sound stops, track the time until a fifth of a second has passed, then go back to the
  // non-playing display.  This just makes the display looks a bit nicer even if the sound jitters a
  // little.  It's subtle but I like the touch.
  if (!note_display_off && !autocrank_toggle_on && !mycrank->isSpinning()) {
    if ((millis() - stopped_playing_time) > 200) {
      note_display_off = true;
      print_display(mystring->getOpenNote(), mylowstring->getOpenNote(), mydrone->getOpenNote(), mytromp->getOpenNote(), tpose_offset, capo_offset, myoffset, mystring->getMute(), mylowstring->getMute(), mydrone->getMute(), mytromp->getMute());
    };
  };

  // Apparently we need to do this to discard incoming data.
  if (EEPROM.read(EEPROM_SEC_OUT) != 1) {
    while (MIDI.read()) {
      };
  };
  while (usbMIDI.read()) {
  };

  // My dev output stuff.
  test_count +=1;
  if (test_count > 25000) {
    test_count = 0;
     Serial.print("25,000 loop()s took: ");
     Serial.print(millis() - start_time);
     Serial.print("  Cur V: ");
     Serial.println(mycrank->getVAvg());
     // Serial.print("ms.  Avg Velocity: ");
     // Serial.print(mycrank->getVAvg());
     // Serial.print("rpm. Transitions: ");
     // Serial.print(mycrank->getCount());
     // Serial.print(", est. rev: ");
     // Serial.println(mycrank->getRev());
     start_time = millis();
//     #ifdef USE_PEDAL
//       Serial.println(String("") + "\nKNOB_V = " + myvibknob->getVoltage());
//       Serial.println(String("") + "KNOB_VIB = " + myvibknob->getVibrato());
//     #endif
  }

};
