/*
  Libraries
*/
#include <LiquidCrystal.h>

/*
  Classes
*/

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

/*
  Constant variables
*/
// Input pins for LCD screen
const int rs = 12, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
// Button to switch menus
const int buttonPinMenuSwitch = 2;
// Input pins for minus and plus
const int buttonPinMinus = A0;
const int buttonPinPlus = A1;
// Input pins sensors
const int LDR = 0;
// Integrated LED
const int integratedLedPin = 13;
// RGB LED
const int rPin = 11, gPin = 10, bPin = 9;
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
/*
  States:
  - Spray Delay = 0
  - Ambient Temperature = 1
  - Remaining Spray Shots = 2
  - Return To Not In Use = 3
*/
int currentMenuState;
// Delay before spraying
int sprayDelaySeconds;
int sprayDelaySecondsLast;
// Button states for spray delay
int buttonStateMinus;
int buttonStateMinusLast;
int buttonStatePlus;
int buttonStatePlusLast;
// Button states to switch menus
int buttonStateMenuSwitch;
int buttonStateMenuSwitchLast;
// Amount of spray shots left
int sprayShots;
bool buttonMinusPressed;
bool buttonPlusPressed;
// Ambient Temperature
int temperature;
// Integrated LED state
int integratedLedState;
// Variables for sensors
int LDRvalue;
// Creating LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*
  Timers
*/

void setup() {
  /*
    Assign values to variables
  */
  LDRvalue = 0;
  currentState = 1;
  sprayDelaySeconds = 0;
  sprayShots = 2400;
  buttonMinusPressed = false;
  buttonPlusPressed = false;
  buttonStateMinusLast = HIGH;
  buttonStatePlusLast = HIGH;
  integratedLedState = LOW;
  /*
    Assign Pins to OUTPUT or INPUT
  */
  pinMode(buttonPinMinus, INPUT);
  pinMode(buttonPinPlus, INPUT);
  pinMode(buttonPinMenuSwitch, INPUT);
  pinMode(LDR, INPUT);
  pinMode(integratedLedPin, OUTPUT);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  states();
  determineStates();
  if (currentState == 6) {
    menuStates();
    determineMenuStates();
  } else {
    normalDisplay();
  }
}

void states() {
  switch(currentState) {
    case 0:
      notInUse();
      break;
    case 1:
      useTypeUnknown();
      break;
    case 2:
      useNumber1();
      break;
    case 3:
      useNumber2();
      break;
    case 4:
      useCleaning();
      break;
    case 5:
      triggeredShot();
      break;
    case 6:
      menuActive();
      break;
  }
}

void menuStates() {
  switch(currentMenuState) {
    case 0:
      sprayDelay();
      break;
    case 1:
      ambientTemperature();
      break;
    case 2:
      remainingSprayShots();
      break;
    case 3:
      returnToNotInUse();
      break;
    default:
      returnToNotInUse();
      break;
  }
}

void determineMenuStates() {
  buttonStateMenuSwitch = digitalRead(buttonPinMenuSwitch);

  if (buttonStateMenuSwitchLast != buttonStateMenuSwitch && buttonStateMenuSwitch == LOW) {
    if (currentMenuState == 3) {
      currentMenuState = 0;
    } else {
      currentMenuState += 1;
    }
    lcd.clear();
  }

  buttonStateMenuSwitchLast = buttonStateMenuSwitch;
}

void determineStates() {
  // Check if notInUse
  // LDRvalue = analogRead(LDR);
  // if (LDRvalue < 500){
  //   currentState = 0;
  // }
  //Plus magnetic contact

  // Check if useTypeUnknown
  // if(currentState == 0) {
  //   LDRvalue = analogRead(LDR);
  //   if (LDRvalue > 500){
  //     currentState = 1;
  //   }
  // }
  // Check if useNumber1
  if(currentState == 4) {

  }
  // Check if useNumber2
  if(currentState == 1) {

  }
  // Check if useCleaning
  if(currentState == 4) {

  }
  // Check if triggeredShot
  if(currentState == 1
  || currentState == 2
  || currentState == 3
  || currentState == 4) {

  }
  // Check if menuActive
  if(currentState == 1
  || currentState == 2
  || currentState == 3
  || currentState == 4) {
    buttonStateMenuSwitch = digitalRead(buttonPinMenuSwitch);

    if (buttonStateMenuSwitch != buttonStateMenuSwitchLast && buttonStateMenuSwitch == LOW) {
      currentState = 6;
      lcd.clear();
    }

    buttonStateMenuSwitchLast = buttonStateMenuSwitch;
  }
}

void notInUse() {
  lcd.noDisplay();
  // Off
  setRGBColor(0, 0, 0);
}

void useTypeUnknown() {
  lcd.display();
  // Purple
  setRGBColor(255, 0, 255);
}

void useNumber1() {
  // Light Blue
  setRGBColor(0, 255, 255);
}

void useNumber2() {
  // Green
  setRGBColor(0, 255, 0);

}

void useCleaning() {
  // White
  setRGBColor(255, 255, 255);

}

void triggeredShot() {
  // Red
  setRGBColor(255, 0, 0);

}

void menuActive() {
  // Blue
  setRGBColor(0, 0, 255);

}

void spray(int times) {
  for (int t = 0; t < times; t++) {
    // spray
  }
}

void normalDisplay() {
  displayTemperature(0);
  displaySprayShot(1);
  manualSprayShot();
}

void sprayDelay() {
  displaySprayDelay(0);
  changeSprayDelay();
}

void ambientTemperature() {
  displayTemperature(0);
}

void remainingSprayShots() {
  displaySprayShot(0);
  resetSprayShot();
}

void returnToNotInUse() {
  currentState = 0;
}

void displayTemperature(int row) {
  lcd.setCursor(0, row);
  lcd.print("Temperature");

  lcd.setCursor(12, row);
  lcd.print(temperature, DEC);

  lcd.setCursor(12 + lengthInt(temperature), row);
  lcd.print((char)223);

  lcd.setCursor(13 + lengthInt(temperature), row);
  lcd.print("C");
}

void displaySprayShot(int row) {
  lcd.setCursor(0, row);
  lcd.print("Spray Shots");

  lcd.setCursor(12, row);
  lcd.print(sprayShots, DEC);
}

void displaySprayDelay(int row) {
  if (lengthInt(sprayDelaySeconds) < lengthInt(sprayDelaySecondsLast)) {
    lcd.clear();
  }

  lcd.setCursor(0, row);
  lcd.print("Spray Delay");

  lcd.setCursor(12, row);
  lcd.print(sprayDelaySeconds, DEC);

  lcd.setCursor(12 + lengthInt(sprayDelaySeconds), row);
  lcd.print("s");

  sprayDelaySecondsLast = sprayDelaySeconds;
}

void manualSprayShot() {
  buttonStateMinus = digitalRead(buttonPinMinus);
  buttonStatePlus = digitalRead(buttonPinPlus);

  if (buttonStateMinusLast != buttonStateMinus && buttonStateMinus == LOW) {
    buttonMinusPressed = true;
  }

  if (buttonStatePlusLast != buttonStatePlus && buttonStatePlus == LOW) {
    buttonPlusPressed = true;
  }

  if (buttonMinusPressed && buttonPlusPressed) {
    spray(1);
    buttonMinusPressed = false;
    buttonPlusPressed = false;
  }

  buttonStateMinusLast = buttonStateMinus;
  buttonStatePlusLast = buttonStatePlus;
}

void resetSprayShot() {
  buttonStateMinus = digitalRead(buttonPinMinus);
  buttonStatePlus = digitalRead(buttonPinPlus);

  if (buttonStateMinusLast != buttonStateMinus && buttonStateMinus == LOW) {
    buttonMinusPressed = true;
  }

  if (buttonStatePlusLast != buttonStatePlus && buttonStatePlus == LOW) {
    buttonPlusPressed = true;
  }

  if (buttonMinusPressed && buttonPlusPressed) {
    sprayShots = 2400;
    buttonMinusPressed = false;
    buttonPlusPressed = false;
  }

  buttonStateMinusLast = buttonStateMinus;
  buttonStatePlusLast = buttonStatePlus;
}

void changeSprayDelay() {
  buttonStateMinus = digitalRead(buttonPinMinus);
  buttonStatePlus = digitalRead(buttonPinPlus);

  if (buttonStateMinusLast != buttonStateMinus && buttonStateMinus == LOW) {
    if (sprayDelaySeconds > 0) {
      sprayDelaySeconds -= 1;
    }
  } else if (buttonStatePlusLast != buttonStatePlus && buttonStatePlus == LOW) {
    sprayDelaySeconds += 1;
  }

  buttonStateMinusLast = buttonStateMinus;
  buttonStatePlusLast = buttonStatePlus;
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

void setRGBColor(int red, int green, int blue) {
  analogWrite(rPin, red);
  analogWrite(gPin, green);
  analogWrite(bPin, blue);
}

