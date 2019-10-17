// Ring
// Rings chimes on hours, halfs, and quarters depending on time, mute, and clock style

#ifndef ring_h
#define ring_h

#include "Mute.h"
#include "Settings.h"
#include "RemoteDebug.h"

#define CLOCK_SHIPS  4
#define CLOCK_WESTM  3
#define CLOCK_ALL    2
#define CLOCK_HALF   1
#define CLOCK_HOUR   0


#define  HR_NONE 0
#define  HR_ALL  1
#define  HR_HALF 2
#define  HR_HOUR 3


class Ring {
  public:
    Ring(Mute &m, Settings &s, RemoteDebug &d);
    void checkForRing();
    void doAllRing(bool hr, bool hf, bool q1, bool q3);
    Mute &mute;
    Settings &settings;
    void getLogs();
    String logs[12];
    String logExtra;
    void printdbg();
    void printdbgLog();
  private:
    RemoteDebug &Debug;
    bool doRing(bool hr, bool hf, bool q1, bool q3);
    bool doRing(bool hr, bool hf);
    bool doRing(bool hr);
    bool doRingShips(bool hr, bool hf);
    int findHrType();
    void play(int p[], float period);
    void play(int p[]);
    static void ding();
};

#endif
