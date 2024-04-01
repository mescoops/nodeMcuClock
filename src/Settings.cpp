#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "Settings.h"
#include "Pin.h"
#include "NetTime.h"

Settings::Settings(Mute &m, ID &i) : mute(m), id(i){ }

bool Settings::loadConfig() {
	File configFile = LittleFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Failed to open config file");
		return false;
	}

	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}

	// Allocate a buffer to store contents of the file.
	std::unique_ptr<char[]> buf(new char[size]);

	// We don't use String here because ArduinoJson library requires the input
	// buffer to be mutable. If you don't use ArduinoJson, you may as well
	// use configFile.readString instead.
	configFile.readBytes(buf.get(), size);
	configFile.close();

	getDataFromJson(buf.get());
	return true;
}

bool Settings::getDataFromJson(const char* buf) {
  JsonDocument json;
  DeserializationError err = deserializeJson(json, buf);
	if (err) {
		Serial.println("Failed to parse config file");
    Serial.println(err.c_str());
		return false;
	}

	time_tz      = json["tz"].as<String>();
	time_tz_name = json["tzn"].as<String>();
	clock_mode   = json["mode"];
	ntp_server   = json["server"].as<String>();
	if (ntp_server == NULL || ntp_server == "null")
	    ntp_server = "pool.ntp.org";

	mute.mute_enab  = json["mute"].as<String>() == "true";
	mute.mute_start = json["mute_start"];
	mute.mute_end   = json["mute_end"];

	Pin::solenoidHoldTime = json["sol_hold"];

	Serial.println("Loaded config");
	Serial.print("     tz "); Serial.println(time_tz);
	Serial.print("    tzn "); Serial.println(time_tz_name);
	Serial.print("   mode "); Serial.println(clock_mode);
	Serial.print(" server "); Serial.println(ntp_server);
	Serial.print("   mute "); Serial.println(mute.mute_enab);
	Serial.print("mute st "); Serial.println(mute.mute_start);
	Serial.print("mote en "); Serial.println(mute.mute_end);
	Serial.print("sol hld "); Serial.println(Pin::solenoidHoldTime);

	return true;
}

bool Settings::saveConfig() {
  JsonDocument json;
	json["tz"]         = time_tz;
	json["tzn"]        = time_tz_name;
	json["mode"]       = clock_mode;
	json["server"]     = ntp_server;
	json["mute"]       = (mute.mute_enab?1:0);
	json["mute_start"] = mute.mute_start;
	json["mute_end"]   = mute.mute_end;
	json["sol_hold"]   = Pin::solenoidHoldTime;

	File configFile = LittleFS.open("/config.json", "w");
	if (!configFile) {
		Serial.println("Failed to open config file for writing");
		return false;
	}

	serializeJson(json, configFile);
	configFile.close();
	Serial.println("Save all configs to file");
	return true;
}

const char compile_date[] = __DATE__; //" " __TIME__;

String Settings::getValues() {
	String values = "";
	values += "clock_software|" + (String)compile_date + "|span\n";
	values += "clock_ip|"   + (String)id.myIP() + "|span\n";
	values += "clock_name|" + (String)id.clientMac+".local" + "|span\n";
	values += "last_sync|"  + NetTime::lastSyncStr() + "|span\n";
	values += "time_tz|"   + (String)time_tz_name + "|select\n";
	values += "clock_mode|" + (String)clock_mode + "|input\n";
	values += "ntp_server|" + (String)ntp_server + "|input\n";
	values += "mute_enab|"  + (String)(mute.mute_enab ? "checked" : "") + "|chk\n";
	values += "mute_start|" + (String)mute.mute_start + "|input\n";
	values += "mute_end|"   + (String)mute.mute_end + "|input\n";
	values += "sol_hold|"   + (String)Pin::solenoidHoldTime + "|input\n";
	values += "clock_mute|Mute "+(mute.dynMute>0?"Ends: "+(String)(mute.dynMute/60):"Off") + "|span\n";
	Serial.print("Values: ");
	Serial.println(values);
	return values;
}

String Settings::getTimeZoneName() {
  return time_tz_name;
}

String Settings::getTimeZone() {
  return time_tz;
}

char* Settings::getNtpServer() {
  char *p = const_cast<char*>(ntp_server.c_str());
  return p;
}
