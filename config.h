#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

const String VERSION = "2.9.4x";
const String REL_DATE = "2023-02-19, v" + VERSION;

/// @defgroup config Configuration Options
/// These variables/definitions are compile-time configuration options.
/// @{

/// @brief This is a freeform line displayed on the About screen
const String EXTRA_LINE = " Rev4.0 Test Build ";
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
  /// @brief Enables WAV Trigger support.
  /// @details Cannot be used with USE_TSUNAMI simultaneously
  #define USE_TRIGGER
  /// @brief Enables Tsunami support.
  /// @details Cannot be used with USE_TRIGGER simultaneously
  #define USE_TSUNAMI
  /// @brief Enables geared-crank support.
  /// @details Disable for optical-crank support
  #define USE_GEARED_CRANK
  /// @brief Enables an LED buzz indicator on LED_PIN.
  #define LED_KNOB
  /// @brief Enables the accessory/vibrato pedal on PEDAL_PIN.
  #define USE_PEDAL
  /// @brief Setting this option allows both MIDI-OUT and Trigger/Tsunami use simultaneously.
  /// @details Set this only if the MIDI-OUT and Trigger/Tsunami Tx pins are different!
  #define ALLOW_COMBO_MODE
  /// @brief Setting this option enables EX pins 7-10, and SPI2 display.
  /// @details meant for rev4 boards.
  #define REV4_MODE
#endif

// One of these OLED options must be enabled.
#define WHITE_OLED
//#define BLUE_OLED

//#define USE_GEARED_CRANK

// Only one of these should be defined.
#define USE_TRIGGER
//#define USE_TSUNAMI

#define ALLOW_COMBO_MODE
#define BAZ_MODE

//#define REV4_MODE

/// @brief The audio output channel used by the Tsunami unit.
/// @details 0 == 1L, 1 == 1R, etc.
const int TSUNAMI_OUT = 0;

/// @brief The Serial port to use on the Teensy unit for the Tsunami.
#define __TSUNAMI_USE_SERIAL1__

/// @brief The Serial port to use on the Teensy unit for the Trigger.
//#define __WT_USE_SERIAL1__
#define __WT_USE_SERIAL5__


#define LED_KNOB

/// @brief Pin used for the LED buzz indicator, if LED_KNOB is enabled.
const int LED_PIN = 40;


//#define USE_PEDAL

/// @brief Pin used for the accessory pedal, if USE_PEDAL is enabled.
const int PEDAL_PIN = 38;

/// @brief The max voltage reported by the accessory pedal.
/// @details 
/// This should be set to the max voltage your pedal will output.
/// * Value need not be exact
/// * Value is on a 0-1023 scale: 1023 = 3.3V
const float PEDAL_MAX_V = 658.0;

/// @}

/// @defgroup optical Optical Crank Configuration Variables
/// These are configuration variables that only apply to optical-crank models. 
/// USE_GEAR_CRANK must be disabled for these to have effect.

/// @ingroup optical
/// @brief The crank speed in RPMs at which expression volume will max out.
const float EXPRESSION_VMAX = 80.0;

/// @ingroup optical
/// @brief The minimum expression volume.
/// @details
/// * Expression (MIDI CC11) value will be at least this much.
/// * Silent = 0, Max = 127 
const int EXPRESSION_START = 70;

/// @ingroup optical
/// @brief The number of "spokes" on the optical crank wheel.
/// @details * This is the number of black/blocking bars on the wheel, not the number of transitions.
const int NUM_SPOKES = 80;

/// @ingroup optical
/// @brief The crank speed at which sound begins to play in RPMs.
const float V_THRESHOLD = 4.0;

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

/// @ingroup gear
/// @brief The lower voltage threshold for the crank to register motion.
/// @details 
/// * Voltage values are reported between 0-1023, with 0 = 0V and 1023 being 3.3V.
/// * This should generally be set as low as possible to be sensitive but not register "phantom" cranking.
const int VOL_THRESHOLD = 5;

/// @ingroup gear
/// @brief The maximum "spin" value.
/// @details Gear crank behavior uses a "spin" number to determine motion, which is added to when cranking is detected, and subtracted from when cranking stops.
/// This is the maximum spin value the counter will allow.
const int MAX_SPIN = 7600;

/// @ingroup gear
/// @brief The amount of spin to add when motion is detected.
/// @details Every cycle that cranking is detected, this amount is added to the spin value, up to the MAX_SPIN.
const int SPIN_WEIGHT = 2500;

/// @ingroup gear
/// @brief The amount of spin to subtract when motion is not detected.
/// @details Every cycle that cranking is not detected, this amount is subtracted from the spin value, down to zero.
/// * This should be significantly less than SPIN_WEIGHT in order to give a smoothing effect, as the geared cranks induce a voltage in steps, not smoothly.
/// * If you have a rectifier on your crank to register motion in both directions, you also don't want it to be too low as to smooth over direction changes entierly.
const int SPIN_DECAY = 200;

/// @ingroup gear
/// @brief The amount of spin necessary to make sound.
/// @details Ultimately, sound will be produced when spin is above this value.
const int SPIN_THRESHOLD = 5001;

/// @ingroup gear
/// @brief The amount of spin below which sound stops.
/// @details Sound will stop when spin is below this value.
/// * Note that it is signficantly lower than SPIN_THRESHOLD.
const int SPIN_STOP_THRESHOLD = 1000;

/// @ingroup gear
/// @brief The amount of buzz "smoothing" to start with when buzzing registers.
/// @details * When the crank voltage registers higer than the buzz knob's voltage, buzzing registers.
/// * Buzz is set to this value when buzzing registers and decays.
/// * The buzzing effect continues as long as this smoothing value is positive
const int BUZZ_SMOOTHING = 250;

/// @ingroup gear
/// @brief The amount of buzz "smoothing" to subtract when buzzing is not registering.
/// @details Buzz smoothing is subtracted by this amount every cycle that buzz does not register.
/// * This produces a set delay for buzzing to stop, as well as a minimum buzz duration.
const int BUZZ_DECAY = 1;


// These are all keybox pins:

/// @ingroup config
/// @{

/// @brief The ordered layout of the keybox keys/buttons.
/// @details * This both determines which Teensy pins compose the keybox, and the order they are in.
/// * index 0 is unused and would theoretically not affect the note being played.
/// * index 1 raises the note played by 1 semitone, index 2 by 2 semitones, etc.
/// * pin_array can be extended or shortened for larger/smaller keyboxes
#ifdef REV4_MODE
const int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
                   7, 31, 8, 32, 33, 11, 34, 10, 35, 36, 9, 37};
#else
const int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
                   7, 31, 8, 32, 33, 18, 34, 19, 35, 36, 20, 37};
#endif

// This is literally just the size of the above array minus one.  I need this as a const to
// declare the KeyboxButton array later on... or I just don't know enough C++ to know how to
// *not* need it ;-)
/// @brief The number of keys on the keybox.
/// @details This must/should be set to the length of pin_array[] - 1.
const int num_keys = 24;

// These control which buttons on the keybox have the roles of the X, A/B, 1-6, etc. buttons.
// Users with non-"standard" keyboxes (if there is such a thing!) may need to adjust these.
//
// These are array indexes, so if you count chromatically up your keybox and then subtract 1,
// that is its index.
/// @brief The position of the "X" key on the keybox.
/// @details * This is intended to be the upper leftmost key.
/// * Note that the first key position is 0.  This is the index in pin_array - 1!
const int X_INDEX = 0;

/// @brief The position of the "A" key on the keybox.
/// @details This is intended to be the upper rightmost key.
const int A_INDEX = num_keys - 2;

/// @brief The position of the "B" key on the keybox.
/// @details This is intended to be the upper key second from the right.
const int B_INDEX = num_keys - 5;

/// @brief The position of the "1" key on the keybox.
/// @details This is intended to be the leftmost key on the bottom.
const int BUTTON_1_INDEX = 1;

/// @brief The position of the "2" key on the keybox.
/// @details This is intended to be the 2nd key from the left on the bottom.
const int BUTTON_2_INDEX = 3;

/// @brief The position of the "3" key on the keybox.
/// @details This is intended to be the 3rd key from the left on the bottom.
const int BUTTON_3_INDEX = 4;

/// @brief The position of the "4" key on the keybox.
/// @details This is intended to be the 4th key from the left on the bottom.
const int BUTTON_4_INDEX = 6;

/// @brief The position of the "5" key on the keybox.
/// @details This is intended to be the 5th key from the left on the bottom.
const int BUTTON_5_INDEX = 8;

/// @brief The position of the "6" key on the keybox.
/// @details This is intended to be the 6th key from the left on the bottom.
const int BUTTON_6_INDEX = 9;

/// @brief The position of the "T-UP" key on the keybox.
/// @details This is intended to be the bottom right key.
const int TPOSE_UP_INDEX = num_keys - 1;

/// @brief The position of the "T-DOWN" key on the keybox.
/// @details This is intended to be the bottom second key from the right.
const int TPOSE_DN_INDEX = num_keys - 3;


// These are less-commonly-changed options:

/// @brief The pin running to the crank.
/// @warning If using a geared crank, this pin must be analog-capable.
const int CRANK_PIN = 15;

/// @brief The analog pin running to the buzz potentiometer/knob.
/// @warning This pin must be analog-capable.
const int BUZZ_PIN = 16;

/// @brief The pin running to the arcade, "auto-crank" button.
const int BIG_BUTTON_PIN = 39;

#ifdef REV4_MODE
/// @brief The pin to the EX1 button.
const int EX1_PIN = 19;
/// @brief The pin to the EX2 button.
const int EX2_PIN = 13;
/// @brief The pin to the EX3 button.
const int EX3_PIN = 12;
/// @brief The pin to the EX4 button.
const int EX4_PIN = 41;
/// @brief The pin to the EX5 button.
const int EX5_PIN = 40;
/// @brief The pin to the EX6 button.
const int EX6_PIN = 17;
/// @brief The pin to the EX7 button.
const int EX7_PIN = 14;
/// @brief The pin to the EX8 button.
const int EX8_PIN = 18;
/// @brief The pin to the EX9 button.
const int EX9_PIN = 22;
/// @brief The pin to the EX10 button.
const int EX10_PIN = 23;

#else
/// @brief The pin to the EX1 button.
const int EX1_PIN = 41;
/// @brief The pin to the EX2 button.
const int EX2_PIN = 17;
/// @brief The pin to the EX3 button.
const int EX3_PIN = 14;
/// @brief The pin to the EX4 button.
const int EX4_PIN = 21;
/// @brief The pin to the EX5 button.
const int EX5_PIN = 22;
/// @brief The pin to the EX6 button.
const int EX6_PIN = 23;
#endif

/// @}
#endif
