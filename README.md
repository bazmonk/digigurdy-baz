# digigurdy-baz

**Pre-compiled builds of the code will be made available from 1.7.99 and up.  See https://github.com/bazmonk/digigurdy-baz/wiki/install-by-hex for installation instructions.**

**Check out version 1.9.1! (NEW)**

* 1.9.1 -
  * Leftmost button acts as pause menu shortcut.

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

### Connecting to a Synthesizer

The DigiGurdy is just a MIDI controller in a hurdy-gurdy-like shape.  It gives out *instructions* on how to make notes, but it has to be connected to a synthsizer to actually *interpret* those into music.

DigiGurdy supports both SerialMIDI over USB, and a powered, standard MIDI-out jack, which can be used with a Bluetooth MIDI transmitter, for example.  Both are ready upon startup with no extra setup necessary on the controller end.  Both the USB MIDI and MIDI jack are running simultaneously: if for some reason you want to use both and hook it up to two synthesizers at the same time, go for it.

The closest thing to a "standard" recommendation for a synth is bs-16i, a MIDI app for iOS.  It works great, it can be connected into Garageband or other apps in your workflow, etc.  If you go with this option, there's a couple presets in this repository for you to try.

#### Soundfonts

In addition to a synthesizer, it needs samples of hurdy gurdy sounds to work with.  I've hosted the soundfont file that comes with DigiGurdies here as well for your downloading convenience.  **Big shout-out to the MIDIgurdy project** which created most of these sounds and were kind enough to release them open-source for our mutual benefit.

https://github.com/bazmonk/digigurdy-baz/blob/main/soundfonts/vienna-alto-mod11.sf2

## Operation, Setup, and Play

Here's a quick video explaining the controls:

https://www.youtube.com/watch?v=yC0skMH7qog

The keys for navigating the menus work much like the original code did.  The bottom row represent number choices in menus, 1-6 starting from the left.  The "A" button is the top right button, "B" is the one next to that, and the "X" button is the top left:

![keybox pic](https://raw.githubusercontent.com/wiki/bazmonk/digigurdy-baz/keycombos.jpeg)

During play, these are all the "hidden" keycombos:
* "X" + "A" = Pause menu: save/load/change tuning, turn drones on/off
* "X" + "B" = Toggle through capos (just like the button on newer ones)
* "1" + Tpose Down or Tpose Up = Transpose up and down (just like the buttons on newer ones)

### Startup

After showing the Adafruit logo (represent!) and a title screen, crank detection will silently work in the background for about 5 seconds.

* **DO NOT TURN THE CRANK DURING CRANK DETECTION!**  It can fool the crank detection into paradoxically not detecting a crank.  Since > v1.0 crank detection is much faster, so this is a lot harder to do now.  If you manage to do it--you're fast!--just restart it again.

From there, you will be shown the Startup Menu.  There's options here to load one of the included presets, load a tuning you've saved before, set up a new tuning, and other options.

**New Users**: the four preset options are for you!  Each one is a reasonable combination, and they're good starting points while you experiment with making others.

#### Other Options
The "Other Options" menu at startup offers two options:
* Clear EEPROM - this clears the memory and save slots (and all of your digigurdy's memory).  First-time installations should do this just to be sure (other data in there can cause bugs, and sometimes the save format changes a little).  It doesn't hurt to do this after upgrading, either...
* Playing Screen - during actual play, you can either have a large "C7"-like display, or that side-by-side with a staff graphic showing the note as well.  This is adjustable here.

The "Other Options" during play has two different options:
* Remove/Attach Crank - this goes through the crank-detection routine that happens on startup again.  If you have a model with a detachable crank, this allows you to dock/un-dock it without having to restart it.
* About screen - Hit the X button to go through it.  Useful for reporting problems: it helps me if you can tell me what version you're using.  The URL on it links back to this repository if you forget where to find this page.  Also a quick thanks to folks that have been helping me throughout this.

### Play

Once you select a tuning, you'll get the "Crank On!" screen briefly, and your digigurdy is ready to play!  You can activate the drones and keybox with either the large arcade button on the front of the keybox, or (if you have one connected) the crank.  The knob next to the arcade button adjusts the buzz sensitivity like it did before.

At any time during play (even if you're not acutally making sound right now), you can use the two "extra" (these used to be for octave up/down in the old digigurdy code) buttons on the digigurdy (if you have these buttons... I'm sorry if you don't, there's no way for you to access these features yet) to transpose all of the strings up or down.  You'll see the display show the updated tunings and how far you've transposed it from the default tuning.

The third "extra" button toggles between a whole-note or two-whole-note capo on the drone/trompette strings, much like how a capo would work on a physical hurdy gurdy.  This is in addition to any transposing, letting you apply capos to any of the transposed tunings as you may be already used to on a real gurdy.

### Re-Tuning, Saving

At any time during play, pressing the X and A (top left and right keys) at the same time brings up the Pause Menu.  This is a lot like the Startup Menu, except here you have an option to save your current tuning instead of the startup options.

There are four save slots available to you.  They preserve the tunings and capo/transpose settings.

Hitting the 'A' button in the Pause Menu toggles through muting options on the drone and trompette strings.  Your current mute settings will be reflected on the main screen.
  * Drone/trompette mute settings are not part of the save settings and must be set every play session.

#### bs16i Scene Switching

If you are using bs16i, enable this feature in the options screen to allow the digigurdy to signal it when you change tunings.

How it works:
* In bs16i, create a "scene"... that's what the app calls a preset.  Choose the instruments and effects you want, etc.
* Under the Settings->Scenes->Assignment menu, assign it to a number.  0-3 correspond with the four preset tunings on the digigurdy, and tunings 4-7 correspond with the four tuning slots that you can save yourself.
* Under Settings->CoreMIDI, turn on the "Switch Scenes with Program Change" feature.  You can either use Omni, or Channel 1 (digigurdy will sent on channel 1).
* Finally, in the digigurdy options screen, enable Scene Control.

Now, when you're using your digigurdy and switch to a preset or a save slot, bs16i will automatically switch to the scene you've configured to go with that tuning!

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
