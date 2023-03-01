#ifndef EX_SCREENS_H
#define EX_SCREENS_H

#include "common.h"
#include "config.h"
#include "pause_screens.h"

extern ExButton *ex1Button;
extern ExButton *ex2Button;
extern ExButton *ex3Button;
extern ExButton *ex4Button;
extern ExButton *ex5Button;
extern ExButton *ex6Button;

#ifdef REV4_MODE
extern ExButton *ex7Button;
extern ExButton *ex8Button;
extern ExButton *ex9Button;
extern ExButton *ex10Button;
#endif

extern ExButton *bigButton;

void ex_config_screen();
void ex_btn_choice_screen();

#endif
