#include "gurdycrank.h"

// class GurdyCrank controls the cranking mechanism, including the buzz triggers.
//   * This version is for optical (IR gate) sensors.  The digital pin readings are expected to
//     oscillate between 0 and 1 only.
//   * NUM_SPOKES in config.h needs to be defined as the number of "spokes" (dark lines) on your
//     wheel, not the number of dark+light bars.  Your RPMs will be half-speed if you do that.

// s_pin is the out pin of the optical sensor.  This is pin 15 (same as analog A1)
// on a normal didigurdy.  buzz_pin is the out pin of the buzz pot, usually A2 (a.k.a 16).
GurdyCrank::GurdyCrank(int s_pin, int buzz_pin, int led_pin) {

  myKnob = new BuzzKnob(buzz_pin);

  #ifdef LED_KNOB
    myLED = new SimpleLED(led_pin);
  #endif

  sensor_pin = s_pin;
  pinMode(sensor_pin, INPUT_PULLUP);
  last_event = digitalRead(sensor_pin);
  expression = 0;
};

// This had a use, and may just go away...
bool GurdyCrank::isDetected() {
  return true;
};

// This is meant to be run every loop().
void GurdyCrank::update() {

  // Check if we need to update the knob reading...
  myKnob->update();

  // Check as close to the sample rate as possible...
  if (the_timer > SAMPLE_RATE) {
    this_event = digitalRead(sensor_pin);

    // If there was a transition on the wheel:
    if (this_event != last_event) {
      last_event = this_event;

      trans_count += 1;
      rev_count = trans_count / (NUM_SPOKES * 2);

      v_6 = v_5;
      v_5 = v_4;
      v_4 = v_3;
      v_3 = v_2;
      v_2 = v_smooth;

      // Velocity is converted to rpm (no particular reason except it's easy to imagine how fast
      // this is in real-world terms.  1 crank per sec is 60rpm, 3 seconds per crank is 20rpm...
      //
      // On nearly every wheel, spokes and holes won't be the same width.  So we average the raw
      // reading of this transition with the last as we go, as a preliminary smoothing technique.
      v_last = v_inst;
      v_inst = (spoke_width * 60000000.0) / (the_spoke_timer);

      //v_smooth = (v_inst + v_last) / 2.0;
      v_smooth = v_inst;

      v_avg = (v_smooth + v_2 + v_3 + v_4 + v_5 + v_6) / 6.0;

      // Rotate our last times
      lt_8 = lt_7;
      lt_7 = lt_6;
      lt_6 = lt_5;
      lt_5 = lt_4;
      lt_4 = lt_3;
      lt_3 = lt_2;
      lt_2 = lt_1;
      lt_1 = the_spoke_timer;

      // Calculate the average of the last four times between readings
      lt_avg = (lt_1 + lt_2 + lt_3 + lt_4 + lt_5 + lt_6 + lt_7 + lt_8) / 8.0;

      // Calculate the standard deviation of the last four times.
      lt_stdev = sqrt((pow((lt_1 - lt_avg), 2) + pow((lt_2 - lt_avg), 2) + pow((lt_3 - lt_avg), 2) + pow((lt_4 - lt_avg), 2) + pow((lt_5 - lt_avg), 2) + pow((lt_6 - lt_avg), 2) + pow((lt_7 - lt_avg), 2) + pow((lt_8 - lt_avg), 2)) / 8.0);

      the_stop_timer = 0;
      the_spoke_timer = 0;

    } else if (the_stop_timer > (lt_avg + (lt_stdev * 3.0)) || the_stop_timer > MAX_WAIT_TIME) {

      v_6 = v_5;
      v_5 = v_4;
      v_4 = v_3;
      v_3 = v_2;
      v_2 = v_smooth;

      v_smooth = v_smooth * DECAY_FACTOR;
      v_avg = (v_smooth + v_2 + v_3 + v_4 + v_5 + v_6) / 6.0;


      // Rotate our last times
      lt_8 = lt_7;
      lt_7 = lt_6;
      lt_6 = lt_5;
      lt_5 = lt_4;
      lt_4 = lt_3;
      lt_3 = lt_2;
      lt_2 = lt_1;
      lt_1 = the_spoke_timer;

      // Calculate the average of the last four times between readings
      lt_avg = (lt_1 + lt_2 + lt_3 + lt_4 + lt_5 + lt_6 + lt_7 + lt_8) / 8.0;

      // Calculate the standard deviation of the last four times.
      lt_stdev = sqrt((pow((lt_1 - lt_avg), 2) + pow((lt_2 - lt_avg), 2) + pow((lt_3 - lt_avg), 2) + pow((lt_4 - lt_avg), 2) + pow((lt_5 - lt_avg), 2) + pow((lt_6 - lt_avg), 2) + pow((lt_7 - lt_avg), 2) + pow((lt_8 - lt_avg), 2)) / 8.0);

      the_stop_timer = 0;
    }

    the_timer = 0;
  };

  updateExpression();
};

void GurdyCrank::updateExpression() {
  // Only do anything every 50ms (20x/sec)
  if (the_expression_timer > 50) {

    float cur_v = getVAvg();
    if (cur_v > EXPRESSION_VMAX) {
      cur_v = EXPRESSION_VMAX;
    } else if (cur_v < V_THRESHOLD) {
      cur_v = V_THRESHOLD;
    }
    int new_expression = int(((cur_v - V_THRESHOLD)/(EXPRESSION_VMAX - V_THRESHOLD)) * (127 - EXPRESSION_START) + EXPRESSION_START);
    if (bigbutton->toggleOn()) {
      new_expression = 90;
    };

    if (expression != new_expression) {
      //Serial.println("EXPRESS LESS");
      expression = new_expression;
      mystring->setExpression(expression);
      mylowstring->setExpression(expression);
      mytromp->setExpression(expression);
      mydrone->setExpression(expression);
      mybuzz->setExpression(expression);
    };
    the_expression_timer = 0;
  };
};

bool GurdyCrank::startedSpinning() {
  if (isSpinning()) {
    if (!was_spinning) {
      was_spinning = true;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
};

bool GurdyCrank::stoppedSpinning() {
  if (!isSpinning()) {
    if (was_spinning) {
      was_spinning = false;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
};

bool GurdyCrank::isSpinning() {
  return (v_avg > V_THRESHOLD);
};

bool GurdyCrank::startedBuzzing() {
  if (getVAvg() > myKnob->getThreshold()) {
    if (!was_buzzing) {
      was_buzzing = true;
      the_buzz_timer = 0;

      #ifdef LED_KNOB
        myLED->on();
      #endif

      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
};

bool GurdyCrank::stoppedBuzzing() {
  if (getVAvg() <= myKnob->getThreshold() && the_buzz_timer > BUZZ_MIN) {
    if (was_buzzing) {
      was_buzzing = false;

      #ifdef LED_KNOB
        myLED->off();
      #endif

      return true;
    } else {
      return false;
    }
  };
  return false;
};

double GurdyCrank::getVAvg() {
  return v_avg;
};

int GurdyCrank::getCount() {
  return trans_count;
};

double GurdyCrank::getRev() {
  return rev_count;
};

void GurdyCrank::disableLED() {
  #ifdef LED_KNOB
  myLED->disable();
  #endif
};

void GurdyCrank::enableLED() {
  #ifdef LED_KNOB
  myLED->enable();
  #endif
};
