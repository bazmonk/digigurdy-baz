// My algorithm for the crank.
int sensor = 15;

elapsedMillis the_time;


// class GurdyCrank controls the cranking mechanism, including the buzz triggers.
class GurdyCrank {
  private:
    int sensor_pin;
    float spoke_width = 1.0/80; // 40 spokes, 80 transitions per revolution
    float v_inst = 0.0;
    float v_last = 0.0;
    float v_avg = 0.0;
    float a_inst = 0.0;
    float a_last = 0.0;
    float a_avg = 0.0;
    int last_event_time;

    bool last_event;
    bool this_event;
    bool has_changed;
    bool was_spinning = false;

  public:
    // s_pin is the out pin of the optical sensor.  This is pin 15 (same as analog A1)
    // on a normal didigurdy.  buzz_pin is the out pin of the buzz pot, usually A2 (a.k.a 16).
    GurdyCrank(int s_pin, int buzz_pin) {

      //myKnob = new BuzzKnob(buzz_pin, adc_obj);

      sensor_pin = s_pin;
      pinMode(sensor_pin, INPUT_PULLUP);
      last_event = digitalRead(sensor_pin);
      last_event_time = the_time;
    };

    bool isDetected() {
      return true;
    };

    // This is meant to be run every loop().
    void update() {
      
      has_changed = false;
      // We want to wait up to 10ms before deciding if we've stopped or not
      int this_time = the_time;
      while ((the_time - this_time < 50) && !has_changed) {
        this_event = digitalRead(sensor_pin);

        // If a transition occurs, continue, otherwise wait a bit and check again
        if (this_event != last_event) {
          has_changed = true;
        } else {
          delay(1);
        }
      }

      // Now after up to 10ms, decide if we're moving.
      if (has_changed) {
        last_event = this_event;
        v_inst = spoke_width * 60000.0 / (the_time - last_event_time); // this is rpm.
        v_avg = (v_inst + v_last) / 2.0;
        a_inst = (v_inst - v_last) / (the_time - last_event_time);
        a_avg = (a_inst + a_last) / 2.0;

        v_last = v_inst;
        a_last = a_inst;
        last_event_time = the_time;
      } else {
        v_inst = 0;
        v_avg = (v_inst + v_last) / 2;
        a_inst = 0;
        a_avg = (a_inst + a_last) / 2;

        v_last = v_inst;
        a_last = a_inst;
      }
    };

    bool startedSpinning() {
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

    bool stoppedSpinning() {
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

    bool isSpinning() {
      Serial.println(v_avg);
      return (v_avg > 1);
    };

    bool startedBuzzing() {
      return false;
    };

    bool stoppedBuzzing() {
      return false;
    };
};

GurdyCrank *mycrank;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");

  mycrank = new GurdyCrank(15, A2);

}



void loop()
{
  mycrank->update();
  if (mycrank->startedSpinning()) {
    Serial.println("PLAY");
  } else if (mycrank->isSpinning()) {
    Serial.println("PLAYING...");
  } else if (mycrank->stoppedSpinning()) {
    Serial.println("STOP");
  }

}  // end of main void loop
