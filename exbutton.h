#ifndef EXBUTTON_H
#define EXBUTTON_H

#include "gurdybutton.h"
#include "exfunctions.h"
//#include "common.h"

class ExButton: public GurdyButton {
  private:
    int my_func;

  public:
    ExButton(int my_pin, int func = 1);

    int getFunc();

    void setFunc(int func);

    void doFunc();
};

#endif
