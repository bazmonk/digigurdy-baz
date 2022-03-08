// This file holds memory addresses within the EEPROM used by digigurdy-baz.
//
// EEPROM memory is rated for at least 100,000 writes (reads are harmless).
// If you end up burning out one of the memory locations, you can shift them around
// by redefining the assingments here.

// The slots below should be arranged so that they are at least 20 apart.  This is
// to provide room for future upgrades.  There may be two more channels later, as well
// as a second value (volume) for each channel.
//
// Teensy3.5 has 4096 bytes of EEPROM, so there's lots of room here.
static const int EEPROM_SLOT1 = 0;
static const int EEPROM_SLOT2 = 20;
static const int EEPROM_SLOT3 = 40;
static const int EEPROM_SLOT4 = 60;

// These define what the values are within the "slots".  You'd add this to one of the
// SLOTs above to get the actual address in EEPROM.
const int EEPROM_HI_MEL = 0;
const int EEPROM_HI_MEL = 1;
const int EEPROM_DRONE = 2;
const int EEPROM_TROMP = 3;
const int EEPROM_TPOSE = 4;
const int EEPROM_CAPO = 5;
const int EEPROM_BUZZ = 6;
