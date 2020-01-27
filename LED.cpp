#include <Arduino.h>
#include <Ticker.h>
#include "LED.h"

// Ticker to flash LED
Ticker ledTicker;
Ticker ledFlashTicker;
int ledCount = 0;
int blinkCount = 0;
float currFlash = 0;

void blinkLED() {
  //toggle led state
  int state = digitalRead(LED::ledPin);  // get the current state of GPIO1 pin
  digitalWrite(LED::ledPin, !state);     // set pin to the opposite state
  if (blinkCount>0) {
    blinkCount--;
    if (blinkCount==0) {
      ledTicker.detach();
      // Turn flashing back on
      if (currFlash!=0)
        LED::flash(currFlash);
    }
  }
}

void flashLEDInt() {
  //flash led state
  if (ledCount==0) {
    digitalWrite(LED::ledPin, LOW);    // turn on
    ledCount++;
  } else {
    ledFlashTicker.detach();
    digitalWrite(LED::ledPin, HIGH);     // turn off
    ledCount = 0;
  }
}

void flashLED() {
  ledFlashTicker.attach(0.001, flashLEDInt);
}

void LED::setupLED() {
    //set led pin as output
  pinMode(ledPin, OUTPUT);
}

void LED::blinkLong() {
  blinkCount = 2;
  digitalWrite(ledPin, HIGH);
  ledTicker.attach(2.0, blinkLED);
}

void LED::blinkTimes(int count) {
  blinkCount = count * 2;
  digitalWrite(ledPin, HIGH);
  ledTicker.attach(0.2, blinkLED);
}

void LED::blink(float rate) {
  currFlash = 0;
  digitalWrite(ledPin, HIGH);
  ledTicker.attach(rate, blinkLED);
}

void LED::flash(float rate) {
  currFlash = rate;
  digitalWrite(ledPin, HIGH);
  ledTicker.attach(rate, flashLED);
}
