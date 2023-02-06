# digigurdy-baz

See [the wiki here](https://github.com/bazmonk/digigurdy-baz/wiki) for more documentation.

See [the code documentation here](https://bazmonk.github.io/digigurdy-baz/html/index.html).

**Check out version 2.9.2! (NEW)**

* 2.9.2 - 
  * New EX function: toggle secondary output

* 2.9.1 - Maybe working REV4 build.

* 2.6.0 - Changes since 2.4

* 2.5.7 - Renamed clear EEPROM for clarity.
  * Other logic improvements in EX buttons
  * New Transpose Toggle EX function
  * Prettier EX button menu.

* 2.5.6 - Bug/hang fix, more improved trigger/crank behavior.

* 2.5.5 - Improved crank behavior, reducing phandom buzzes.

* 2.5.4 - Melody vibrato is now adjustable in the Input/Output Config menu.

* 2.5.3 - eliminated need for USE_TEENSY35/36 variables.

* 2.5.2 - 
  * Auto-Crank is an EX function.
  * Auto-Crank button is an EX button!

* 2.5.1 -
  * Added QR codes as a help function (to User Guide), and after the about screen (to main Digi-Gurdy site)

* 2.4.0 = 2.3.8 rebadged for main branch.  **Summary of changes since 2.2**:
  * *Switch to hardware SPI screen wiring requirement.*
    * This brings significant speed improvements!
  * Button performance fixes
  * Geared crank support
  * "Official" Trigger/Tsunami support incl. volume control, power control, and ability to switch between it and MIDI-OUT.
  * New EX functions: volume, transpose, capo.
  * Transpose and capo buttons are now programmable EX buttons 4-6.
  * Bug fixes, documentation and various code improvements.
  * Basil's secret fun menu for playing double tones on any string.

* 2.3.8 -
  * Parameterized all pins.
  * Moved Tsunami/Trigger config to main config file.

* 2.3.7 -
  * EX functions include transpose and capo functions.
  * Transpose and capos buttons *are* EX buttons.
  * EX menu prints current assigned function!

* 2.3.6 -
  * Basil's secret fun menu (ask him!)

* 2.3.5 -
  * ALLOW_COMBO_MODE in config.h allows MIDI-OUT and Trigger/Tsunami simultaneously.
  * Switching between MIDI-OUT and Trigger (if supported) is possible in-menu, not in config.h.
  * Trigger or Tsunami support must be compiled in.  If you don't one have... pick one you might get later, doesn't matter.

* 2.3.4 -
  * Extended volume up/down control to key combo for legacy support.  Will backport this to 2.0.x.
  * Making some functions to clean up the main ino file.

* 2.3.3 -
  * New EX button functions: turning volume up and down.
  
* 2.3.2 -
  * Broke out and modified Tsunami and Trigger libraries to support all 8 Teensy4.1 Serial TX pins.
  * Rewrote ALL documentation in doxygen format
    * docs included in ./docs/html/index.html
  * Added VScode configuration file that should make it happy, at least on a Mac.

* 2.3.1 -
  * Added TSUNAMI-OUT variable, made def channel 0
  * Linked trigger/tsunami volume to MIDI volume.  MIDI 112 = line-level wav out.
  * Geared crank detection:
    * If using geared crank, crank detection routine runs at startup.
    * EX config menu is also replaced with a option to trigger crank-redetection.

* 2.3.0 - Geared Crank Support!
  * Remember, in this branch you must have rewired your screen.

* 2.2.2 - Button debouncing
  * Big button debounce interval bumped up to 250ms.
  * EX/tpose/capo buttons up to 200ms (maybe higher than needed).

* 2.2.1 - Hardware SPI support/requirement
  * Necessary after testing discovered screen draw time was interrupting crank performance.
  * Using the teensy unit's hardware SPI controller fixes this nicely, and brings overall speed and
    responsiveness improvements.
  * However, this requires rewiring your screen!  See the wiki for more info.

* 2.2.0 - Typos plus all the 2.1.x changes below
  * Graphics overhaul to transition to U8G2.
  * Solfege support
  * A combined ABC/solfege notation Support
  * New play screen options
  * On-screen buzz indicator
  * MIDI performance improvements
  * Numerous typos fixed
  * Streamlined guided tuning menu with more intelligent default choices.

* 2.1.9 - Streamlined tuning menu (2.1.8 was a screen typo)
  * Will be 2.2.0 if I can't find a bug.

* 2.1.75 - Turned Buzz indicator menu into a toggle instead of two separate on/off options.

* 2.1.7 - Created a printed note play screen and options
  * For DoReMi support.

* 2.1.6 - Created a ABC/DoReMi "Combo" option

* 2.1.5 - DoReMi support
  * Enabled/disabled in Pause->Other-Playing Screen Config

* 2.1.4 - Cut down on excessive expression changes
  * Expression-based volume was being sampled and updated about 20 times a second.
  * By only sending changes (even tiny ones), this cuts down on a lot of idle MIDI messages.

* 2.1.3 - On-screen Buzz indicator
  * This can be enabled/disabled in the pause menu (Pause->Other->Playing Screen Config)
  * Option saves to EEPROM independent of save slot
  * Draws a thin rectangle around the play screen when you buzz!

* 2.1.2 -
  * Several formatting typos fixed.
  * Playing screen adjustment moved to pause screen (not at startup)
  * Added two new playing screen options.
  * Fixed several cases of the "# or X) Go Back" not having the # updated.
    * Now that this is standardized, I've probably missed it more.
  * Added name function to GurdyString
  * Removed unused "7" button message from menu_6
  * Tuning and volume screen show string name

* 2.1.1 -
  * Working build with U8G2.  Brings back Blue OLED support.

* 2.1.0 -
  * Beginning of conversion to U8G2.  Display.cpp done, starup_screens done.

* 2.0.0 -
  * Same as 2.0rc5.
  * See the extensive list of changes since 1.0 listed below!

* 2.0rc5 -
  * `config.h` options to enable/disable the accessory pedal.
  * Menu options for LED and accessory pedal won't display if disabled at compile time.

* 2.0rc4 -
  * Vibrato "accessory pedal" support.
    * This runs on the same pin as the Buzz LED.  Don't attempt to use both unless you find new pins.

* 2.0rc3 -
  * LED stops if trompette/buzz are muted. (Thanks David!)
  * Buzz knob now ranges from about 60rpm - 250rpm, and then ramps up to 500+.

* 2.0rc2 -
  * Fixed long-standing error in the order the strings are displayed on a preset screen.
  * Fixed bug where cleared EEPROM LED preference would not take effect until after an addition restart of the gurdy. (thanks John)
  * Uploaded new Trigger/Tsunami wav zips with a 50ms fade-in on the buzz sounds to soften their attack (thanks John!)

* 2.0rc1 -
  * Fixed EX button logic bug when using buttons other than EX1 for the pause screen.
  * Added EEPROM slots for EX button preferences (they persist across reboots now).
  * Added Pause->Options menu for turning the buzz LED on and off.
    * Also has EEPROM slot, also persistent.
    * This is distinct from disabling the LED in `config.h`, which prevents the object from being created at all.  This is for people that do have a buzz LED, but don't want it used for whatever reason.
  * **CLEAR YOUR EEPROM WHEN YOU INSTALL THIS VERSION!**

* 1.9.9 -
  * Moved the tuning menus internally.
  * Menu "button speed" standardized at 150ms across the menus.
  * Beginning prep for 2.0 release candidates.

* 1.9.83 -
  * Lowered volume of tracks pre-master mix.
  * Added builds for MIDI ("regular"), TSUNAMI, and TRIGGER versions.

* 1.9.82 -
  * Enabled looping on all tracks.  On WAV Trigger, there's an unavoidable gap when this happens.
  * Added 0.2s fade-out on tracks to make it slightly less abrupt.

* 1.9.81 -
  * Some commits of what I was doing was and wasn't using voice-stealing lock on the tracks.
  * This version, just to be clear, *does*.  On Trigger, you need firmware >= 1.30b for it to work!
  * On Tsunami, over v1.0 should have voice lock.
  * This fixes an issue where rapidly changing notes with the keybox (playing fast) would fool the "voice-stealing" algorithm of the Trigger/Tsunami into turning off "old" tracks... i.e. the drone/trompette.  I'm locking all sounds, as we're not using all the voices at all.

* 1.9.8 -
  * Experimental WAV Trigger and Tsunami Support.

* 1.9.7 -
  * Velocity-based volume change of strings!

* 1.9.6 -
  * Options menu now includes support for switching EX button functions.  There are 5 choices:
    * Toggle pause menu
    * Cycle through mute/unmute of melody Strings
    * Cycle through mute of drone/trompette strings (through all 4 combos)
    * Toggle drone mute only
    * Toggle trompette mute only
  * Muting trompette also mutes buzz now.

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
