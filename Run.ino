int ledPin = 8;
int shortTimeOn = 300;
int longTimeOn = 1000;
int timeOff = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    blinkLed(LED_BUILTIN, 1000, 1000);
    SOS();
}


void SOS() {
  // Loops 3 times short blink
  for (int t = 0; t < 2; t++) {ß
    blinkLed(ledPin, shortTimeOn, timeOff);
  }
  // Loops 3 times long blink
  for (int t = 0; t < 2; t++) {
    blinkLed(ledPin, longTimeOn, timeOff);
  }
  // Loops 3 times short blink
  for (int t = 0; t < 2; t++) {
    blinkLed(ledPin, shortTimeOn, timeOff);
  }
  delay(2000);
}

void blinkLed(int ledPin, int firstDelay, int secondDelay) {
  digitalWrite(ledPin, HIGH);
  delay(firstDelay);
  digitalWrite(ledPin, LOW);
  delay(secondDelay);
}
