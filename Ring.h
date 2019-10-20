// Ring
// Rings chimes on hours, halfs, and quarters depending on time, mute, and chime style

#ifndef ring_h
#define ring_h

#include "Mute.h"
#include "Settings.h"
#include "RemoteDebug.h"

#define CLOCK_SHIPS         5
#define CLOCK_WESTM         4
#define CLOCK_ALL           3
#define CLOCK_HALF_MULTI    2
#define CLOCK_HALF_SINGLE   1
#define CLOCK_HOUR          0

class Ring {
  public:
    Ring(Mute &m, Settings &s, RemoteDebug &d);
    void checkForRing();
    void testRing(bool hr, bool hf, bool q1, bool q3);
    Mute &mute;
    Settings &settings;
    void getLogs();
    String logs[12];
    String logExtra;
    void printdbg();
    void printdbgLog();
    enum QTR { q_no, q_q1, q_hf, q_q3, q_hr };
    QTR currQuarter;
    RemoteDebug &Debug;
  private:
    void doAllRing();
    QTR calcQuarter();
    QTR calcQuarter(bool hr, bool hf, bool q1, bool q3);

    bool doRing();
    bool doRingShips();
    int findHrType();
    void play(int p[], float period);
    void play(int p[]);
    static void ding();
};

#endif
