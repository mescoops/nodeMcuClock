#include <NtpClientLib.h>
#include "NetTime.h"

// NTP every 4 hours
#define NTP_INTERVAL 3600*4

NetTime::NetTime(Settings &s) : settings(s){ }

void processSyncEvent (NTPSyncEvent_t ntpEvent) {
	if (ntpEvent) {
		Serial.print ("Time Sync error: ");
		if (ntpEvent == noResponse)
			Serial.println ("NTP server not reachable");
		else if (ntpEvent == invalidAddress)
			Serial.println ("Invalid NTP server address");
	} else {
		Serial.print ("Got NTP time: ");
		Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
	}
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

void NetTime::setup() {
	NTP.onNTPSyncEvent ([&](NTPSyncEvent_t event) {
		ntpEvent = event;
		syncEventTriggered = true;
	});
}

void NetTime::startup() {
	NTP.begin("pool.ntp.org", settings.getTimeZone(), true, settings.getMinutesTimeZone());
	NTP.setInterval(NTP_INTERVAL);
}

void NetTime::loop() {
	if (syncEventTriggered) {
		processSyncEvent (ntpEvent);
		syncEventTriggered = false;
	}
}

