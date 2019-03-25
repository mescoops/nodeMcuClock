#ifndef settings_h
#define settings_h

#include "Mute.h"
#include "ID.h"

class Settings {
public:
	Settings(Mute &m, ID &i);
	bool getDataFromJson(const char* buf);
	bool saveConfig();
	bool loadConfig();
	String getValues();
  int8_t getTimeZone();
  int8_t getMinutesTimeZone();

	int clock_mode = 0;
private:
	Mute &mute;
	ID &id;
	int time_tz;
	bool time_dst = true;
};

#endif
