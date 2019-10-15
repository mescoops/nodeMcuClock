// Button
// Sets action for short or long press

#ifndef button_h
#define button_h
typedef void (*ButtonAction) ();

class Button {
public:
  void setup(int pin, ButtonAction shortPress, ButtonAction longPress);
  void check();
private:
};
#endif
