#include <Arduino.h>
#include "Pin.h"
#include "Chime.h"

int Pin::solenoidHoldTime;

Pin::Pin(int p) {
  pin = p;
  realPin = (pin != Chime::PAUSE) && (pin != Chime::END);
  if (realPin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }
}

void toggPinOff(int pin) {
  Pin* p = Pin::findPin(pin);
  if (p!=NULL)
    p->off();
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
  toggleTicker.once_ms(solenoidHoldTime, toggPinOff, pin);
//  Serial.print("Togle");
//  Serial.println(solenoidHoldTime);
//  Serial.print("pin ");
//  Serial.print(pin);
//  Serial.println(" On");
}

void Pin::off() {
  digitalWrite(pin, 0);    // set pin off
//  Serial.print("pin ");
//  Serial.print(pin);
//  Serial.println(" Off");
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

int Pin::getPin(int i) {
  if (i<0 || i>=8)
    i = 0;
  return pinList[i].pin;
}
