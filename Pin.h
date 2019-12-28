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
  unsigned long started;
  void off();
  static Pin * findPin(int p); 
  static Pin * findOverPin(); 
  static bool anyActivePin();
private:
  bool isOver();
  int currState;
  void toggPin();
  bool realPin;
};

#endif
