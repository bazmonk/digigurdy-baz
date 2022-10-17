
// ALL USERS!!! Uncomment one of these lines depending on what kind of OLED screen you have.
#define WHITE_OLED
//#define BLUE_OLED

// VIBRATO: I use a long-delay, very slow vibrato on the melody strings.  This variable controls how
// much vibrato (how much modulation like with a physical mod wheel on a MIDI keyboard) to send.
// Setting it to 0 sends no modulation.  Max is 127.  I use 16...
const int MELODY_VIBRATO = 16;

// Cranking and buzz behavior:

// We're doing continuous reading of the cranks now, so reads are nearly instantaneous.  To smooth
// out the voltage so our readings don't wander on their own, we're taking several thousand
// very fast readings and averaging them to get our "reading" in the code.  This is how many readings
// we average.
//
// You do what you want, but in my opinion, this should be left alone in lieu of adjusting the SPIN
// and BUZZ variables below.  I'd only adjust this to make a faster Teensy or other controller run
// as quickly as this does.
//
// Currently, the rest of the settings assume "1,000 loop()s took: 139ms" per the Serial Monitor output.
// Teensy3.5 @ 120MHz = 700   <-- full 3.5 speed
// Teensy3.5 @  72Mhz = 240   <-- this seems to work fine...
// Teensy3.5 @  48Mhz = 100   <-- more like 160ms... this is so-so-stable.
// Teensy4.1 @ 600MHz ~ 3850  <-- full 4.1 speed... seems unnecessary
// Teensy4.1 @ 150Mhz ~ 1150  <-- seems fine

const int SPIN_SAMPLES = 700;

// This is the high voltage mark.  It determines how easily the crank makes the drones start.
// With my crank, I can go as low as 2, but it gets ridiculously sensitive (bumping into your gurdy
// agitates the crank enough to register).  It doesn't need to be very high, though, unless you want
// you crank to have a "minmum speed limit" before it starts sounding.
//
// Especially if you are using a bridge rectifier to have a 2-way crank, you'll want this **as low as you can.**
const int V_THRESHOLD = 5;

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
int pin_array[] = {-1, 2, 24, 3, 25, 26, 4, 27, 5, 28, 29, 6, 30,
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
