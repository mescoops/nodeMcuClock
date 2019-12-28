#include <Arduino.h>
#include "Pin.h"
#include "Chime.h"

// Time for solenoid activation
#define SOLEN_TIME   400  // ms

bool toggleStarted = false;
Ticker toggleTicker;

Pin::Pin(int p) {
  pin = p;
  started = 0;
  realPin = (pin != Chime::PAUSE) && (pin != Chime::END);
  if (realPin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }
}

void toggPinOff() {
  if (!Pin::anyActivePin()) {
    toggleTicker.detach();
    toggleStarted = false;
//    Serial.println("pin Off DONE");
    return;
  }
  
  Pin *p = Pin::findOverPin();
  while (p->pin != Chime::END) {
    p->off();
    p = Pin::findOverPin();
  }
}

void startToggle() {
  if (toggleStarted) return;
  toggleStarted = true;
  toggleTicker.attach(0.01, toggPinOff);
}

Pin PinEND(Chime::END);

Pin pinList[] = {
  Pin(Chime::PAUSE),
  Pin(Chime::A),
  Pin(Chime::B),
  Pin(Chime::C),
  Pin(Chime::D),
  Pin(Chime::E),
  Pin(Chime::F),
  Pin(Chime::G)
};

void Pin::toggle() {
  if (realPin)
    digitalWrite(pin, 1);    // set pin on
  started = millis();
  startToggle();
//  Serial.print("pin ");
//  Serial.print(pin);
//  Serial.println(" On");
}

void Pin::off() {
  digitalWrite(pin, 0);    // set pin off
  started = 0;
//  Serial.print("pin ");
//  Serial.print(pin);
//  Serial.println(" Off");
}

bool Pin::isOver() {
  unsigned long n = millis();
  return started && started+SOLEN_TIME < n;
}

Pin * Pin::findPin(int p) {
  //Serial.print("Find Pin ");
  //Serial.println(p);
  for (int i=0; i<8; i++) {
    if (pinList[i].pin == p)
      return &pinList[i];
  }
  Serial.println("Find Pin - Not Found");
  return &PinEND;
}

bool Pin::anyActivePin() {
  for (int i=0; i<8; i++)
    if (pinList[i].started)
      return true;
  return false;
}

Pin * Pin::findOverPin() {
  for (int i=0; i<8; i++)
    if (pinList[i].isOver())
      return &pinList[i];
  return &PinEND;
}
