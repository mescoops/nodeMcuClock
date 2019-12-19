#include <Arduino.h>
#include <Ticker.h>
#include "Chime.h"
#include "Actuator.h"

// Time for solenoid activation
#define SOLEN_TIME   0.25
#define SOLEN_TIME_2 0.25

Ticker toggle;

int currPin;

int *pinList;
int pinListLen = 0;

void Actuator::play(int pin) {
  currPin = pin;
  toggle.attach(SOLEN_TIME, toggPin);
}

void Actuator::play(int pList[], int len) {
  pinList = pList;
  pinListLen = len;
  toggle.attach(SOLEN_TIME_2, toggPinList);
}

void Actuator::toggPin() {
  //toggle bell pin
  int state = 0;
  //  Serial.print("pin ");
  //  Serial.print(currPin);
  //  Serial.println(" ");
  if (currPin>=Chime::PAUSE) { // currPin -1 means pause
    state = digitalRead(currPin);  // get the current state of GPI pin
    digitalWrite(currPin, !state);     // set pin to the opposite state
  }
  if (state==1 || currPin<0) {
    toggle.detach();
  }
}

void Actuator::toggPinList() {
  //toggle multiple bell
  int state = digitalRead(pinList[0]);  // get the current state of one of the GPIO pins
  for (int i=0; i<pinListLen; i++)
    digitalWrite(pinList[i], !state);     // set pin to the opposite state
  if (state == 1)
    toggle.detach();
}
