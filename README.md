# digigurdy-baz

**Check out version 1.4.0! (NEW)**

* Chanter/melody string on/off control!
* Renaming of the O/OK key and Capo key (the key next to it) to A and B respectively
  * This is both for clarity, and because the keys no longer stick to those functions.

Other "testing" branch changes since v1.0:

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

This is a rewrite of the digigurdy code (https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy).

The code has been significantly streamlined (20% the size of the original source code, compiles to only 30% the teensy3.5's capacity), and there are several new features:

* A re-worked tuning menu, allowing a choice of G/C or D/G tuning, each with a selection (one each, four total) of:
  * 4 high chanterelles (unison and fifths)
  * 4 low chanterelles (unison and fifths)
  * 6 drones
  * 6 trompettes
* The capo button allows **on-the-fly** toggling of 1 or 2 whole-note capos on the drone/trompette strings.
* Two transpose buttons allow **on-the-fly** transposing of all strings up *or* down one octave in half-steps (12 up, 12 down)!
  * Every tuning can be played in *any key* within an octave of it.
* 4 pre-loaded default presets (matching John Dingley's orignal presets)
* 4 _save slots_ for saving your favorite tunings!
* Built-in EEPROM clearing feature for first-time installation
* **On-Screen-Display** showing current string tunings, transpose and capo settings, as well as a read-out of the current note being played on the keybox.
  * While playing, a [big pretty bitmap note](https://www.youtube.com/shorts/XOBMtSio8nA) or [a note + staff](https://www.youtube.com/shorts/KD5NucKWTos) now displays instead of the full settings.  They still show up when not playing.  This is adjustable in the startup menu.
* Muting of both or either the drone/trompette strings directly from the pause menu.

## Installation

### OLED Display
Your digigurdy may have an SSD1306 display (the "white OLED"), or the SH1106 display (the "blue OLED").  This code needs to be compiled for the correct display.

_Note: Older versions of the code also needed to have the Adafruit_SSD1306.h file slightly edited to specify the correct resolution, causing a "half-screen" problem if it wasn't.  This has been fixed in v1.2.8.  No editing of the libraries are necessary now._

Near the very top of the code are two `#define` statements.  Uncomment the appropriate line to build for either display type.  The version on this repository is set to white (because that's the one I have).

For white OLEDs:

```
#define WHITE_OLED
//#define BLUE_OLED
```

For blue OLEDs:

```
//#define WHITE_OLED
#define BLUE_OLED
```

### Teensy4.1 Support
Digigurdy-baz has preliminary Teensy4.1 support.  The `SPIN_SAMPLES` variable should be adjusted so that each cycle of the code takes about as long.  These are the values I use, which both give about 100 cycles in 40ms.  This isn't a magic number, but it's what I settled on during testing and base the rest of the timing off of.  The crank/buzz should behave about the same by adjusting this to match:

```
// Teensy3.5 @120mHz = 4000
// Teensy4.1 @600mHz = 11000
const int SPIN_SAMPLES = 11000;
```

The `testing` branch of the code has Serial Monitoring turned on and prints each 100 loops with how long it's taking.  If you hook up your digigurdy to your computer and view the serial monitor inside the Teensyduino program, you can use this to gauge what `SPIN_SAMPLES` should be in other situations.

### Compiling/Uploading
After that, the steps are the same for the original digigurdy code.  You will need:
  * Teensyduino, the IDE and build environment for the electronics
  * The appropriate `Adafruit_[SSD1306 or SH1106]` library (see the whilte/blue display section above) for your gurdy installed within Teensyduino (`Sketch->Include Library->Manage Libraries...`).
    * If it isn't already installed or doesn't install with the OLED library as a dependency, you also need the `Adafruit_GFX` general graphics library as well.
    * If you have the SH1106 "blue" OLED, you need the SH1106 libraries here: https://github.com/wonho-maker/Adafruit_SH1106. Other versions will not work.
  * The main `digigurdy-baz.ino` file along with the header files in a directory/folder named `digigurdy-baz`.  Teensyduino will offer to make this folder and move the file around if you don't.  The easiest way to make sure everything is in the right place is to simply clone this repository from github to your computer.  The repo itself is named the correct way for Teensyduino, and you can easily pull new updates later on if you want.

With that in place, you should be able to open the `digigurdy-baz.ino` file in Teensyduino, and compile/upload it to your digigurdy.  Make sure to select the correct port for it and that the other settings look like this after plugging your gurdy to your computer:

![teensyduino settings](https://raw.githubusercontent.com/wiki/bazmonk/digigurdy-baz/teensyduino.png)

NOTE: Make sure in particular that the Teensy version is correct and that "USB Type" is "Serial + MIDI".  

After uploading, it should automatically restart and be ready-to-go!

### Clear EEPROM for first-time installations

Testing has already shown that strange side-effects occur when the EEPROM (your digigurdy's memory) is filled with other stuff.  The start-up screen includes an `Other Options` selection, and inside is an option to clear your EEPROM.

First-time users of this codebase should do this before continuing.  It only needs to be done once.

Regular users can utilize it to effectively clear their save slots and reset the display style.

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

HUGE credit to John Dingley, his amazing creation (the DigiGurdy itself), and the original codebase that he wrote.  He worked out the display, the way the crank works, the interaction with the MIDI subsystem... I'm standing on his shoulders.
