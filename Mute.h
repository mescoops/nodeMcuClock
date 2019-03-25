#ifndef mute_h
#define mute_h

class Mute {
public:
	int calcCurMute();

	int dynMute = 0;
	bool mute_enab = false;
	int mute_start = 230; // 11:00 pm
	int mute_end   = 65;  //  6:30 am
private:
	long calcMuteStart();
	long calcMuteEnd();
	int currMinFromMidnight(int muteV);
};

#endif
