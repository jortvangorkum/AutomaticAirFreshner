// Create a class to define a blinking led
class Blinking {
    // Variables for which led and how long it is on or off
    int ledPin;
    long onTime;
    long offTime;
    // Variables for the state of led (LOW or HIGH) and how long was the previous currentMillis
    int ledState;
    unsigned long previousMillis;

    public:
        // Constructor to give the variables value
        Blinking(int pin, long on, long off) {
            ledPin = pin;
            pinMode(ledPin, OUTPUT);

            onTime = on;
            offTime = off;

            ledState = LOW;
            previousMillis = 0;
        };
        // Function to set the onTime and offTime different
        void Set(long on, long off) {
            onTime = on;
            offTime = off;
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


};

// Create the leds with the timing and pin
Blinking led1(8, 300, 500);
Blinking led2(13, 350, 350);

void setup() {

}

// Loops throught the update function of the leds
void loop() {
    led1.Update();
    led2.Update();
}
