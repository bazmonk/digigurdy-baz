#include "notes.h"

// string array NoteNum is the reverse of the above Note enum.  It maps MIDI note numbers to
// screen-friendly note names.
//
// This lets us recall string names for printing on the screen without having to refer to a table.
String NoteNumABC[] = {
  "C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "#F-1", "G-1", "G#-1", "A-1", "A#-1", "B-1",
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

String NoteNumDRM[] = {
  "DO-1", "DO#-1", "RE-1", "MIb-1", "MI-1", "FA-1", "#FA-1", "SOL-1", "SOL#-1", "LA-1", "LA-1", "SI-1",
  "DO0", "DO#0", "RE0", "MIb0", "MI0", "FA0", "FA#0", "SOL0", "SOL#0", "LA0", "SIb0", "SI0",
  "DO1", "DO#1", "RE1", "MIb1", "MI1", "FA1", "FA#1", "SOL1", "SOL#1", "LA1", "SIb1", "SI1",
  "DO2", "DO#2", "RE2", "MIb2", "MI2", "FA2", "FA#2", "SOL2", "SOL#2", "LA2", "SIb2", "SI2",
  "DO3", "DO#3", "RE3", "MIb3", "MI3", "FA3", "FA#3", "SOL3", "SOL#3", "LA3", "SIb3", "SI3",
  "DO4", "DO#4", "RE4", "MIb4", "MI4", "FA4", "FA#4", "SOL4", "SOL#4", "LA4", "SIb4", "SI4",
  "DO5", "DO#5", "RE5", "MIb5", "MI5", "FA5", "FA#5", "SOL5", "SOL#5", "LA5", "SIb5", "SI5",
  "DO6", "DO#6", "RE6", "MIb6", "MI6", "FA6", "FA#6", "SOL6", "SOL#6", "LA6", "SIb6", "SI6",
  "DO7", "DO#7", "RE7", "MIb7", "MI7", "FA7", "FA#7", "SOL7", "SOL#7", "LA7", "SIb7", "SI7",
  "DO8", "DO#8", "RE8", "MIb8", "MI8", "FA8", "FA#8", "SOL8", "SOL#8", "LA8", "SIb8", "SI8",
  "DO9", "DO#9", "RE9", "MIb9", "MI9", "FA9", "FA#9", "SOL9"
};

String NoteNumCOMBO[] = {
  "DO-1", "DO#-1", "RE-1", "MIb-1", "MI-1", "FA-1", "#FA-1", "SOL-1", "SOL#-1", "LA-1", "LA-1", "SI-1",
  "DO0", "DO#0", "RE0", "MIb0", "MI0", "FA0", "FA#0", "SOL0", "SOL#0", "LA0", "SIb0", "SI0",
  "DO1", "DO#1", "RE1", "MIb1", "MI1", "FA1", "FA#1", "SOL1", "SOL#1", "LA1", "SIb1", "SI1",
  "DO2", "DO#2", "RE2", "MIb2", "MI2", "FA2", "FA#2", "SOL2", "SOL#2", "LA2", "SIb2", "SI2",
  "DO3", "DO#3", "RE3", "MIb3", "MI3", "FA3", "FA#3", "SOL3", "SOL#3", "LA3", "SIb3", "SI3",
  "DO4", "DO#4", "RE4", "MIb4", "MI4", "FA4", "FA#4", "SOL4", "SOL#4", "LA4", "SIb4", "SI4",
  "DO5", "DO#5", "RE5", "MIb5", "MI5", "FA5", "FA#5", "SOL5", "SOL#5", "LA5", "SIb5", "SI5",
  "DO6", "DO#6", "RE6", "MIb6", "MI6", "FA6", "FA#6", "SOL6", "SOL#6", "LA6", "SIb6", "SI6",
  "DO7", "DO#7", "RE7", "MIb7", "MI7", "FA7", "FA#7", "SOL7", "SOL#7", "LA7", "SIb7", "SI7",
  "DO8", "DO#8", "RE8", "MIb8", "MI8", "FA8", "FA#8", "SOL8", "SOL#8", "LA8", "SIb8", "SI8",
  "DO9", "DO#9", "RE9", "MIb9", "MI9", "FA9", "FA#9", "SOL9"
};

// This is a version of the above but with flats listed as well.
String LongNoteNumABC[] = {
  "EMPTY", "C#-1/Db-1", "D-1", "D#-1/Eb-1", "E-1", "F-1", "F#-1/Gb-1", "G-1", "G#-1/Ab-1", "A-1", "A#-1/Bb-1", "B-1",
  "C0", "C#0/Db0", "D0", "D#0/Eb0", "E0", "F0", "F#0/Gb0", "G0", "G#0/Ab0", "A0", "A#0/Bb0", "B0",
  "C1", "C#1/Db1", "D1", "D#1/Eb1", "E1", "F1", "F#1/Gb1", "G1", "G#1/Ab1", "A1", "A#1/Bb1", "B1",
  "C2", "C#2/Db2", "D2", "D#2/Eb2", "E2", "F2", "F#2/Gb2", "G2", "G#2/Ab2", "A2", "A#2/Bb2", "B2",
  "C3", "C#3/Db3", "D3", "D#3/Eb3", "E3", "F3", "F#3/Gb3", "G3", "G#3/Ab3", "A3", "A#3/Bb3", "B3",
  "C4", "C#4/Db4", "D4", "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4",
  "C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5",
  "C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6", "G#6/Ab6", "A6", "A#6/Bb6", "B6",
  "C7", "C#7/Db7", "D7", "D#7/Eb7", "E7", "F7", "F#7/Gb7", "G7", "G#7/Ab7", "A7", "A#7/Bb7", "B7",
  "C8", "C#8/Db8", "D8", "D#8/Eb8", "E8", "F8", "F#8/Gb8", "G8", "G#8/Ab8", "A8", "A#8/Bb8", "B8",
  "C9", "C#9/Db9", "D9", "D#9/Eb9", "E9", "F9", "F#9/Gb9", "G9"
};

String LongNoteNumDRM[] = {
  "EMPTY", "DO#-1/REb-1", "RE-1", "RE#-1/MIb-1", "MI-1", "FA-1", "FA#-1/SOLb-1", "SOL-1", "SOL#-1/LAb-1", "LA-1", "LA#-1/SIb-1", "SI-1",
  "DO0", "DO#0/REb0", "RE0", "RE#0/MIb0", "MI0", "FA0", "FA#0/SOLb0", "SOL0", "SOL#0/LAb0", "LA0", "LA#0/SIb0", "SI0",
  "DO1", "DO#1/REb1", "RE1", "RE#1/MIb1", "MI1", "FA1", "FA#1/SOLb1", "SOL1", "SOL#1/LAb1", "LA1", "LA#1/SIb1", "SI1",
  "DO2", "DO#2/REb2", "RE2", "RE#2/MIb2", "MI2", "FA2", "FA#2/SOLb2", "SOL2", "SOL#2/LAb2", "LA2", "LA#2/SIb2", "SI2",
  "DO3", "DO#3/REb3", "RE3", "RE#3/MIb3", "MI3", "FA3", "FA#3/SOLb3", "SOL3", "SOL#3/LAb3", "LA3", "LA#3/SIb3", "SI3",
  "DO4", "DO#4/REb4", "RE4", "RE#4/MIb4", "MI4", "FA4", "FA#4/SOLb4", "SOL4", "SOL#4/LAb4", "LA4", "LA#4/SIb4", "SI4",
  "DO5", "DO#5/REb5", "RE5", "RE#5/MIb5", "MI5", "FA5", "FA#5/SOLb5", "SOL5", "SOL#5/LAb5", "LA5", "LA#5/SIb5", "SI5",
  "DO6", "DO#6/REb6", "RE6", "RE#6/MIb6", "MI6", "FA6", "FA#6/SOLb6", "SOL6", "SOL#6/LAb6", "LA6", "LA#6/SIb6", "SI6",
  "DO7", "DO#7/REb7", "RE7", "RE#7/MIb7", "MI7", "FA7", "FA#7/SOLb7", "SOL7", "SOL#7/LAb7", "LA7", "LA#7/SIb7", "SI7",
  "DO8", "DO#8/REb8", "RE8", "RE#8/MIb8", "MI8", "FA8", "FA#8/SOLb8", "SOL8", "SOL#8/LAb8", "LA8", "LA#8/SIb8", "SI8",
  "DO9", "DO#9/REb9", "RE9", "RE#9/MIb9", "MI9", "FA9", "FA#9/SOLb9", "SOL9"
};

String LongNoteNumCOMBO[] = {
  "EMPTY", "C#-1/DO#-1", "D-1/RE-1", "Eb-1/MIb-1", "E-1/MI-1", "F-1/FA-1", "F#-1/FA#-1", "G-1/SOL-1", "G#-1/SOL#-1", "A-1/LA-1", "Bb-1/SIb-1", "B-1/SI-1",
  "C0/DO0", "C#0/DO#0", "D0/RE0", "Eb0/MIb0", "E0/MI0", "F0/FA0", "F#0/FA#0", "G0/SOL0", "G#0/SOL#0", "A0/LA0", "Bb0/SIb0", "B0/SI0",
  "C1/DO1", "C#1/DO#1", "D1/RE1", "Eb1/MIb1", "E1/MI1", "F1/FA1", "F#1/FA#1", "G1/SOL1", "G#1/SOL#1", "A1/LA1", "Bb1/SIb1", "B1/SI1",
  "C2/DO2", "C#2/DO#2", "D2/RE2", "Eb2/MIb2", "E2/MI2", "F2/FA2", "F#2/FA#2", "G2/SOL2", "G#2/SOL#2", "A2/LA2", "Bb2/SIb2", "B2/SI2",
  "C3/DO3", "C#3/DO#3", "D3/RE3", "Eb3/MIb3", "E3/MI3", "F3/FA3", "F#3/FA#3", "G3/SOL3", "G#3/SOL#3", "A3/LA3", "Bb3/SIb3", "B3/SI3",
  "C4/DO4", "C#4/DO#4", "D4/RE4", "Eb4/MIb4", "E4/MI4", "F4/FA4", "F#4/FA#4", "G4/SOL4", "G#4/SOL#4", "A4/LA4", "Bb4/SIb4", "B4/SI4",
  "C5/DO5", "C#5/DO#5", "D5/RE5", "Eb5/MIb5", "E5/MI5", "F5/FA5", "F#5/FA#5", "G5/SOL5", "G#5/SOL#5", "A5/LA5", "Bb5/SIb5", "B5/SI5",
  "C6/DO6", "C#6/DO#6", "D6/RE6", "Eb6/MIb6", "E6/MI6", "F6/FA6", "F#6/FA#6", "G6/SOL6", "G#6/SOL#6", "A6/LA6", "Bb6/SIb6", "B6/SI6",
  "C7/DO7", "C#7/DO#7", "D7/RE7", "Eb7/MIb7", "E7/MI7", "F7/FA7", "F#7/FA#7", "G7/SOL7", "G#7/SOL#7", "A7/LA7", "Bb7/SIb7", "B7/SI7",
  "C8/DO8", "C#8/DO#8", "D8/RE8", "Eb8/MIb8", "E8/MI8", "F8/FA8", "F#8/FA#8", "G8/SOL8", "G#8/SOL#8", "A8/LA8", "Bb8/SIb8", "B8/SI8",
  "C9/DO9", "C#9/DO#9", "D9/RE9", "Eb9/MIb9", "E9/MI9", "F9/FA9", "F#9/FA#9", "G9/SOL9"
};

String getNoteNum(int num) {
  if (use_solfege == 0) {
    return NoteNumABC[num];
  } else if (use_solfege == 1) {
    return NoteNumDRM[num];
  } else {
    return NoteNumCOMBO[num];
  };
};

String getLongNoteNum(int num) {
  if (use_solfege == 0) {
    return LongNoteNumABC[num];
  } else if (use_solfege == 1) {
    return LongNoteNumDRM[num];
  } else {
    return LongNoteNumCOMBO[num];
  };
};
