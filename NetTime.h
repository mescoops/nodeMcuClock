#ifndef nettime_h
#define nettime_h

#include "Settings.h"

class NetTime {
public:
	NetTime(Settings &s);
	void setup();
	void startup();
	void loop();
private:
	Settings &settings;
};

#endif
