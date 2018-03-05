/*
  Libraries
*/
#include <OneWire.h>
#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <Arduino.h>

#define ONE_WIRE_BUS A4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

/*
/ Timer
*/
class Timer {
    unsigned long previousMillis;
    long interval;
    boolean done;

    public:
        Timer(long inter) {
          Serial.println("TIMER CREATED");
            interval = inter;

            previousMillis = millis();
            done = false;
        }

        void Start() {
          Serial.print("STARTED AT ");
          Serial.println(millis());
          previousMillis = millis();
          done = false;
        }

        boolean Update() {
            unsigned long currentMillis = millis();
            Serial.print("UPDATE OF TIMER: ");
            Serial.print(currentMillis);
            Serial.print(" ");
            Serial.println(previousMillis);

            if (done != true) {
                if (currentMillis - previousMillis >= interval) {
                    done = true;
                    return true;
                }
                return false;
            } else {
                return false;
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
const int Magnetpin = A3;
const int LDRpin = A2;
const int Motionpin = 3;
// Toiletet Freshener Pin
const int toiletFreshenerPin = 13;
// RGB LED
const int rPin = 11, gPin = 10, bPin = 9;
// Adresses
const int EEPROMVariablesAddress = 0;
// Structs
struct EEPROMVariables {
  int sprayDelaySeconds;
  int sprayShots;
};
/*
  States:
  - notInUse = 0
  - useTypeUnknown = 1
  - useNumber1 = 2
  - useNumber2 = 3
  - useCleaning = 4
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
//Timer
Timer timer1(4000);
Timer timer2(10000);
// Variables for sensors
int LDRvalue;
int Magnetvalue;
int Motionvalue;
// EEPROM Variables saved
bool EEPROMVariablesSaved;
// Creating LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  /*
    Assign values to variables
  */
  LDRvalue = 0;
  Magnetvalue = 0;
  Motionvalue = 0;
  switchState(0);
  buttonMinusPressed = false;
  buttonPlusPressed = false;
  buttonStateMinusLast = HIGH;
  buttonStatePlusLast = HIGH;
  EEPROMVariablesSaved = true;
  /*
    Assign values to EEPROM variables
  */
  readEEPROM(EEPROMVariablesAddress);
  /*
    Assign Pins to OUTPUT or INPUT
  */
  pinMode(buttonPinMinus, INPUT);
  pinMode(buttonPinPlus, INPUT);
  pinMode(buttonPinMenuSwitch, INPUT);
  pinMode(toiletFreshenerPin, OUTPUT);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);

  // sensors
  pinMode(Magnetpin, INPUT_PULLUP);
  pinMode(LDRpin, INPUT);
  pinMode(Motionpin, INPUT);
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //Serial port
  Serial.begin(9600);
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
  Temperature();
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
      triggeredShot(0);
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

void switchState(int state) {
  Serial.print("SWITCHED STATE: ");
  Serial.println(state);

  currentState = state;

  // start timer when switched to state 1
  if (state == 1) {
    timer1.Start();
  }
  if (state == 2) {
    timer2.Start();
  }
}

void determineStates() {
  //check for use => check voor LDR + Magnet => useTypeUnknown
  if (currentState == 0) {

    LDR();
    Magnet();
    if (LDRvalue > 500 && Magnetvalue == HIGH) {
      switchState(1);
    }
  }

  //check for use type => check voor LDR + Motion + Magnet + Timer => useNumber1 or useCleaning
  if (currentState == 1) {

    LDR();
    Magnet();
    Motion();

    if (LDRvalue > 500 && Magnetvalue == LOW && Motionvalue == HIGH) {
      switchState(2);
    }
    else if (LDRvalue > 500 && Magnetvalue == HIGH && timer1.Update()) {
      switchState(4);
    }
  }

  //check for number type => check voor LDR + Magnet + Motion + Timer => useNumber2
  if (currentState == 2) {

    LDR();
    Magnet();
    Motion();

    if (LDRvalue > 500 && Magnetvalue == LOW && timer2.Update()) {
      switchState(3);
    }
  }

  //check if cleaning is finished => check voor LDR + Magnet => notInUse
  if (currentState == 4) {

  }

   // Check if triggeredShot
  if(currentState == 2
  || currentState == 3) {

    LDR();
    Magnet();
    Motion();

    if (LDRvalue < 300 && Magnetvalue == LOW) {
      if (currentState == 2) {
        triggeredShot(1);
        switchState(0);
      }
      else if (currentState == 3) {
        triggeredShot(2);
        switchState(0);
      }
    }



  }
  // Check if menuActive
  if(currentState == 1
  || currentState == 2
  || currentState == 3
  || currentState == 4) {
    buttonStateMenuSwitch = digitalRead(buttonPinMenuSwitch);

    if (buttonStateMenuSwitch != buttonStateMenuSwitchLast && buttonStateMenuSwitch == LOW) {
      switchState(6);
      lcd.clear();
    }

    buttonStateMenuSwitchLast = buttonStateMenuSwitch;
  }
}

//Sensor Functions
void LDR() {
  LDRvalue = analogRead(LDRpin);
}

void Magnet() {
  Magnetvalue = digitalRead(Magnetpin);
}

void Motion() {
  Motionvalue = digitalRead(Motionpin);
}

void Temperature() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
}

//State Functions
void notInUse() {
  // Off
  if (EEPROMVariablesSaved != true) {
    writeEEPROM(EEPROMVariablesAddress);
    EEPROMVariablesSaved = true;
  }
  lcd.noDisplay();
  setRGBColor(0, 0, 0);
}

void useTypeUnknown() {
  // Purple
  lcd.display();
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

void triggeredShot(int n) {
  // Red
  setRGBColor(255, 0, 0);
  delay(sprayDelaySeconds * 1000);
  spray(n);
}

void menuActive() {
  // Blue
  setRGBColor(0, 0, 255);
  EEPROMVariablesSaved = false;

}

void spray(int times) {
  for (int t = 0; t < times; t++) {
    digitalWrite(toiletFreshenerPin, HIGH);
    delay(1000);
    digitalWrite(toiletFreshenerPin, LOW);
    delay(500);
    sprayShots -= 1;
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
  switchState(0);
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
    triggeredShot(1);
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

void readEEPROM(int address) {
  EEPROMVariables customVar;
  EEPROM.get(address, customVar);
  if (customVar.sprayDelaySeconds >= 0 && customVar.sprayShots >= 0) {
    sprayDelaySeconds = customVar.sprayDelaySeconds;
    sprayShots = customVar.sprayShots;
  } else {
    sprayDelaySeconds = 0;
    sprayShots = 2400;
  }
}

void writeEEPROM(int address) {
  EEPROMVariables customVar = {
    sprayDelaySeconds,
    sprayShots
  };
  EEPROM.put(address, customVar);
}