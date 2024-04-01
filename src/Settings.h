// Settings
// Saves settings in LittleFS

#ifndef settings_h
#define settings_h

#include <Arduino.h>
#include "Mute.h"
#include "ID.h"

class Settings {
public:
	Settings(Mute &m, ID &i);
	bool getDataFromJson(const char* buf);
	bool saveConfig();
	bool loadConfig();
	String getValues();
	String getTimeZone();
	String getTimeZoneName();
	char* getNtpServer();
	int clock_mode = 0;
private:
	Mute &mute;
	ID &id;
	String time_tz;
	String time_tz_name;
	String ntp_server = "pool.ntp.com";
};

#endif
