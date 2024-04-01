#include <Arduino.h>
#include "Chime.h"
#include "Pin.h"
#include "Actuator.h"


void Actuator::play(int pin) {
  Pin *p = Pin::findPin(pin);
  p->toggle();
}

void Actuator::play(const int pList[], int len) {
  for (int i=0; i<len; i++)
    play(pList[i]);
}
