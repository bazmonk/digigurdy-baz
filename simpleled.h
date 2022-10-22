#ifndef SIMPLELED_H
#define SIMPLELED_H

// class SimpleLED is for controlling LED lights wired up to a given pin on the Teensy (and ground)
class SimpleLED {
  private:
    int led_pin;

  public:
    SimpleLED(int pin);

    void on();
    void off();
};

#endif
