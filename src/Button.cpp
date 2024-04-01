#include <Arduino.h>
#include "Button.h"

long buttonTimer = 0;
long unsigned longPressTime = 500;

boolean buttonActive = false;
boolean longPressActive = false;
int btnPin;

ButtonAction shortAction;
ButtonAction longAction;

void Button::setup(int pin, ButtonAction shortPress, ButtonAction longPress) {
  btnPin = pin;
  shortAction = shortPress;
  longAction = longPress;
}

void Button::check() {
  if (!digitalRead(btnPin)) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      longAction();
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        shortAction();
      }
      buttonActive = false;
    }
  }
}
