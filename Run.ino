const int ledPin = 8;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long shortInterval = 300;
const long longInterval = 1000;
int timeOffInterval = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    blinkLed(LED_BUILTIN, 500, 500);
    //SOS();
}


void SOS() {
  // Loops 3 times short blink
  for (int t = 0; t < 2; t++) {
    blinkLed(ledPin, shortInterval, timeOffInterval);
  }
  // Loops 3 times long blink
  for (int t = 0; t < 2; t++) {
    blinkLed(ledPin, longInterval, timeOffInterval);
  }
  // Loops 3 times short blink
  for (int t = 0; t < 2; t++) {
    blinkLed(ledPin, shortInterval, timeOffInterval);
  }
  delay(2000);
}

void blinkLed(int ledPin, int firstDelay, int secondDelay) {
  // currentMillis = millis();

  // if (currentMillis - previousMillis >= firstDelay) {
  //   previousMillis = currentMillis;

  //   digitalWrite(ledPin, HIGH);
  //   if (currentMillis - previousMillis >= secondDelay) {
  //     previousMillis = currentMillis;

  //     digitalWrite(ledPin, LOW);
  //   }
  // }

  timerSwitchLedState(ledPin, LOW, firstDelay);
  timerSwitchLedState(ledPin, HIGH, secondDelay);
}

void timerSwitchLedState(int ledPin, int ledState, long interval) {
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    digitalWrite(ledPin, ledState);
  }
}
