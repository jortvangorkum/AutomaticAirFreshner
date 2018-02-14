void setup() {
    pinMode(3, INPUT);
    pinMode(4, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(4, LOW);
    digitalWrite(13, LOW);
}

void loop() {
    int buttonState = digitalRead(3);

    if(buttonState == LOW) {
        digitalWrite(4, HIGH);
        digitalWrite(13, HIGH);
        delay(1000);
    } else {
        digitalWrite(4, LOW);
        digitalWrite(13, LOW);
    }
}