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

            if (done == false) {
                if (currentMillis - previousMillis >= interval) {
                    done = true;
                    return true;
                }
            } else {
                return false;
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