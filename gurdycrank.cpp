#include "gurdycrank.h"

void myisr() {
  if (debounce_timer >= 1250) {
    num_events = num_events + 1;
    last_event = last_event_timer;
    debounce_timer = 0;
  }
  
}

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
  attachInterrupt(digitalPinToInterrupt(sensor_pin), myisr, RISING);

  expression = 0;
  the_buzz_timer = 0;
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

  if (eval_timer > 25250 && num_events > 1) {
    double new_vel = (num_events * spoke_width * 60000000.0) / (last_event);
//    cur_vel = cur_vel + (smoothing_factor * (new_vel - cur_vel)) + 1;
    cur_vel = (cur_vel + new_vel) / 2.0 + (0.3 * (new_vel - cur_vel));
    num_events = 0;
    last_event = 0;
    last_event_timer = 0;
    eval_timer = 0;
  }
  if (eval_timer > 31250 && num_events < 2) {
//    cur_vel = cur_vel + (smoothing_factor * ( 0 - cur_vel)) - 2;
    cur_vel = (cur_vel) / 2.0;
    
    num_events = 0;
    last_event = 0;
    last_event_timer = 0;
    eval_timer = 0;
  }

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
  return (cur_vel > V_THRESHOLD);
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
  return cur_vel;
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
