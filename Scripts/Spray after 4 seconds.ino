void setup() {
    pinMode(4, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(4, LOW);
    digitalWrite(13, LOW);
    delay(4000);
    digitalWrite(4, HIGH);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    digitalWrite(13, LOW);
}

void loop() {

}