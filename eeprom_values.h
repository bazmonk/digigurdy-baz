// This file holds memory addresses within the EEPROM used by digigurdy-baz.
//
// EEPROM memory is rated for at least 100,000 writes (reads are harmless).
// If you end up burning out one of the memory locations, you can shift them around
// by redefining the assingments here.

const int eeprom_hi_mel = 1;
const int eeprom_lo_mel = 2;
const int eeprom_drone = 3;
const int eeprom_tromp = 4;
const int eeprom_tpose = 5;
const int eeprom_capo = 6;
const int eeprom_buzz = 7;
