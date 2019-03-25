#ifndef ring_h
#define ring_h

#include "Mute.h"
#include "Settings.h"

#define CLOCK_ALL  2
#define CLOCK_HALF 1
#define CLOCK_HOUR 0

// Time for solenoid activation
#define SOLEN_TIME   0.05
#define SOLEN_TIME_2 0.08


#define  HR_NONE 0
#define  HR_ALL  1
#define  HR_HALF 2
#define  HR_HOUR 3


class Ring {
public:
	Ring(Mute &m, Settings &s);
	void checkForRing();
	void doAllRing(bool hr, bool hf, bool q1, bool q3);
	static void multiTogg();
private:
	Mute &mute;
	Settings &settings;

	void doRing(bool hr, bool hf, bool q1, bool q3);
	void doRing(bool hr, bool hf);
	void doRing(bool hr);
	int findHrType();
	void play(int p[]);
	static void ding();
	static void togg();
};

#endif
