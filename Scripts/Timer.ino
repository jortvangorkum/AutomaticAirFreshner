class Timer {
    unsigned long previousMillis;
    long interval;
    boolean done;

    public:
        Timer(long inter) {
            interval = inter;

            previousMillis = 0;
            done = false;
        }

        boolean Update() {
            unsigned long currentMillis = millis();

            if (done != true) {
                if (currentMillis - previousMillis >= interval) {
                    done = true;
                    return true;
                }
            } else {
                return false;
            }
        }
};

class Timer {
    unsigned long previousMillis;
    long interval;
    boolean done;
    boolean unlimited;

    public:
        Timer(long inter, boolean unlim) {
            interval = inter;
            unlimited = unlim;

            previousMillis = 0;
            done = false;
        }

        boolean Update() {
            unsigned long currentMillis = millis();

            if (unlimited) {
              if (currentMillis - previousMillis >= interval) {
                return true;
              } else {
                return false;
              }
            } else {
              if (done != true) {
                if (currentMillis - previousMillis >= interval) {
                  done = true;
                  return true;
                }
              } else {
                  return false;
              }
            }
        }
};

Timer timer1(2000);
Timer timer2(4000);

void setup() {
    pinMode(13, OUTPUT);
}

void loop() {
    if (timer1.Update()) {
        digitalWrite(13, HIGH);
    } else if(timer2.Update()) {
        digitalWrite(13, LOW);
    }
}