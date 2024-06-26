#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "WebHandlers.h"
#include "Settings.h"
#include "NetTime.h"
#include "Mute.h"
#include "Ring.h"
#include "Button.h"
#include "PinDefs.h"
#include "LED.h"

ESP8266WebServer server(80);
Button button;
static WebHandlers* inst; 

WebHandlers::WebHandlers(Mute &m, Settings &s, ID &i, Ring &r) : mute(m), settings(s), id(i), ring(r){}

void WebHandlers::setup() {
  inst = this;
	server.onNotFound([&]() {                              // If the client requests any URI
		if (!handleFileRead(server.uri()))                  // send it if it exists
			server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
	});

	server.begin();
  button.setup(RESET_PIN, handleMuteMore, handleResetMute);
}

void WebHandlers::doHandling() {
	server.handleClient();
  button.check();
}

void WebHandlers::handleRoot(File file) {
	server.send (200, "text/html", file.readString());
	Serial.print("Args ");
	Serial.println(server.args());
}

void WebHandlers::processPostData(){
	Serial.println("Post Data");
	if (server.args() > 0 )
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			Serial.println(server.argName(i));
			Serial.println(server.arg(i));
		}

	if (settings.getDataFromJson(server.arg("plain").c_str())) {
		settings.saveConfig();
    	NetTime::setTimeZone(settings.getTimeZone());
	}
	rooter();
}

void WebHandlers::handleValues() {
	String s = settings.getValues();
	server.send(200, "text/plain", s);
}

void WebHandlers::rooter() {
	File file = LittleFS.open("/index.html", "r");
	handleRoot(file);
}



String WebHandlers::getContentType(String filename) { // convert the file extension to the MIME type
	if (filename.endsWith(".html")) return "text/html";
	else if (filename.endsWith(".css")) return "text/css";
	else if (filename.endsWith(".js")) return "application/javascript";
	else if (filename.endsWith(".ico")) return "image/x-icon";
	return "text/plain";
}

bool WebHandlers::handleFileRead(String path) { // send the right file to the client (if it exists)
	//Serial.println("handleFileRead: " + path);
	if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
	bool root   = path.endsWith("/index.html");
	bool values = path.endsWith("/values");
	bool save   = path.indexOf("SAVE")>0;
	bool test   = path.indexOf("TEST")>0;
	bool detect = path.indexOf("DETECT")>0;
	bool mute1  = path.indexOf("MUTE1")>0;
	bool curTime  = path.indexOf("CURTIME")>0;
	bool curMute  = path.indexOf("CURMUTE")>0;
	bool rstMute  = path.indexOf("RSTMUTE")>0;
	bool lastSync = path.indexOf("LASTSYNC")>0;
	bool syncNow  = path.indexOf("SYNCNow")>0;
	bool play     = path.indexOf("PLAY")>0;
	String contentType = getContentType(path);            // Get the MIME type
  if (!curTime)
    Serial.println("handleFileRead: " + path);
	if (LittleFS.exists(path)) {                            // If the file exists
		File file = LittleFS.open(path, "r");                 // Open it
		if (root)
			handleRoot(file);
		else
			server.streamFile(file, contentType); // And send it to the client
		file.close();                                       // Then close the file again
		return true;
	} else if (values) {
		handleValues();
		return true;
	} else if (save) {
		processPostData();
		return true;
	} else if (test) {
		handleTest(path);
		return true;
	} else if (detect) {
		handleDetect();
		return true;
	} else if (curTime) {
		handleCurTime();
		return true;
	} else if (mute1) {
		handleMuteMore();
		return true;
	} else if (curMute) {
		handleCurMute();
		return true;
	} else if (rstMute) {
		handleResetMute();
		return true;
	} else if (lastSync) {
		handleLastSync();
		return true;
  } else if (syncNow) {
    handleSyncNow();
    return true;
  } else if (play) {
    handlePlay(path);
    return true;
	}
	Serial.println("\tFile Not Found");
	return false;                                         // If the file doesn't exist, return false
}


void WebHandlers::handleTest(String t) {
	Serial.println("Ring Test");
	Serial.println(t);
	bool isHour = t.endsWith("hr");
	bool isHalf = t.endsWith("hf");
	bool is1Qtr = t.endsWith("q1");
	bool is3Qtr = t.endsWith("q3");
	rooter();
	ring.testRing(isHour, isHalf, is1Qtr, is3Qtr);
}

void WebHandlers::handleDetect() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "*");
	server.send(200, "text/html", id.clientMac+".local "+id.myIP());
	Serial.println("Detect");
}


void WebHandlers::handleCurMute() {
	int res = mute.calcCurMute();
	server.send(200, "text/plain", (String)res);
	Serial.println("Cur Mute");
}

void WebHandlers::handleCurTime() {
  int h = NetTime::hour();
  int m = NetTime::min();
  int s = NetTime::sec();
  String res = (h<10?"0":"")+String(h)+":"+(m<10?"0":"")+String(m)+":"+(s<10?"0":"")+String(s);
  server.send(200, "text/plain", res);
  //Serial.println("Cur Time "+res);
}

void WebHandlers::handleLastSync() {
	String s = NetTime::lastSyncStr();
	server.send(200, "text/plain", s);
	Serial.print("Last Sync ");
	Serial.println(s);
}

void WebHandlers::handleResetMute() {
  LED::blinkLong();
  inst->mute.dynMute = 0;
  server.send(200, "text/plain", "Reset ok");
  Serial.println("Reset Mute");
}

void WebHandlers::handleMuteMore() {
  Serial.println("Add 1 Hour Mute");
  if (inst->mute.dynMute + 60 <= 1500) // 25 hours max
    inst->mute.dynMute += 60;
  int muteHours = (inst->mute.dynMute+50)/60; // round up to 60, sort of
  LED::blinkTimes(muteHours);
  Serial.print("Values: ");
  Serial.println(inst->mute.dynMute);
  server.send(200, "text/plain", "ok");
}

void WebHandlers::handleSyncNow() {
  NetTime::getTime();
  server.send(200, "text/plain", "Sync ok");
  Serial.println("Sync Now");
}

void WebHandlers::handlePlay(String t) {
  Serial.println("Play");
  Serial.println(t);
  ring.ringList(t);
  server.send(200, "text/plain", "ok");
}
