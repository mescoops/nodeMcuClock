#ifndef led_h
#define led_h

class LED {
public:
  static void setupLED();
  static void blinkTimes(int count);
  static void blinkLong();
  static void blink(float rate);
  static void flash(float rate);
private:
};

#endif
