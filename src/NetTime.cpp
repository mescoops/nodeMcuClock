#include "NetTime.h"
#include <coredecls.h>  // settimeofday_cb()
#include <Schedule.h>
#include <PolledTimeout.h>

#include <time.h>      // time() ctime()
#include <sys/time.h>  // struct timeval
#include <HardwareSerial.h>
#include <Arduino.h>

// NTP every 4 hours
#define NTP_INTERVAL 3600*4
#define NTP_TIMEOUT 2000

NetTime::NetTime(Settings &s) : settings(s){ }

static time_t now;
static time_t lastSync;
static String ntpServer;


void time_is_set(bool from_sntp) {
	Serial.print ("Got NTP time\n");
	lastSync = time(nullptr);
}

void NetTime::startup() {
	settimeofday_cb(time_is_set);
	String tz = settings.getTimeZone();
	ntpServer = settings.getNtpServer();
	Serial.printf ("configTime init TZ %s\n", tz.c_str());
    configTime(tz.c_str(), ntpServer);
}

void NetTime::setTimeZone(String tz) {
	settimeofday_cb(time_is_set);
	Serial.printf ("configTime TZ %s\n", tz.c_str());
    configTime(tz.c_str(), ntpServer);
}


void NetTime::getTime() {
//  NTP.getTime();
}

long NetTime::hour() {
	now = time(nullptr);
	return localtime(&now)->tm_hour;
}

long NetTime::min() {
	now = time(nullptr);
	return localtime(&now)->tm_min;
}

long NetTime::sec() {
	now = time(nullptr);
	return localtime(&now)->tm_sec;
}

long NetTime::hourMins() {
	now = time(nullptr);
	struct tm *tm = localtime(&now);
	return tm->tm_hour * 60 + tm->tm_min;
}

String NetTime::lastSyncStr() {
	return ctime(&lastSync);
}