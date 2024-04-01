#include <WiFiManager.h>
#include "ID.h"

ID::ID() {
	unsigned char mac[6];
	WiFi.macAddress(mac);
	clientMac += mac2Str(mac, 3);
	clientMac = "clock-"+clientMac;
	Serial.println (clientMac);
	cMac = clientMac.c_str();
}

String ID::myIP() {
	return WiFi.localIP().toString();
}

String ID::lastOfMac() {
	unsigned char mac[6];
	WiFi.macAddress(mac);
	return mac2Str(mac, 1);
}

String ID::mac2Str(byte ar[], int len){
	int tot = 6;
	int start = tot - len;

	String s;
	for (byte i = start; i < tot; ++i) {
		char buf[3];
		sprintf(buf, "%2X", ar[i]);
		s += buf;
	}
	s.toLowerCase();
	return s;
}


