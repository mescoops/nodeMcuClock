// NetTime
// Periodically gets time from NTP server

#ifndef nettime_h
#define nettime_h

#include "Settings.h"

class NetTime {
public:
	NetTime(Settings &s);
	void setup();
	void startup();
	void loop();
  static void getTime();
private:
	Settings &settings;
};

#endif
