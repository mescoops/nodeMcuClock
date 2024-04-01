// NetTime
// Periodically gets time from NTP server

#ifndef nettime_h
#define nettime_h

#include "Settings.h"

class NetTime {
public:
	NetTime(Settings &s);
	void startup();
    static void getTime();
	static long hour();
	static long min();
	static long sec();
	static long hourMins();
	static String lastSyncStr();
    static void setTimeZone(String tz);
private:
	Settings &settings;
};

#endif
