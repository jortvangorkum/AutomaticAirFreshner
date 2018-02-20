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
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// Button to switch menus
const int buttonPinMenuSwitch = 8;
// Input pins for minus and plus
const int buttonPinMinus = 9;
const int buttonPinPlus = 10;

// Input pins sensors
const int LDR = 0;

// variables for sensors
int LDRvalue = 0;

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

// Creating LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*
  Timers
*/

void setup() {
  /*
    Assign values to variables
  */
  currentState = 0;
  sprayDelaySeconds = 0;
  sprayShots = 2400;
  buttonMinusPressed = false;
  buttonPlusPressed = false;
  buttonStateMinusLast = HIGH;
  buttonStatePlusLast = HIGH;
  /*
    Assign Pins to OUTPUT or INPUT
  */
  pinMode(buttonPinMinus, INPUT);
  pinMode(buttonPinPlus, INPUT);
  pinMode(buttonPinMenuSwitch, INPUT);
  pinMode(LDR, INPUT);
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  states();
  menuStates();
  determineStates();
  determineMenuStates();
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
    default:
      ambientTemperature();
      break;
  }
}

void determineMenuStates() {
  buttonStateMenuSwitch = digitalRead(buttonPinMenuSwitch);

  if (buttonStateMenuSwitchLast != buttonStateMenuSwitch && buttonStateMenuSwitch == LOW) {
    if (currentMenuState == 2) {
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
  LDRvalue = analogRead(LDR);
  if (LDRvalue < 500){
    currentState = 0;
  }
  //Plus magnetic contact

  // Check if useTypeUnknown
  if(currentState == 0) {
    LDRvalue = analogRead(LDR);
    if (LDRvalue > 500){
      currentState = 1;
    }
  }
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

  }
}

void notInUse() {

}

void useTypeUnknown() {

}

void useNumber1() {

}

void useNumber2() {

}

void useCleaning() {

}

void triggeredShot() {

}

void menuActive() {

}

void spray(int times) {
  for (int t = 0; t < times; t++) {
    // spray
  }
}

void sprayDelay() {
  displaySprayDelay();
  changeSprayDelay();
}

void ambientTemperature() {
  displayTemperature();
}

void remainingSprayShots() {
  displaySprayShot();
  resetSprayShot();
}

void displayTemperature() {

}

void displaySprayShot() {
  lcd.setCursor(0, 0);
  lcd.print("Spray Shots");

  lcd.setCursor(12, 0);
  lcd.print(sprayShots, DEC);
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

void displaySprayDelay() {
  if (lengthInt(sprayDelaySeconds) < lengthInt(sprayDelaySecondsLast)) {
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Spray Delay");

  lcd.setCursor(12, 0);
  lcd.print(sprayDelaySeconds, DEC);

  lcd.setCursor(12 + lengthInt(sprayDelaySeconds), 0);
  lcd.print("s");

  sprayDelaySecondsLast = sprayDelaySeconds;
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


