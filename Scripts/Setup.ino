/*
  Libraries
*/
#include <LiquidCrystal.h>


/*
  Constant variables
*/
// Input pins for LCD screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// Input pins for spray delay
const int buttonPinDelayLow = 9;
const int buttonPinDelayHigh = 10;
/*
  States:
  - notInUse = 0
  - useNumber1 = 1
  - useNumber2 = 2
  - useCleaning = 3
  - useTypeUnknown = 4
  - triggeredShot = 5
  - menuActive = 6
*/
int currentState;
int sprayDelay;
int sprayDelayLast;
// Button for change spray delay;
int buttonStateDelayLow;
int buttonStateDelayLowLast;
int buttonStateDelayHigh;
int buttonStateDelayHighLast;
// Creating LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  /*
    Assign values to variables
  */
  currentState = 0;
  sprayDelay = 0;
  buttonStateDelayLowLast = HIGH;
  buttonStateDelayHighLast = HIGH;
  /*
    Assign Pins to OUTPUT or INPUT
  */
  pinMode(buttonPinDelayLow, INPUT);
  pinMode(buttonPinDelayHigh, INPUT);
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  determineState();
  changeSprayDelay();
  displaySprayDelay();
}

void determineState() {
  notInUse();
  useTypeUnknown();
  useNumber1();
  useNumber2();
  useCleaning();
  triggeredShot();
  menuActive();
}

void notInUse() {

}

void useTypeUnknown() {
  if(currentState == 0) {

  }
}

void useNumber1() {
  if(currentState == 4) {

  }
}

void useNumber2() {
  if(currentState == 1) {

  }
}

void useCleaning() {
  if(currentState == 4) {

  }
}

void triggeredShot() {
  if(currentState == 1
  || currentState == 2
  || currentState == 3
  || currentState == 4) {

  }
}

void menuActive() {
  if(currentState == 1
  || currentState == 2
  || currentState == 3
  || currentState == 4) {

  }
}

void spray(int times) {
  for (int t = 0; t < times; t++) {
    // spray
  }
}

void displayTemperature() {

}

void displaySprayShot() {

}

void changeSprayDelay() {
  buttonStateDelayLow = digitalRead(buttonPinDelayLow);
  buttonStateDelayHigh = digitalRead(buttonPinDelayHigh);

  if (buttonStateDelayLowLast != buttonStateDelayLow && buttonStateDelayLow == LOW) {
    if (sprayDelay > 0) {
      sprayDelay -= 1;
    }
  } else if (buttonStateDelayHighLast != buttonStateDelayHigh && buttonStateDelayHigh == LOW) {
    sprayDelay += 1;
  }

  buttonStateDelayLowLast = buttonStateDelayLow;
  buttonStateDelayHighLast = buttonStateDelayHigh;
}

void displaySprayDelay() {
  if (lengthInt(sprayDelay) < lengthInt(sprayDelayLast)) {
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Spray Delay");

  lcd.setCursor(12, 0);
  lcd.print(sprayDelay, DEC);

  lcd.setCursor(12 + lengthInt(sprayDelay), 0);
  lcd.print("s");

  sprayDelayLast = sprayDelay;
}

void resetSprayShot() {

}

int lengthInt(int integer) {
  int result = 0;

  if(integer > 0) {
    while (integer > 0) {
      integer /= 10;
      result++;
    }
  } else {
    return 1;
  }

  return result;
}



