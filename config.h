#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

const String VERSION = "2.3.1";
const String REL_DATE = "2022-12-19, v" + VERSION;

/// @brief This is a freeform line displayed on the About screen
const String EXTRA_LINE = "MIDI-OUT/HWSPI/PEDAL";
//const String EXTRA_LINE = "      3.5 TEST       ";
//const String EXTRA_LINE = " MIDI-OUT, LED, SWSPI";
//const String EXTRA_LINE = " TRIGGER - LED KNOB  ";
//const String EXTRA_LINE = " TSUNAMI - LED KNOB  ";

// This block here is for documentation purposes of macros that may be
// commented out.
#ifdef _DOXYGEN_
  /// @brief Enables SD1306 display support, do not use with BLUE_OLED.
  #define WHITE_OLED
  /// @brief Enables SSH1106 display support, do not use with WHITE_OLED.
  #define BLUE_OLED
  /// @brief Enables WAV Trigger support, effectively disables MIDI-OUT.
  /// @details Cannot be used with USE_TSUNAMI simultaneously
  #define USE_TRIGGER
  /// @brief Enables Tsunami support, effectively disables MIDI-OUT.
  /// @details Cannot be used with USE_TRIGGER simultaneously
  #define USE_TSUNAMI
  /// @brief Enables geared-crank support.
  /// @details Disable for optical-crank support
  #define USE_GEARED_CRANK
  /// @brief Enables an LED buzz indicator on LED_PIN.
  #define LED_KNOB
  /// @brief Enables the accessory/vibrato pedal on PEDAL_PIN.
  #define USE_PEDAL
#endif

// One of these OLED options must be enabled.
#define WHITE_OLED
//#define BLUE_OLED

//#define USE_GEARED_CRANK

//#define USE_TRIGGER
//#define USE_TSUNAMI

/// @brief The audio output channel used by the Tsunami unit.
/// @details 0 == 1L, 1 == 1R, etc.
const int TSUNAMI_OUT = 0;


//#define LED_KNOB

/// @brief Pin used for the LED buzz indicator, if LED_KNOB is enabled.
const int LED_PIN = 40;


#define USE_PEDAL

/// @brief Pin used for the accessory pedal, if USE_PEDAL is enabled.
const int PEDAL_PIN = 40;

/// @brief The max voltage reported by the accessory pedal.
/// @details 
/// This should be set to the max voltage your pedal will output.
/// * Value need not be exact
/// * Value is on a 0-1023 scale: 1023 = 3.3V
const float PEDAL_MAX_V = 658.0;


/// @brief Amount of modulation to apply to the melody strings.
/// @details
/// * Meant to give a slight vibrato effect
/// * Intensity 0 = no modulation, 127 = full modulation
/// * Actual modulation behavior is controlled by the MIDI sampler/synthesizer.  This only controls the intensity of it.
const int MELODY_VIBRATO = 16;


/// @defgroup optical Optical Crank Configuration Variables
/// These are configuration variables that only apply to optical-crank models. 
/// USE_GEAR_CRANK must be disabled for these to have effect.

/// @ingroup optical
/// @brief The crank speed in RPMs at which expression volume will max out.
const float EXPRESSION_VMAX = 120.0;

/// @ingroup optical
/// @brief The minimum expression volume.
/// @details
/// * Expression (MIDI CC11) value will be at least this much.
/// * Silent = 0, Max = 127 
const int EXPRESSION_START = 90;

/// @ingroup optical
/// @brief The number of "spokes" on the optical crank wheel.
/// @details * This is the number of black/blocking bars on the wheel, not the number of transitions.
const int NUM_SPOKES = 80;

/// @ingroup optical
/// @brief The crank speed at which sound begins to play in RPMs.
const float V_THRESHOLD = 5.5;

/// @ingroup optical
/// @brief The delay between crank samples in microseconds.
/// @details
/// * Code will cycle though at least this long between samples.
/// * This is not how long the code waits for movement, just how often it checks.
const int SAMPLE_RATE = 100;

/// @ingroup optical
/// @brief The maximum amount of time in microseconds to wait for crank movement.
/// @details * The actual wait time changes dynamically, but will not exceed this value.
const int MAX_WAIT_TIME = 40000;

/// @ingroup optical
/// @brief The multiplier applied to the velocity when no movement is detected.
/// @details * Smaller values cause sound to cut out more quickly once crank motion stop.
const float DECAY_FACTOR = 0.00;

// This is how long in milliseconds to buzz *at least* once it starts.
/// @ingroup optical
/// @brief The minimum duration of buzz sounds.
/// @details 
/// * Increase this if buzzing feels too "jittery" or rapid.
/// * Decrease if buzzing feels sluggish or unresponsive.
const int BUZZ_MIN = 100;


/// @defgroup gear Gear-Motor Crank Configuration Variables
/// These are configuration variables that only apply to gear-motor-crank models.
///
/// USE_GEAR_CRANK must be enabled for these to have effect.

/// @ingroup gear
/// @brief The number of voltage samples per update.
/// @details This is used to slow down how fast the code runs and avoid timing issues.  Choose based on your Teensy unit:
/// * Teensy3.5 @ 120MHz = 700   <-- full 3.5 speed
/// * Teensy3.5 @  72Mhz = 240   <-- probably too slow now
/// * Teensy3.5 @  48Mhz = 100   <-- probably too slow now
/// * Teensy4.1 @ 600MHz ~ 3850  <-- full 4.1 speed
/// * Teensy4.1 @ 150Mhz ~ 1150  <-- seems fine, fast enough, but why do this?
const int SPIN_SAMPLES = 700;

// This is the high voltage mark.  It determines how easily the crank makes the drones start.
// With my crank, I can go as low as 2, but it gets ridiculously sensitive (bumping into your gurdy
// agitates the crank enough to register).  It doesn't need to be very high, though, unless you want
// you crank to have a "minmum speed limit" before it starts sounding.
//
// Especially if you are using a bridge rectifier to have a 2-way crank, you'll want this **as low as you can.**
const int VOL_THRESHOLD = 5;

// (the equivalent of V_THRESHOLD for buzzing is what the knob does, so there's no variable for it).

// The crank uses an internal "spin" counter to make it continue to play through the inconsistent
// raw voltage the crank produces.  Each high voltage read increases the counter by SPIN_WEIGHT,
// up to MAX_SPIN.  While low voltage is being read, spin decreases by SPIN_DECAY.  While spin is
// greater than SPIN_THRESHOLD, it makes sound.
//
// A larger MAX_SPIN versus the SPIN_DECAY make the cranking more consistent at very low speeds,
// at the expense of responding quickly when you stop cranking.  A higher SPIN_WEIGHT influences
// how quickly the crank kicks on when you start spinning, and should be at least a few times larger
// than the SPIN_DECAY.  SPIN_THRESHOLD influences how quickly the noise cuts off after you stop
// spinning.
const int MAX_SPIN = 7600;
const int SPIN_WEIGHT = 2500;

// I think I've got good values for everything else... in my opinion at least, users should try adjusting just
// the SPIN_DECAY at first to change responsiveness.  Lower values make notes last longer over jitters in the
// crank voltage.  Higher values make it run down spin faster overall, increasing how quickly you can
// play staccato.
//
// Users using a rectifier for two-way cranking, you'll want to fiddle with this for personal feel.
// Users using a normal crank like John does it, you'll just want to increase if your crank is jittery at low
// speeds.  You don't need to adjust it much to hear a diffrence (try +- 20 at first).
const int SPIN_DECAY = 200;

// This is the point notes start:
const int SPIN_THRESHOLD = 5001;
// This is the point they stop:
const int SPIN_STOP_THRESHOLD = 1000;

// Buzzing works sort of the same way except the buzz counter jumps immediately to the
// BUZZ_SMOOTHING value and then begins to decay by BUZZ_DECAY.  Any positive "buzz"
// value makes a buzz.
//
// Larger BUZZ_SMOOTHING values make the buzz stay on more consistently once you've triggered it,
// but make it harder to make separate coups rapidly.  Adjust this to find the sweet spot between
// how easy you want it to buzz and how quickly/consistently you can work a crank.  Players much
// better than me may want a smaller value.  ***THE SPIN VALUES DON'T AFFECT BUZZ**
const int BUZZ_SMOOTHING = 250;
const int BUZZ_DECAY = 1;



// KEYBOX VARIABLES:
// The pin_array[] index here represents the MIDI note offset, and the value is the corresponding
// teensy pin.  This defines which physical keys are part of the "keybox" and what order they're in.
//
// NOTE: There's no key that produces 0 offset (an "open" string),
// so the first element is bogus.  It gets skipped entirely.
const int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
                   7, 31, 8, 32, 33, 18, 34, 19, 35, 36, 20, 37};

// This is literally just the size of the above array minus one.  I need this as a const to
// declare the KeyboxButton array later on... or I just don't know enough C++ to know how to
// *not* need it ;-)
const int num_keys = 24;

// These control which buttons on the keybox have the roles of the X, A/B, 1-6, etc. buttons.
// Users with non-"standard" keyboxes (if there is such a thing!) may need to adjust these.
//
// These are array indexes, so if you count chromatically up your keybox and then subtract 1,
// that is its index.
const int X_INDEX = 0;
const int A_INDEX = num_keys - 2;
const int B_INDEX = num_keys - 5;
const int BUTTON_1_INDEX = 1;
const int BUTTON_2_INDEX = 3;
const int BUTTON_3_INDEX = 4;
const int BUTTON_4_INDEX = 6;
const int BUTTON_5_INDEX = 8;
const int BUTTON_6_INDEX = 9;
const int TPOSE_UP_INDEX = num_keys - 1;
const int TPOSE_DN_INDEX = num_keys - 3;

#endif
