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
#include "LED.h"
#include "RemoteDebug.h"

// Delay when staring to check if you press the reset button
// Gives you time to power clock up and then find the button and press it
#define RESET_WAIT_DELAY 4000

// Give mDNS time to reply
#define SERVER_STARTUP_DELAY 20000

RemoteDebug Debug;

// Info on this clock: IP and MAC addr
ID id;

// Start and stop time of mute overnight
Mute mute;

// Saved settings
Settings settings(mute, id);

// Thing that rings chimes on quarters, hours
Ring ring(mute, settings, Debug);

// Process web controls
WebHandlers webHandlers(mute, settings, id, ring);

// Get time from NTP
NetTime nettime(settings);

// Main control ticker
Ticker mainTicker;

// If first connected, start checking time from net
bool wifiFirstConnected = false;


//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
	Serial.println("Entered config mode");
	Serial.println(WiFi.softAPIP());
	//if you used auto generated SSID, print it
	Serial.println(myWiFiManager->getConfigPortalSSID());
	//entered config mode, make led toggle faster
	LED::blink(0.2);
}


// line up ticker to 0th second
void setupClockCheck() {
  int secs = second();
  if ((secs % 60)==0) {
    mainTicker.detach();
    // maybe we are at a quarter etc.
    ring.checkForRing();
    // Then start check every 60 seconds
    mainTicker.attach(60.0, checkForRing);
  }
}

// Check if we need to ring some bells
void checkForRing() {
  int secs = second();
	ring.checkForRing();
  // if we are out of sync, reset clock
  if (secs>4) {
    mainTicker.detach();
    mainTicker.attach(0.5, setupClockCheck);
  }
}

////////////////////////////////////////////

void setup() {
	Serial.begin (115200);
	Serial.println ();

	// Init file system
	if (!SPIFFS.begin())
    Serial.println ("An Error has occurred while mounting SPIFFS");

  LED::setupLED();
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
  LED::blink(1.5);

	// wait a bit on boot to give time to press reset button
	delay(RESET_WAIT_DELAY);
  
	// reset settings if button has been pressed
	if (!digitalRead(RESET_PIN)) {
		Serial.println ("Reset WiFI");
		wifiManager.resetSettings();
	}

	String lab = "<p>Connect to: <b>"+id.clientMac+".local</b></p>";
	WiFiManagerParameter custom_text(lab.c_str());
	wifiManager.addParameter(&custom_text);

	// Blink every 0.6 because we start in AP mode and try to connect
  LED::blink(0.6);

	//first parameter is name of access point, second is the password
	String url = "<a href=http://"+id.clientMac+".local>"+id.clientMac+".local</a>";
	String scClock = "Scoops' Clock "+id.lastOfMac();//<br>"+url;
	wifiManager.autoConnect(scClock.c_str(), "dingding");

  //LED off, then long period flashing
  LED::flash(60.0);

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

  Debug.begin("CLOCK");
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showColors(true); // Colors
  String helpCmd = "hist - Histoty of dings\n";
  //helpCmd.concat("bench2 - Benchmark 2");
  Debug.setHelpProjectsCmds(helpCmd);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);

	settings.loadConfig();

	webHandlers.setup();

	delay(SERVER_STARTUP_DELAY);

  // Find next 0th second, then start clock
  mainTicker.attach(0.5, setupClockCheck);
}


void loop () {
	if (wifiFirstConnected) {
		wifiFirstConnected = false;
		nettime.startup();
	}

	nettime.loop();
	webHandlers.doHandling();
  Debug.handle();
  yield();
}

void processCmdRemoteDebug() {
  String lastCmd = Debug.getLastCommand();
  if (lastCmd == "hist") {
    ring.getLogs();
  }
}
