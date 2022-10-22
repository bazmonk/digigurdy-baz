#ifndef NOTES_H
#define NOTES_H

// enum Note maps absolute note names to MIDI note numbers (middle C4 = 60),
// which range from 0 to 127.
//
// This lets us specify MIDI notes by their name instead of having to refer to a table.
enum Note {
  c_1, c_1s, d_1, d_1s, e_1, f_1, f_1s, g_1, g_1s, a_1, a_1s, b_1,
  c0, c0s, d0, d0s, e0, f0, f0s, g0, g0s, a0, a0s, b0,
  c1, c1s, d1, d1s, e1, f1, f1s, g1, g1s, a1, a1s, b1,
  c2, c2s, d2, d2s, e2, f2, f2s, g2, g2s, a2, a2s, b2,
  c3, c3s, d3, d3s, e3, f3, f3s, g3, g3s, a3, a3s, b3,
  c4, c4s, d4, d4s, e4, f4, f4s, g4, g4s, a4, a4s, b4,
  c5, c5s, d5, d5s, e5, f5, f5s, g5, g5s, a5, a5s, b5,
  c6, c6s, d6, d6s, e6, f6, f6s, g6, g6s, a6, a6s, b6,
  c7, c7s, d7, d7s, e7, f7, f7s, g7, g7s, a7, a7s, b7,
  c8, c8s, d8, d8s, e8, f8, f8s, g8, g8s, a8, a8s, b8,
  c9, c9s, d9, d9s, e9, f9, f9s, g9
};

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

#endif
