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

void setup() {
  currentState = 0;
}

void loop() {
  determineState();
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

}

void resetSprayShot() {

}

