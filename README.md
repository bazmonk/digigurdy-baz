# digigurdy-baz
Digigurdy Teensy code

This is a rewrite of the digigurdy code (https://hackaday.io/project/165251-the-digi-gurdy-and-diginerdygurdy).

The code has been significantly streamlined (20% the size of the original source code, compiles to only 25% the teensy3.5's capacity), and there are several new features:

* A re-worked tuning menu, allowing a choice of G/C or D/G tuning, each with a selection (one each, four total) of:
  * 4 high chanterelles (unison and fifths)
  * 4 low chanterelles (unison and fifths)
  * 6 drones
  * 6 trompettes
* The capo button allows **on-the-fly** toggling of 1 or 2 whole-note capos on the drone/trompette strings.
* Two transpose buttons allow **on-the-fly** transposing of all strings up *or* down one octave in half-steps (12 up, 12 down)!
  * Every tuning can be played in *any key* within an octave of it.
* Easy-to-access save/load of all string, transpose and capo settings.
* **On-Screen-Display** showing current string tunings, transpose and capo settings, as well as a read-out of the current note being played on the keybox.

## Installation

Your digigurdy may have an SSD1306 display (the "white OLED"), or the SH1106 display (the "blue OLED").  This code needs to be compiled for the correct display.

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

After that, the steps are the same for the original digigurdy code.  You will need:
  * Teensyduino, the IDE and build environment for the electronics
  * The appropriate `Adafruit_[SSD1306 or SH1106]` library (see the whilte/blue display section above) for your gurdy installed within Teensyduino (`Sketch->Include Library->Manage Libraries...`).
    * If you have the SH1106 "blue" OLED, you need the SH1106 libraries here: https://github.com/wonho-maker/Adafruit_SH1106. Other versions will not work.
  * The main `digigurdy-baz.ino` file along with `bitmaps.h` in a directory/folder named `digigurdy-baz`.  Teensyduino will offer to make this folder and move the file around if you don't.  The easiest way to make sure everything is in the right place is to simply clone this repository from github to your computer.  The repo itself is named the correct way for Teensyduino, and you can easily pull new updates later on if you want.

With that in place, you should be able to open the `digigurdy-baz.ino` file in Teensyduino, and compile/upload it to your digigurdy.  Make sure to select the correct port for it and that the other settings look like this after plugging your gurdy to your computer:

![teensyduino settings](https://raw.githubusercontent.com/wiki/bazmonk/digigurdy-baz/teensyduino.png)

It should automatically restart and be ready-to-go!

## Operation, Setup, and Play

The keys for navigating the menus work just like John Dingley's code.  The bottom row represent number choices 1-6 starting from the left.  THe "OK" or X button is the top right, and the "Back" or O button is the top left:

![keybox pic](https://raw.githubusercontent.com/wiki/bazmonk/digigurdy-baz/keys.jpeg)

### Startup

After showing the Adafruit logo (represent!) and a title screen, crank detection will silently work in the background for about 5 seconds.

* **DO NOT TURN THE CRANK DURING CRANK DETECTION!**  It's ok if you do, but you may fool the digigurdy into thinking there is no crank installed (yes, that does sound backwards, but it's the truth).  If you do this accidentally, just unpower and power back up the digigurdy.  It'll forgive you and forget it ever even happened.

From there, you will be shown the Tuning Menu.  There's quick options here to immediately load the last tuning you used and get straight to playing, or options to choose a new tuning.  

(If you're a little overwhelmed with all the string options, each selection screen has a "default" selection that you can quickly choose with the 'O' button (just mash it to fly through the whole tuning menu if you want).  That will at least give you somewhere to start, and you can experiment from there.)

### Play

Once you select a tuning, you'll get the "Crank On!" screen briefly, and your digigurdy is ready to play!  You can activate the drones and keybox with either the large arcade button on the front of the keybox, or (if you have one connected) the crank.  The knob next to the arcade button adjusts the buzz sensitivity like it did before.

At any time during play (even if you're not acutally making sound right now), you can use the two "extra" (these used to be for octave up/down in the old digigurdy code) buttons on the digigurdy (if you have these buttons... I'm sorry if you don't, there's no way for you to access these features yet) to transpose all of the strings up or down.  You'll see the display show the updated tunings and how far you've transposed it from the default tuning.

The third "extra" button toggles between a whole-note or two-whole-note capo on the drone/trompette strings, much like how a capo would work on a physical hurdy gurdy.  This is in addition to any transposing, letting you apply capos to any of the transposed tunings as you may be already used to on a real gurdy.

### Re-Tuning, Saving

At any time during play, pressing the X and O (top left and right keys) at the same time brings up the Tuning Menu again.  

**If you want to save your transpose/capo settings for later, do this real quick and hit 4) to save them.**  Since you likely applied these after going through the tuning menu before, they need to be saved after you choose them if you want to load them up again after restarting your gurdy.

(For those curious, the EEPROM memory on teensy units are only rated to last "at least 100,000 writes", so I didn't implement a periodic auto-save feature out of an abundance of caution to preserve your unit's memory.  If you want auto-saving and you don't mind burning out your bytes--you do have a few thousand spare bytes--let me know and I'll make it auto-saving.)

## I tried it and I don't like it.  Now what?

This repository includes (in the `other` folder) a recent version of John Dingley's original digigurdy code, as well as a version of it I modified with a different tuning setup.  Follow the same installation instructions above to put the old code back, and thanks for trying it out!

If there's something particular about it that annoys you, feel free to open an issue (see the top of this page) for me to suggest what you'd like me to change, and I'll see if I can accommodate it.

## I found a bug.  

Please open an issue (top of the page here) and describe it for me.  If it's a build problem, I'd like to see the output of Teensyduino (from the bottom panel) when it fails.  

Big bonus points (I'll make a credits screen for you all) if you can reproduce it and tell me how so I can fix it.  Git issues here generate emails for me so it's the preferred way to document them (versus facebook or some other method).

## I want it to do something it doesn't do.

Feel free to open an issue.  I'll see what I can do.  This is all in my spare time so responsiveness is what it is, but I will see your issue pretty quick.

Part of rewriting this was with maintainability and extendability in mind.  The code is well-formatted, I've abstracted the strings, MIDI interaction, buttons, keybox, crank, etc. into easy-to-use classes.  I put a lot of effort into using readable variables, commenting the crap out of the code, and not doing anything needlessly "clever"... a first-year programming student should be able to make sense of it.  

I haven't really thought of how I'll handle a bunch of competing pull requests because I'm not expecting it, but feel free to clone the repo, make improvements, and submit a pull request.  I'm open to them.

## Acknowledgements

HUGE credit to John Dingley, his amazing creation (the DigiGurdy itself), and the original codebase that he wrote.  Everything I've done here is what he did, just written down differently.  
