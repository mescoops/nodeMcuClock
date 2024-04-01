// Actuator
// Toggles pins or sets of pins to activate actuator

#ifndef actuator_h
#define actuator_h

class Actuator {
public:
  void play(int pin);
  void play(const int pinList[], int len);
private:
};

#endif
