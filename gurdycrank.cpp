#include "gurdycrank.h"

/// @brief Constructor.
/// @details This class abstracts the cranking mechanism on Digi-Gurdies.
/// 
/// If you are not using an LED buzz indicator, a pin for it still needs to be specified here.  The object won't touch it unless LED_KNOB is defined.
/// @warning This is for optical-sensor cranks.  See GearCrank for a gear-motor crank version.
/// 
/// @warning 
/// * A hidden member object here is a BuzzKnob.
/// * This class' header includes common.h and specific string objects are expected to exist.  See common.h.
/// @param s_pin The analog voltage pin coming from the motor.
/// @param buzz_pin The pin of the buzz knob.
/// @param led_pin The pin of the LED indicator.
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

/// @brief Reports if a crank is connected.
/// @return True
/// @note Currently this method is pointless.  Gear-motor cranks do use this, however, and this is being kept for future changes.
/// It may be useful to expect any crank object to be able to report this in-code whether or not it's relevant...
bool GurdyCrank::isDetected() {
  return true;
};

/// @brief Samples the crank and updates its state
/// @details Also updates the buzz knob, and calls updateExpression().
/// This should be run every loop().  It paces itself internally and expects to be run frequently.
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

/// @brief Updates the expression value and applies it to the strings.
/// @details * Expression is MIDI CC11 which is usually interpreted as a volume adjustment independent of the channel volume.
/// * Here expression is calculated based off EXPRESSION_VMAX and EXPRESSION_START along with the current crank velocity.  See config.h for those values.
/// * The end-user effect is that the volume "swells" as the user cranks faster up to a point.
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
    if (autocrank_toggle_on) {
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

/// @brief Reports whether the crank started spinning this update() cycle.
/// @return True if crank started spinning this cycle, false otherwise
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

/// @brief Reports whether the crank stopped spinning this update() cycle.
/// @return True if crank stopped spinning this cycle, false otherwise
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

/// @brief Reports whether the crank is currently spinning this update() cycle.
/// @return True if crank is spinning, false otherwise.
bool GurdyCrank::isSpinning() {
  return (v_avg > V_THRESHOLD);
};

/// @brief Reports whether buzzing began this update() cycle.
/// @return True if buzzing started thie cycle, false otherwise.
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

/// @brief Reports whether buzzing stopped this update() cycle.
/// @return True if buzzing stopped thie cycle, false otherwise.
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

/// @brief Returns the crank's current (heavily-adjusted) velocity.
/// @return The crank's measured current average velocity in estimated RPMs.
double GurdyCrank::getVAvg() {
  return v_avg;
};

/// @brief Returns the number of detected spoke transitions.
/// @return The number of detected off->on and on->off events.
/// @note This is for debug purposes to see how well it tracks.  Not part of the core logic.
int GurdyCrank::getCount() {
  return trans_count;
};

/// @brief Returns the number of estimated crank revolutions.
/// @return The number of estimated crank revolutions.
/// @note This is for testing purposes and not part of the core logic.
double GurdyCrank::getRev() {
  return rev_count;
};

/// @brief Disables the buzz LED indicator object.
/// @note This does nothing if LED_KNOB is not defined.
void GurdyCrank::disableLED() {
  #ifdef LED_KNOB
  myLED->disable();
  #endif
};

/// @brief Enables the buzz LED indicator object.
/// @note This does nothing if LED_KNOB is not defined.
void GurdyCrank::enableLED() {
  #ifdef LED_KNOB
  myLED->enable();
  #endif
};
