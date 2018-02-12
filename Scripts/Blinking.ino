class Blinking {
    int ledPin;
    long onTime;
    long offTime;

    int ledState;
    unsigned long previousMillis;

    public:
    Blinking(int pin, long on, long off) {
        ledPin = pin;
        pinMode(ledPin, OUTPUT);

        onTime = on;
        offTime = off;

        ledState = LOW;
        previousMillis = 0;
    }

    void Update() {
        unsigned long currentMillis = millis();

        if ((ledState == HIGH) && (currentMillis - previousMillis >= onTime)) {
            ledState = LOW;
            previousMillis = currentMillis;
            digitalWrite(ledPin, ledState);
        } else if ((ledState == LOW) && (currentMillis - previousMillis >= offTime)) {
            ledState = HIGH;
            previousMillis = currentMillis;
            digitalWrite(ledPin, ledState);
        }
    }
};

Blinking led1(8, 100, 400);
Blinking led2(13, 350, 350);

void setup() {

}

void loop() {
    led1.Update();
    led2.Update();
}