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
/// @param s_pin The digital pin coming from the optical sensor.
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
  buzz_expression = 0;
  the_buzz_timer = 0;
};

/// @brief Constructor (2-pin encoders).
/// @param s_pin A pin (digital, interrupt-capable)
/// @param s_pin2 B pin (digital, interrupt-capable)
/// @param buzz_pin The pin of the buzz knob.
/// @param led_pin The pin of the LED indicator.
/// @version *New in 2.9.5*
GurdyCrank::GurdyCrank(int s_pin, int s_pin2, int buzz_pin, int led_pin) {

  myKnob = new BuzzKnob(buzz_pin);

  #ifdef LED_KNOB
    myLED = new SimpleLED(led_pin);
  #endif

  #ifdef USE_ENCODER
  // This automatically enabled INPUT_PULLUP, FYI.
  myEnc = new Encoder(s_pin2, s_pin);
  last_event_timer = 0;

  last_pulse = 0;

  angle = 0;
  #endif

  expression = 0;
  buzz_expression = 0;
  the_buzz_timer = 0;
  clockwise = true;
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

  #ifdef USE_ENCODER


  if (eval_timer > 30000) {
    cur_vel = (cur_vel) / 2.0;
    eval_timer = 0;
    return;
  };
  

  if (eval_timer > 10000) {
    pulse = myEnc->read();

    if (clockwise && pulse < last_pulse) {
      clockwise = false;
      cur_vel = -5;

      last_pulse = pulse;
      last_event_timer = 0;
      eval_timer = 0;
    } else if (!clockwise && pulse > last_pulse) {
      clockwise = true;
      cur_vel = 0;

      last_pulse = pulse;
      last_event_timer = 0;
      eval_timer = 0;
    }
    
    if (last_pulse != pulse) {
      
      new_vel = (abs(last_pulse - pulse) * 30000000.0) / (NUM_SPOKES * last_event_timer);

      //cur_vel = cur_vel + (0.78 * (new_vel - cur_vel));    //Rolling mean
      cur_vel = new_vel;     // No rolling mean, gives subjective fastest response. Not much difference but still works fine.
      last_pulse = pulse;
      last_event_timer = 0;
      eval_timer = 0;
    };
  } 
  
  

  #else
  if (eval_timer > 25250 && num_events > 1) {
    double new_vel = (num_events * spoke_width * 60000000.0) / (last_event);
//    cur_vel = cur_vel + (smoothing_factor * (new_vel - cur_vel)) + 1;
    if (new_vel > cur_vel) {
      cur_vel = cur_vel + (0.8 * (new_vel - cur_vel)) + 0.5;
    }
    else {
      cur_vel = cur_vel + (0.75 * (new_vel - cur_vel));
    }
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
  #endif

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

    int new_buzz_expression = int(((cur_v - myKnob->getThreshold())/(0.45 * myKnob->getThreshold())) * (42) + 85);
    if (new_buzz_expression > 127) {
      new_buzz_expression = 127;
    };
    
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
    };

    if (buzz_expression != new_buzz_expression) {
      buzz_expression = new_buzz_expression;
      mybuzz->setExpression(buzz_expression);
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

// Basils default was  * 0.95
       // Comment JD   If the value here is higher, close to 1, what you find is that you can get double buzzes. If you let the value drop a little before it turns off, it is then
       // not immediately hovering on the cusp of turning the buzz on again, you have to accelerate the crank a little to get the next buzz
       // You can hide double buzzes by making the minimum buzz duration longer, but if you make that too long, you struggle to get 4 buzzes per turn as it has not turned off 
       // by the time you need the next buzz.
       // So balancing several factors here
bool GurdyCrank::stoppedBuzzing() {
  if (getVAvg() <= (myKnob->getThreshold() * 0.85)) {
    if (was_buzzing && the_buzz_timer > 50) {
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

#ifdef USE_ENCODER
int GurdyCrank::getAngle() {
  return int(pulse * 0.15) % 360;
}

void GurdyCrank::resetAngle() {
  last_pulse = 0;
  myEnc->readAndReset();
}
#endif
