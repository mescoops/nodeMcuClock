#include <WiFiManager.h>
#include <TimeLib.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include "Settings.h"
#include "WebHandlers.h"
#include "Mute.h"
#include "Ring.h"
#include "ID.h"
#include "PinDefs.h"
#include "NetTime.h"

// Delay when staring to check if you press the reset button
// Gives you time to powerclock up and then find the button and press it
#define RESET_WAIT_DELAY 4000

// Give mDNS time to reply
#define SERVER_STARTUP_DELAY 20000

// Info on this clock: IP and MAC addr
ID id;

// Start and stop time of mute overnight
Mute mute;

// Saved settings
Settings settings(mute, id);

// Thing that rings chimes
Ring ring(mute, settings);

// Process web controls
WebHandlers webHandlers(mute, settings, id, ring);

// Get time from NTP
NetTime nettime(settings);

// Main control ticker
Ticker mainTicker;

// If first connected, start checking time from net
bool wifiFirstConnected = false;

void blinkLED() {
	//toggle led state
	int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
	digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
	Serial.println("Entered config mode");
	Serial.println(WiFi.softAPIP());
	//if you used auto generated SSID, print it
	Serial.println(myWiFiManager->getConfigPortalSSID());
	//entered config mode, make led toggle faster
	mainTicker.attach(0.2, blinkLED);
}


// Check if we need to ring some bells
void checkForRing() {
	ring.checkForRing();
}

////////////////////////////////////////////

void setup() {
	Serial.begin (115200);
	Serial.println ();

	// Init file system
	if (!SPIFFS.begin())
    Serial.println ("An Error has occurred while mounting SPIFFS");

	//set led pin as output
	pinMode(BUILTIN_LED, OUTPUT);
	// set reset pin as input
	pinMode(RESET_PIN, INPUT_PULLUP);

	// set ringer pins
	pinMode(BELL_A_PIN, OUTPUT);
	pinMode(BELL_B_PIN, OUTPUT);
	pinMode(BELL_C_PIN, OUTPUT);
	pinMode(BELL_D_PIN, OUTPUT);
	pinMode(BELL_E_PIN, OUTPUT);
	pinMode(BELL_F_PIN, OUTPUT);
	pinMode(BELL_G_PIN, OUTPUT);

	digitalWrite(BELL_A_PIN, 0);
	digitalWrite(BELL_B_PIN, 0);
	digitalWrite(BELL_C_PIN, 0);
	digitalWrite(BELL_D_PIN, 0);
	digitalWrite(BELL_E_PIN, 0);
	digitalWrite(BELL_F_PIN, 0);
	digitalWrite(BELL_G_PIN, 0);

	WiFiManager wifiManager;

	Serial.println ();

  // Blink LED for setup mode
	mainTicker.attach(1.5, blinkLED);

	// wait a bit on boot to give time to press rest button
	delay(RESET_WAIT_DELAY);
  
	// reset settings if button has been pressed
	if (!digitalRead(RESET_PIN)) {
		Serial.println ("Reset WiFI");
		wifiManager.resetSettings();
	}

	String lab = "<p>Connect to: <b>"+id.clientMac+".local</b></p>";
	WiFiManagerParameter custom_text(lab.c_str());
	wifiManager.addParameter(&custom_text);

	// start ticker with 0.5 because we start in AP mode and try to connect
	mainTicker.attach(0.6, blinkLED);

	//first parameter is name of access point, second is the password
	String url = "<a href=http://"+id.clientMac+".local>"+id.clientMac+".local</a>";
	String scClock = "Scoops' Clock "+id.lastOfMac();//<br>"+url;
	wifiManager.autoConnect(scClock.c_str(), "dingding");

	mainTicker.detach();
	//keep LED on
	digitalWrite(BUILTIN_LED, LOW);

	id.setIP();

	if (!MDNS.begin(id.cMac)) {             // Start the mDNS responder for clock-XXXX.local
		Serial.println("Error setting up MDNS responder!");
	} else {
		MDNS.addService("http", "tcp", 80);
		Serial.println("mDNS responder started");
	}

	WiFi.hostname(id.cMac);

	wifiFirstConnected = true;

	nettime.setup();

	settings.loadConfig();

	webHandlers.setup();

	delay(SERVER_STARTUP_DELAY);

	mainTicker.attach(5.0, checkForRing);
}


void loop () {
	if (wifiFirstConnected) {
		wifiFirstConnected = false;
		nettime.startup();
	}

	nettime.loop();

	delay(0);
	webHandlers.doHandling();
}
