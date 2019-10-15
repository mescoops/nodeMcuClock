// Settings
// Saves settings in SPIFFS

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
  char* getNtpServer();
  bool getSummer();
	int clock_mode = 0;
private:
	Mute &mute;
	ID &id;
	int time_tz;
	bool time_dst = true;
  String ntp_server = "pool.ntp.com";
};

#endif
