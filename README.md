# digigurdy-baz

**Pre-compiled builds of the code will be made available from 1.7.99 and up.  See https://github.com/bazmonk/digigurdy-baz/wiki/install-by-hex for installation instructions.**

**Check out version 1.9.4! (NEW)**

* 1.9.5 -
  * Internal support for exButton configurable actions.
  * Fix necessary to work around Teensyduino 1.57 (the latest) bug.
    * https://forum.pjrc.com/threads/71359-EEPROM-h-doesn-t-include-String-library-on-its-own-now-that-it-has-String-support?p=314717#post314717

* 1.9.4 -
  * Using the MIDI object the right way
  * Moved menu screen functions off to separate file
  * Cleaned up header tree and general structure.
  * Compiled code less than 50% of 1.7.99.

* 1.9.3 -
  * I've been using the wrong `string` :-(  Migrated from `std::string` to `String`.
  * Compiled code got about 40% smaller.

* 1.9.2 -
  * Further re-org.

* 1.9.1 -
  * The new "extra" buttons are (left-to-right) called ex1, ex2, ex3.
    * ex1 toggles the pause menu (the "A+X" combo)
    * ex2 toggles through the melody mute options (there's 3)
    * ex3 toggles through the drone/trompette mute options (there's 4)
    * (the second two work during play!)
  * I've made the crank a little more sensitive to cutting off when cranking stops.
  * I've made the minimum velocity for sound a tiny lower (about 1 rev per 5 seconds, pretty slow)
  * Cleaned up some unused variables.


* 1.9.0 -
  * Reorganization of custom classes into separate files.  It's for my sanity... this makes the main file about 25% smaller, and enforces some best practices on my part.
  * Introduction of precombiled builds for users.

* 1.7.99 -
  * Buzz Knob LED can be disabled in config.h if desired.  LED_PIN also a config variable.
  * Buzz Knob LED implemented (~~or any LED you happen to connect to pin 40~~ see above)
  * Timings of debounce button performance improved
  * Taking average of last six readings now to determine voltage.
  * Sampling is done as quick as possible, but the time to expect a sensor-change is determined based on the average times of the last several readings.
  * There is now a BUZZ_MIN variable.  Buzz events will last at least this long (smoother buzz).
* OPTICAL CRANK VERSION!  Versions above v1.5 will be for John's new IR-sensor based crank design.
  * It's smooth!
  * Buzzing acts better.
  * The `config.h` file contains all-new parameters for adjusting the crank behavior.  There's less of them, and they're easier to understand in terms of what they actually do.
  * This code expects to have the sensor hooked up to it directly, *not* through a TeensyLC.
    * This will be explained [in a wiki page here soon](https://github.com/bazmonk/digigurdy-baz/wiki/optical_setup).
    * Long story short, the OUT pin of the sensor goes straight to the same pin on the Teensy as the crank did before (that's pin 15, a.k.a. A1).  You need to run 3.3v (NOT 5v, do not do that) to the sensor.  I'm grabbing it off the voltage wire going to the buzz potentiometer because it's somewhat accessible on my unit w/o taking it all apart.
  * **The motor-crank version of the code is still available in the [v1.4.1 branch of this repo](https://github.com/bazmonk/digigurdy-baz/tree/v1.4.1).**  It will continue to get bug fixes and any UI improvements that carry over from future versions.
  * This code has only been tested on Teensy4.1, Adafruit ("WHITE") screen.  The BLUE screen isn't tested and likely doesn't work.  Teensy3.5 is untested, but likely does work just fine.

Other "testing" branch changes since v1.0:

* bs16i scene switching with tuning change!
  * When enabled, if you have scenes (bs16i "presets") set up and assigned to numbers, this will tell bs16i to automatically switch scenes when you change to a preset or saved tuning slot.  The first four bs16i scenes go with the four preset tunings, and the next four go with the save slots.  [More info below.](https://github.com/bazmonk/digigurdy-baz/tree/testing#bs16i-scene-switching)
* **Manual volume control of all channels!**
  * Volume saves with tuning in save slots.
  * Gurdy starts up with an even volume on all channels (like before).  Presets do *not* carry volume info.
* Chanter/melody string on/off control!
* Renaming of the O/OK key and Capo key (the key next to it) to A and B respectively
  * This is both for clarity, and because the keys no longer stick to those functions.
* Basic voltage compensation!
  * Should let you lower your `V_THRESHOLD` and get better crank performance.
* **Configuration variables are now in a config.h file**
* Tweaked spin algorithm and values for use with reversible cranks.  Should still work fine with normal cranks!
  * Lower `SPIN_DECAY` if you find it too sensitive at slow speeds before trying to mess with other values.  It'll give you a good place to start.
  * Raise `V_THRESHOLD` if you get "phantom" cranking.  I *strongly recommend* you slightly modify your crank motor by grounding it to the negative wire somehow.  In my tests it significantly elminates a lot of the noise from the crank sampling and allows you to have a hair-trigger crank and great responsiveness.
  * Raise `BUZZ_SMOOTHING` to make coups smoother, lower it to allow you to make coups faster.  Buzzing is separate from the `SPIN_` variables entirely so don't adjust them in an attempt to influence the buzz.  Because you can adjust the buzz voltage on-the-fly with the knob, this shouldn't require much adjustment.  
* Cranking uses a rolling average to calculate voltage now.
* No need to manually edit display library header file to specify resolution (for white OLEDs)
  * Stock Adafruit_SSD1306 library works out-of-the-box, no weird half-screen.
* Formatting/look-n-feel tweaks.
* **Manual free-tuning of _each_ string!**
* Ability to remove/attach crank during play.
* Tuninig preset names are easily-adjustable with the tuning itself.
* Warns before overwriting occupied save slot.
* A re-worked analog polling mechanism giving better crank performance
* Set this if you want the melody strings to send any modulation (usually used as vibrato).
  * Setting it to zero disables the feature (I'll leave it this way in the repository).
* Added a new bs-16i "Scene" profile that uses this feature.
* https://www.youtube.com/watch?v=lreQdIwvJE4 - Example of using `MELODY_VIBRATO = 16`

## About

This is the software repository for the digigurdy code (https://digigurdy.com).

## I tried it and I don't like it.  Now what?

This repository includes (in the `other` folder) a recent version of John Dingley's original digigurdy code, as well as a version of it I modified with a different tuning setup.  Follow the same installation instructions above to put the old code back, and thanks for trying it out!

If there's something particular about it that annoys you, feel free to open an issue (see the top of this page) for me to suggest what you'd like me to change, and I'll see if I can accommodate it.

## I found a bug.  

Please open an issue (top of the page here) and describe it for me.  If it's a build problem, I'd like to see the output of Teensyduino (from the bottom panel) when it fails.  

Big bonus points (I'll make a credits screen for you all) if you can reproduce it and tell me how so I can fix it.  Git issues here generate emails for me so it's the preferred way to document them (versus facebook or some other method).

## I want it to do something it doesn't do.

Feel free to open an issue.  I'll see what I can do.  This is all in my spare time so responsiveness is what it is, but I will see your issue pretty quick.

Part of rewriting this was with maintainability and extendability in mind.  The code is well-formatted, I've abstracted the strings, MIDI interaction, buttons, keybox, crank, etc. into easy-to-use classes.  I put a lot of effort into using readable variables, commenting the crap out of the code, and not doing anything needlessly "clever"... I tried to make it easy for you to explore adding to it yourself.

I haven't really thought of how I'll handle a bunch of competing pull requests because I'm not expecting it, but feel free to clone the repo, make improvements, and submit a pull request.  I'm open to them.

## Acknowledgements

HUGE credit to John Dingley, his amazing creation (the DigiGurdy itself), and the original codebase that he wrote.  He worked out the display, the way the original crank worked, the interaction with the MIDI subsystem... I'm standing on his shoulders.
