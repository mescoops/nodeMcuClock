#ifndef webHandlrs_h
#define webHandlrs_h

#include <FS.h>
#include "Mute.h"
#include "Ring.h"

class WebHandlers {
public:
	WebHandlers(Mute &m, Settings &s, ID &i, Ring &r);
	void setup();
	void doHandling();
private:
	Mute &mute;
	Settings &settings;
	ID &id;
	Ring &ring;

	void rooter();
	void handleRoot(File file);
	void processPostData();
	String getContentType(String filename);
	bool handleFileRead(String path);
	void handleTest(String t);
	void handleDetect();
  void handleCurTime();
	void handleMuteMore();
	void handleCurMute();
	void handleResetMute();
	void handleLastSync();
	void handleValues();
};

#endif
