#ifndef actuator_h
#define actuator_h

class Actuator {
public:
  void play(int pin);
  void play(int pinList[], int len);
private:
  static void toggPin();
  static void toggPinList();
};

#endif
