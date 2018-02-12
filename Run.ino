// Create a class to define a blinking led
class Blinking {
    // Variables for which led and how long it is on or off
    int ledPin;
    long onTime;
    long offTime;
    // Variables for the state of led (LOW or HIGH) and how long was the previous currentMillis
    int ledState;
    unsigned long previousMillis;

    // Constructor to give the variables value
    public:
    Blinking(int pin, long on, long off) {
        ledPin = pin;
        pinMode(ledPin, OUTPUT);

        onTime = on;
        offTime = off;

        ledState = LOW;
        previousMillis = 0;
    }

    // Function to update the led to on or off
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

    void UpdateTimer(int on, int off) {
      onTime = on;
      offTime = off;
    }
};

// Create the leds with the timing and pin
Blinking led1(8, 300, 500);
Blinking led2(13, 350, 350);

void setup() {

}

// Loops throught the update function of the leds
void loop() {
    led1.UpdateTimer(300, 500);
    for (int t = 0; t < 2; t++) {
        led1.Update();
    }
    led1.UpdateTimer(1000, 500);
    for (int t = 0; t < 2; t++) {
        led1.Update();
    }
    led1.UpdateTimer(300, 500);
    for (int t = 0; t < 2; t++) {
        led1.Update();
    }
    led2.Update();
}