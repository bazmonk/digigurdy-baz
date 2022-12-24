#include "gearcrank.h"

/// @brief GearCrank controls the cranking mechanism on geared-crank gurdies.
/// @param v_pin The analog pin connected to the crank
/// @param buzz_pin The analog pin of the buzz knob potentiometer
/// @warning A BuzzKnob object using buzz_pin is a hidden private member object of GearCrank.
GearCrank::GearCrank(int v_pin, int buzz_pin) {

  myKnob = new BuzzKnob(buzz_pin);

  voltage_pin = v_pin;
  pinMode(voltage_pin, INPUT);

  crank_voltage = 0;
  spin = 0;
  started_spinning = false;
  stopped_spinning = false;
  is_spinning = false;

  started_buzzing = false;
  stopped_buzzing = false;
  is_buzzing = false;
  buzz_countdown = BUZZ_SMOOTHING;
};

/// @brief Begins ADC sampling of the crank's voltage pin.
/// @details HIGH_SPEED continuous sampling is being used here.  This is very fast (readings are available on-deman, no delay), but ties up one of the ADCs full-time.
/// @warning This is hard-coded to use adc0.
void GearCrank::beginPolling() {
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  adc->adc0->startContinuous(voltage_pin);
};

// Crank detection - this comes from John's code.  We sample the voltage
// of the crank's voltage pin 500 times really quick (100/s for 5s).
// With that, we calculate the standard deviation of the results.
//
// My understanding is this: if the motor is connected and at rest, it
// will give a consistent very-low voltage.  If the pin is not connected
// to anything, its voltage will wander around.  If the results are less than
// 10 stDev from the mean, we consdier it detected.
//
// Moving the crank during the detection *does* throw it off.  Don't do that.
/// @brief Determines if the crank is connected or not.
/// @details This takes advantage of the fact that analog pin voltage "wanders" without some resistance on it.
/// Calculating the standard deviation of a series of readings determines if crank is conntected/attached.
/// @warning Physically moving the crank can create false negative detections.
void GearCrank::detect() {

  sample_sum = 0;
  sample_mean = 0;
  squared_sum = 0;
  deviations = 0;

  // Read the crank 500 times real quick.
  for (int i = 0; i < num_samples; i++) {
    samples[i] = adc->adc0->analogReadContinuous();
    sample_sum += samples[i];
    delay(2);
  };

  // Get the average voltage
  sample_mean = sample_sum / float(num_samples);
  Serial.print("Detection average voltage: ");
  Serial.println(sample_mean);

  // We need the sum of the square of the difference of each value now.
  for (int i = 0; i < num_samples; i++) {
    squared_sum += pow((sample_mean - float(samples[i])), 2);
  };

  // The square root of the average of *that* is the stardard devitaion.
  deviations = sqrt(squared_sum / float(num_samples));
};

/// @brief Reports whether or not there is a crank attached.
/// @return True if detected, false if not detected
bool GearCrank::isDetected() {
  return (deviations < 10);
  //return true;
};

/// @brief 
void GearCrank::refreshBuzz() {
  if (isDetected()) {

    // Buzzing happens if the crank generates more voltage than the
    // adjusted voltage from the knob.  But this is too jittery.  Instead, we take many rapid
    // readings, average that, and then use that.  Even smooth, the motors most digigurdies use
    // are indexed and don't generate consistent voltage.  So we employ two weighted counters
    // that increase rapidly if voltage is high and then decrese more slowly, and use *those*
    // to actually determine whether or not to make cranking/buzzing sound.
    if (crank_voltage > myKnob->getVoltage()) {
      buzz_countdown = BUZZ_SMOOTHING;
    } else if (buzz_countdown > 0) {
      buzz_countdown -= BUZZ_DECAY;
    };

    if (buzz_countdown > 0) {

      // If we weren't buzzing before this, we just started.
      if (!is_buzzing) {
        started_buzzing = true;
      };

      // If we were already buzzing and started_buzzing last cycle,
      // we didn't just start buzzing anymore.
      if (started_buzzing && is_buzzing) {
        started_buzzing = false;
      };

      // Now that we checked, we can update this...
      is_buzzing = true;

    } else {

      // If we were buzzing before, we just stopped.
      if (is_buzzing) {
        stopped_buzzing = true;
      };

      // If we stopped buzzing last cycle, we didn't just stop
      // anymore.
      if (stopped_buzzing && !is_buzzing) {
        stopped_buzzing = false;
      };

      // Now that we're done checking if we *were* buzzing, we can set this.
      is_buzzing = false;
    };

  // If the crank isn't *connected*, the pin will report phantom buzzing,
  // so if the crank isn't *detected*, don't buzz at all:
  } else {
    started_buzzing = false;
    is_buzzing = false;
    stopped_buzzing = false;
  };
};

// This is meant to be run every loop().
void GearCrank::update() {
  if (isDetected()) {
    // Update the knob first.
    myKnob->update();
    refreshBuzz();

    // Poll the crank voltage a few thousand times real quick...
    for (int x = 0; x < SPIN_SAMPLES; x++) {
      sample_total += adc->adc0->analogReadContinuous();
    };

    Serial.print("SPIN_SAMPLES: ");
    Serial.print(SPIN_SAMPLES);

    Serial.print("Sampled: ");
    Serial.print((sample_total / SPIN_SAMPLES));

    // The voltage reading we're using is the average of those.
    crank_voltage = ((sample_total / SPIN_SAMPLES) + (crank_voltage)) / 2;
    sample_total = 0;

    Serial.print(" Smoothed: ");
    Serial.print(crank_voltage);

    crank_voltage = crank_voltage - int(sample_mean);

    Serial.print(" Adjusted: ");
    Serial.print(crank_voltage);

    Serial.print("  Buzz: ");
    Serial.println(myKnob->getVoltage());

    // Based on that voltage, we either bump up the spin by the SPIN_WEIGHT,
    // or we let it decay.
    if (crank_voltage > VOL_THRESHOLD) {
      spin += SPIN_WEIGHT;
      if (spin > MAX_SPIN) {
        spin = MAX_SPIN;
      };
    } else {
      spin -= SPIN_DECAY;
      if (spin < 0) {
        spin = 0;
      };
    };

    // The crank is considered spinning if the spin is over SPIN_THRESHOLD.
    if (spin > SPIN_THRESHOLD) {

      // If we weren't spinning before this, we just started.
      if (!is_spinning) {
        started_spinning = true;
      };

      // If we were already spinning and started_spinning last cycle,
      // we didn't just start spinning anymore.
      if (started_spinning && is_spinning) {
        started_spinning = false;
      };

      // Now that we checked, we can update this...
      is_spinning = true;

    } else if (spin < SPIN_STOP_THRESHOLD) {

      // If we were spinning before, we just stopped.
      if (is_spinning) {
        stopped_spinning = true;
      };

      // If we stopped spinning last cycle, we didn't just stop
      // anymore.
      if (stopped_spinning && !is_spinning) {
        stopped_spinning = false;
      };

      is_spinning = false;
    };

  // If the crank wasn't detected, it acts like a crank that never gets spun.
  } else {
    is_spinning = false;
    started_spinning = false;
    stopped_spinning = false;
  };
};

bool GearCrank::startedSpinning() {
  return started_spinning;
};

bool GearCrank::stoppedSpinning() {
  return stopped_spinning;
};

bool GearCrank::isSpinning() {
  return is_spinning;
};

bool GearCrank::startedBuzzing() {
  return started_buzzing;
};

bool GearCrank::stoppedBuzzing() {
  return stopped_buzzing;
};
