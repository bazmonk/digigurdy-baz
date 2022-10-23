#include "notes.h"

// string array NoteNum is the reverse of the above Note enum.  It maps MIDI note numbers to
// screen-friendly note names.
//
// This lets us recall string names for printing on the screen without having to refer to a table.
std::string NoteNum[] = {
  "C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "#F-1", "G-1", "G#-1", "A-1", "#A-1", "B-1",
  "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
  "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
  "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
  "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
  "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
  "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
  "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8",
  "C9", "C#9", "D9", "D#9", "E9", "F9", "F#9", "G9"
};

// This is a version of the above but with flats listed as well.
std::string LongNoteNum[] = {
  " EMPTY ", "C#-1/Db-1", "  D-1  ", "D#-1/Eb-1", "  E-1  ", "  F-1  ", "F#-1/Gb-1", "  G-1  ", "G#-1/Ab-1", "  A-1  ", "A#-1/Bb-1", "  B-1  ",
  "  C0   ", "C#0/Db0", "  D0   ", "D#0/Eb0", "  E0   ", "  F0   ", "F#0/Gb0", "  G0   ", "G#0/Ab0", "  A0   ", "A#0/Bb0", "  B0   ",
  "  C1   ", "C#1/Db1", "  D1   ", "D#1/Eb1", "  E1   ", "  F1   ", "F#1/Gb1", "  G1   ", "G#1/Ab1", "  A1   ", "A#1/Bb1", "  B1   ",
  "  C2   ", "C#2/Db2", "  D2   ", "D#2/Eb2", "  E2   ", "  F2   ", "F#2/Gb2", "  G2   ", "G#2/Ab2", "  A2   ", "A#2/Bb2", "  B2   ",
  "  C3   ", "C#3/Db3", "  D3   ", "D#3/Eb3", "  E3   ", "  F3   ", "F#3/Gb3", "  G3   ", "G#3/Ab3", "  A3   ", "A#3/Bb3", "  B3   ",
  "  C4   ", "C#4/Db4", "  D4   ", "D#4/Eb4", "  E4   ", "  F4   ", "F#4/Gb4", "  G4   ", "G#4/Ab4", "  A4   ", "A#4/Bb4", "  B4   ",
  "  C5   ", "C#5/Db5", "  D5   ", "D#5/Eb5", "  E5   ", "  F5   ", "F#5/Gb5", "  G5   ", "G#5/Ab5", "  A5   ", "A#5/Bb5", "  B5   ",
  "  C6   ", "C#6/Db6", "  D6   ", "D#6/Eb6", "  E6   ", "  F6   ", "F#6/Gb6", "  G6   ", "G#6/Ab6", "  A6   ", "A#6/Bb6", "  B6   ",
  "  C7   ", "C#7/Db7", "  D7   ", "D#7/Eb7", "  E7   ", "  F7   ", "F#7/Gb7", "  G7   ", "G#7/Ab7", "  A7   ", "A#7/Bb7", "  B7   ",
  "  C8   ", "C#8/Db8", "  D8   ", "D#8/Eb8", "  E8   ", "  F8   ", "F#8/Gb8", "  G8   ", "G#8/Ab8", "  A8   ", "A#8/Bb8", "  B8   ",
  "  C9   ", "C#9/Db9", "  D9   ", "D#9/Eb9", "  E9   ", "  F9   ", "F#9/Gb9", "  G9   "
};
