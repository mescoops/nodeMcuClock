// Pin
// Holds state of pin for toggling

#ifndef pin_h
#define pin_h

#include <Ticker.h>

class Pin {
public:
  Pin(int p);
  void toggle();
  int pin;
  void off();
  static Pin * findPin(int p); 
  static int getPin(int i); 
  // Time for solenoid activation - ms
  static int solenoidHoldTime;
private:
  bool realPin;
  Ticker toggleTicker;
};

#endif
