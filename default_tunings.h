// For reference, these are the values we're defining for each of these

// int HI_MEL = high chanter open note
// int LO_MEL = low chanter open note
// int DRONE = drone note
// int TROMP = trompette note
// int BUZZ = buzz note
// int TPOSE = transpose offset (-12 < offset < 12)
// int CAPO = 5; capo offset (0, 2, or 4)

// Feel free to adjust these if you want other presets hard-coded in.
// I'm using what John's code offered.

// The *_NAME strings have a max length of 17 characters to fit on the screen.

// "G/C G Drones" and I'm tweaking the buzz
static String PRESET1_NAME = "G/C, G Drones";
static const int PRESET1[] = {Note(g4), Note(g3), Note(g2), Note(g3), Note(g4), 0, 0};

// "G/C C Drones"
static String PRESET2_NAME = "G/C, C Drones";
static const int PRESET2[] = {Note(g4), Note(g3), Note(c2), Note(c4), Note(c4), 0, 0};

// "D/G D Drones"
static String PRESET3_NAME = "D/G, D Drones";
static const int PRESET3[] = {Note(d5), Note(d4), Note(d3), Note(d4), Note(d4), 0, 0};

// "D/G G Drones"
static String PRESET4_NAME = "D/G, G Drones";
static const int PRESET4[] = {Note(d5), Note(d4), Note(g2), Note(d4), Note(d4), 0, 0};
