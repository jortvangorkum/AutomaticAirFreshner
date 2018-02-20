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
    boolean unlimitedTrue;

    public:
        Timer(long inter, boolean uTrue) {
            interval = inter;
            unlimitedTrue = uTrue;

            previousMillis = 0;
            done = false;
        }

        boolean Update() {
            unsigned long currentMillis = millis();

            if(unlimitedTrue) {
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
bool resettingSprayShots;

// Creating LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  /*
    Assign values to variables
  */
  currentState = 0;
  sprayDelaySeconds = 0;
  sprayShots = 2400;
  resettingSprayShots = false;
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
  /*Check de lightsensor (LDR)
   LDRvalue = analogRead(LDR)
   if (LDRvalue > 500){
     currentState = 1;
   }
   gaan we zo de states determen?
   */
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
  if (!resettingSprayShots) {
    lcd.setCursor(0, 0);
    lcd.print("Spray Shots");

    lcd.setCursor(12, 0);
    lcd.print(sprayShots, DEC);
  }
}

void resetSprayShot() {
  buttonStateMinus = digitalRead(buttonPinMinus);
  buttonStatePlus = digitalRead(buttonPinPlus);

  if (buttonStateMinusLast != buttonStateMinus && buttonStateMinus == LOW) {
    resettingSprayShots = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.blink();
    lcd.print("Reset spray shot");
    lcd.setCursor(0, 1);
    lcd.print("(y/n)");
    Timer timer1(10000, true);
    if (timer1.Update()) {
      if (buttonStatePlusLast != buttonStatePlus && buttonStatePlus == LOW) {
        sprayShots = 2400;
        resettingSprayShots = false;
        lcd.clear();
      } else if (buttonStateMinusLast != buttonStateMinus && buttonStateMinus == LOW) {
        resettingSprayShots = false;
        lcd.clear();
      }
    }
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



