#include <Arduino.h>
#include <TimeLib.h>
#include <Ticker.h>
#include "Chime.h"
#include "Ring.h"
#include "Actuator.h"
#include "Pin.h"


Chime westminster = Chime::westminster();
Chime quarters_2  = Chime::quarters_2();
Chime quarters_3  = Chime::quarters_3();
Chime halfs_3     = Chime::halfs_3();
Chime halfs_1_2   = Chime::halfs_1_2();
Chime hours       = Chime::hours();

Chime chime;

////////////////////////////////

//////////////////////////////////
// Ship's Clock
int listShip[12];
////////////////////////////////

int pIdx = 0;
const int *pList;
int pLen = 0;
Ticker ringer;
bool isRinging = false;
int hourCount;

Ring *instance;

void playHour(int h);
void hourDing();

unsigned long mmDevug = 0;

Actuator actuator;

Ring::Ring(Mute &m, Settings &s, RemoteDebug &d) : mute(m), settings(s), Debug(d) { 
  instance = this;
  for (int i=0; i<12; i++)
     logs[i] = "OK";
  logExtra = "Start";
  logExtra2 = "Sec "+second();
}

void Ring::printdbg() {
  unsigned long mm = millis();
  mm = mm / 100;
  Debug.print("DDD ");
  Debug.print(hourCount);
  Debug.print(" ");
  Debug.print(mm);
  Debug.print(" ");
  Debug.print(mm-mmDevug);
  if (instance->mute.dynMute!=0)
    Debug.print("  MUTED");
  Debug.println();
  mmDevug = mm;
}

void Ring::printdbgLog() {
  unsigned long mm = millis();
  mm = mm / 100;
  char s[100];
  //bool mut = instance->mute.dynMute!=0;
  sprintf(s, "[%d %lu %lu]", hourCount, mm, (mm-mmDevug));
  int hh = hour();
  if (hh>12) hh -= 12;
  if (hh==0) hh = 12;
  hh = hh-1;
  logs[hh] += String(s);
  mmDevug = mm;
//  Serial.println("SDasd");
//  Serial.println(s);
//  Serial.println(hh);
//  Serial.println("SDasd 2");
}

void Ring::getLogs() {
  for (int i=0; i<12; i++)
     Debug.println(logs[i]);
  Debug.println(logExtra);
  Debug.println(logExtra2);
}

Ring::QTR Ring::calcQuarter(bool hr, bool hf, bool q1, bool q3) {
  bool ship = settings.clock_mode == CLOCK_SHIPS;
  if (hr) return q_hr;
  if (hf) return q_hf;
  if (q1 && !ship) return q_q1;  // no ship's on 1st qtr
  if (q3 && !ship) return q_q3;  // no ship's on 3rd qtr
  return q_no;
}

String toString(Ring::QTR v) {
    switch (v) {
        case Ring::q_hr: return "q_hr";
        case Ring::q_hf: return "q_hf";
        case Ring::q_q1: return "q_q1";
        case Ring::q_q3: return "q_q3";
        default:      return "qqq";
    }
}

Ring::QTR Ring::calcQuarter() {
  int mins = minute();
  int secs = second();
  QTR q = calcQuarter(mins==0, mins==30, mins==15, mins==45);
  logExtra2 = String(" Calc Qtr ")+toString(q)+"  "+mins+":"+secs;
  return q;
}

// called every 60 seconds
void Ring::checkForRing() {
  static bool checking = false;
  if (checking) return;
  checking = true;

  //Debug.print("Sec ");
  //Debug.println(secs);
  
  // decrement mute every minute, if we are muted
  if (mute.dynMute>0) {
    mute.dynMute -= 1;
    //Debug.print("Dec MUte ");
    //Debug.println(mute.dynMute);
  }

  // Check every 60 seconds if we need to ring
  //   Are we ringing?
  //   Is it 0 minutes? (The hour)
  //   Is it 30 minutes?
  //   Is it 15 or 45 minutes?

  if (mute.calcCurMute()!=0 || isRinging) {
    logExtra = String("Mute or Ring ")+isRinging;
    checking = false;
    return;
  }

  currQuarter = calcQuarter();

  if (currQuarter != q_no)
    doAllRing();
    
  checking = false;
}

void setChimes(int t) {
  if (t == CLOCK_HOUR)        chime = hours;
  if (t == CLOCK_HALF_1_2)    chime = halfs_1_2;
  if (t == CLOCK_HALF_3)      chime = halfs_3;
  if (t == CLOCK_ALL_2)       chime = quarters_2;
  if (t == CLOCK_ALL_3)       chime = quarters_3;
  if (t == CLOCK_WESTM)       chime = westminster;
}

void Ring::testRing(bool hr, bool hf, bool q1, bool q3) {
  currQuarter = calcQuarter(hr, hf, q1, q3);
  doAllRing();
}

void Ring::doAllRing() {
  if (settings.clock_mode == CLOCK_SHIPS) {
    isRinging = doRingShips();
  } else {
    setChimes(settings.clock_mode);
    isRinging = doRing();
  }
}

bool Ring::doRing() {
  if (currQuarter == q_q1)
    play(chime.Q1);
  else if (currQuarter == q_q3)
    play(chime.Q3);
  else if (currQuarter == q_hf)
    play(chime.Hf);
  else if (currQuarter == q_hr)
    play(chime.Hr);
  return true;
}

void findShipRing() {
  //{A, A, PAUSE, A, A, PAUSE, A, A, PAUSE, A, A, END};
  int h = hour();
  h = h % 4;
  // copy base list
  for (int i=0; i<12; i++)
    listShip[i] = Chime::shipBase[i];
  // if 8 bells, just use that.
  if (h==0 && instance->currQuarter==Ring::q_hr)
    return;
  // put END after bell
  // eg. if one bell, put END at 1 
  int p = h * 3 + ((instance->currQuarter==Ring::q_hf) ? 1 : -1);
  listShip[p] = Chime::END;
}

bool Ring::doRingShips() {
  findShipRing();
  play(listShip, Chime::shipChPeriod_ms);
  return true;
}


void Ring::play(const int p[]) {
  play(p, chime.strikePeriod_ms);
}

// Play this list of notes
// List should end with END
void Ring::play(const int p[], long period_ms) {
  instance->Debug.println("play");
  if (p==NULL) return;
  pIdx = 0;
  pList = p;
  // Find len
  pLen = 30; // 'default'
  for (int i=0; i<30; i++) {
    if (p[i] == Chime::END) {
      pLen = i;
      break;
    }
  }
  //Serial.print("Play Len ");
  //Serialx.println(ll);
  // Play list items at intervals
  ringer.attach_ms(period_ms, ding);
}

void Ring::ding() {
  instance->Debug.print("Ding ");
  instance->Debug.print(pIdx);
  instance->Debug.print(" ");
  // Check if we are at last in list
  if (pIdx>=pLen) {
    ringer.detach();
    // Check if we need to play hour, too
    if (instance->currQuarter==Ring::q_hr && instance->settings.clock_mode != CLOCK_SHIPS) {
      int hh = hour();
      if (hh>12) hh -= 12;
      if (hh==0) hh = 12;
      playHour(hh);
    } else 
        isRinging = false;
  } else {
    // Play current selected note in list
    instance->Debug.print("Chime ");
    instance->Debug.println(Chime::bellToChar(pList[pIdx]));
    if (instance->mute.dynMute==0) // maybe someone pressed mute?
      actuator.play(pList[pIdx]);
    pIdx++;
  }
}


int delCount = 0;
void delay1() {
    delCount--;
    if (delCount<=0) {
      // make delay by no ringing first time
      ringer.detach();
      // after delay, play hour notes
      ringer.attach_ms(chime.strikeHrPeriod_ms, hourDing);
    }
}

void playHour(int h) {
  instance->Debug.print("play hour ");
  instance->Debug.println(h);
  instance->logs[h-1] = String(h)+" ";
  hourCount = h;
  if (instance->settings.clock_mode == CLOCK_HOUR || instance->settings.clock_mode == CLOCK_HALF_1_2)
    // just play hour, so play it right away
    ringer.attach_ms(chime.strikeHrPeriod_ms, hourDing);
  else {
    delCount = 2;
    ringer.attach_ms(chime.strikeHrPeriod_ms, delay1);
  }
}


void hourDing() {
  instance->printdbgLog();
  if (hourCount == 0) {
    ringer.detach();
    isRinging = false;
  } else {
    if (instance->mute.dynMute==0) // maybe someone pressed mute?
      actuator.play(chime.strikeHr, chime.strikeHrLen);
    hourCount--;
  }
}

void Ring::ringList(String l) {
  // after '&' split list on commas
  int list[8];  
  char* inp = const_cast<char*>(l.c_str());
  char* separatorAmp = strchr(inp, '&');
  ++separatorAmp;
  // Read each value
  char* v = strtok(separatorAmp, ",");
  int len = 0;
  while (v != 0 && len < 8) {
    list[len] = Pin::getPin(atoi(v));
    len++;
    // Find the next val
    v = strtok(0, ",");
  }
  actuator.play(list, len);
}
