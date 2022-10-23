#ifndef TUNING_SCREENS_H
#define TUNING_SCREENS_H

#include <Arduino.h>
#include "config.h"
#include "common.h"
#include "display.h"
#include "notes.h"

extern KeyboxButton *myAButton;
extern KeyboxButton *myXButton;
extern KeyboxButton *my1Button;
extern KeyboxButton *my2Button;
extern KeyboxButton *my3Button;
extern KeyboxButton *my4Button;
extern KeyboxButton *my5Button;
extern KeyboxButton *my6Button;

extern GurdyString *mystring;
extern GurdyString *mylowstring;
extern GurdyString *mykeyclick;
extern GurdyString *mytromp;
extern GurdyString *mydrone;
extern GurdyString *mybuzz;

extern GurdyButton *tpose_up;
extern GurdyButton *tpose_down;
extern GurdyButton *capo;

extern GurdyButton *ex1Button;
extern GurdyButton *ex2Button;
extern GurdyButton *ex3Button;

extern bool gc_or_dg;

bool tuning();
void manual_tuning_screen();
void volume_screen();
void tuning_hi_melody();
void tuning_low_melody();
void tuning_drone();
void tuning_tromp();
void tune_string_screen(GurdyString *this_string);
void change_volume_screen(GurdyString *this_string);

#endif
