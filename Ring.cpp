#include <Arduino.h>
#include <TimeLib.h>
#include <Ticker.h>
#include "Ring.h"
#include "PinDefs.h"
#include "Actuator.h"

int A = BELL_A_PIN;
int B = BELL_B_PIN;
int C = BELL_C_PIN;
int D = BELL_D_PIN;
int E = BELL_E_PIN;
int F = BELL_F_PIN;
int G = BELL_G_PIN;
int END = -100;
int PAUSE = -1;

char bellToChar(int b){
  if (b==A) return 'A';
  if (b==B) return 'B';
  if (b==C) return 'C';
  if (b==D) return 'D';
  if (b==E) return 'E';
  if (b==F) return 'F';
  if (b==G) return 'G';
  if (b==END) return 'n';
  if (b==PAUSE) return '.';
  return '?';
}

int listRQ1[5] = {C, C, PAUSE, A, END};
int listRHf[6] = {B, B, PAUSE, A, C, END};
int listRQ3[7] = {A, A, PAUSE, C, B, C, END};
int listRHr[6] = {C, B, C, B, A, END};

// Pair of chimes for Hour chiming
int listRCh[2] = {C, A};
int listRChLen = 2;
float listRChPeriod = 0.9;

int listHHf[2] = {B, END};
int listHHr[1] = {END};
int listHCh[3] = {A, C, END};


//////////////////////////////////
// Westminster
// G♯4, F♯4, E4, B3
// D    C    B   A

int listWQ1[5]  = {D, C, B, A, END};
int listWHf[10] = {B, D, C, A, PAUSE, B, C, D, B, END};
int listWQ3[15] = {D, B, C, A, PAUSE, A, C, D, B, PAUSE, D, C, B, A, END};
int listWHr[20] = {B, D, C, A, PAUSE, B, C, D, B, PAUSE, D, B, C, A, PAUSE, A, C, D, B, END};

int listWCh[2] = {B, A};
int listWChLen = 2;
float listWChPeriod = 1.2;
////////////////////////////////


int *listQ1;
int *listHf;
int *listQ3;
int *listHr;

int *listCh;
int listChLen;
float listChPeriod;

int pIdx = 0;
int *pList;
int pLen = 0;
Ticker ringer;
bool isRinging = false;
int doHour = HR_NONE;
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
  bool mut = instance->mute.dynMute!=0;
  sprintf(s, "[%d %d %d]", hourCount, mm, (mm-mmDevug));
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
    checking = false;
    return;
  }
  
  int mins = minute();
  bool isHour = mins == 0;// || (mins>0 && mins<=10);
  bool isHalf = mins == 30;
  bool is1Qtr = mins == 15;
  bool is3Qtr = mins == 45;

  if (isHour || isHalf || is1Qtr || is3Qtr)
    doAllRing(isHour, isHalf, is1Qtr, is3Qtr);
    
  checking = false;
}


void setHourLists(bool westm) {
  if (westm) {
    listQ1 = listWQ1;
    listHf = listWHf;
    listQ3 = listWQ3;
    listHr = listWHr;
    listCh = listWCh;
    listChLen = listWChLen;
    listChPeriod = listWChPeriod;
  } else {
    listQ1 = listRQ1;
    listHf = listRHf;
    listQ3 = listRQ3;
    listHr = listRHr;
    listCh = listRCh;
    listChLen = listRChLen;
    listChPeriod = listRChPeriod;
  }
}

void Ring::doAllRing(bool hr, bool hf, bool q1, bool q3) {
  isRinging = hr || hf || q1 || q3;
  setHourLists(settings.clock_mode == CLOCK_WESTM);
  if (settings.clock_mode == CLOCK_ALL || settings.clock_mode == CLOCK_WESTM)
    doRing(hr, hf, q1, q3);
  else if (settings.clock_mode == CLOCK_HALF)
    doRing(hr, hf);
  else if (settings.clock_mode == CLOCK_HOUR)
    doRing(hr);
}



int Ring::findHrType() {
  if (settings.clock_mode == CLOCK_ALL || settings.clock_mode == CLOCK_WESTM)
    return HR_ALL;
  else if (settings.clock_mode == CLOCK_HALF)
    return HR_HALF;
  else
    return HR_HOUR;
}

void Ring::doRing(bool hr, bool hf, bool q1, bool q3) {
  doHour = hr ? findHrType() : HR_NONE;
  if (q1)
    play(listQ1);
  else if (q3)
    play(listQ3);
  else if (hf)
    play(listHf);
  else if (hr)
    play(listHr);
}

void Ring::doRing(bool hr, bool hf) {
  doHour = hr ? findHrType() : HR_NONE;
  if (hf)
    play(listHHf);
  else if (hr)
    play(listHHr);
}


void Ring::doRing(bool hr) {
  doHour = hr ? findHrType() : HR_NONE;
  play(listHHr);
}

// Play this list of notes
// List should end with END
void Ring::play(int p[]) {
  pIdx = 0;
  pList = p;
  // Find len
  pLen = 30; // 'default'
  for (int i=0; i<30; i++) {
    if (p[i] == END) {
      pLen = i;
      break;
    }
  }
  //Serial.print("Play Len ");
  //Serialx.println(ll);
  // Play list items every 0.7
  ringer.attach(0.7, ding);
}

void Ring::ding() {
  //  Serial.print("Ding ");
  //  Serial.print(pIdx);
  //  Serial.print(" ");
  // Check if we are at last in list
  if (pIdx>=pLen) {
    ringer.detach();
    // Check if we need to play hour, too
    if (doHour != HR_NONE) {
      int hh = hour();
      if (hh>12) hh -= 12;
      if (hh==0) hh = 12;
      playHour(hh);
    } else 
        isRinging = false;
  } else {
    // Play current selected note in list
    //instance->Debug.print("Chime ");
    //instance->Debug.println(bellToChar(pList[pIdx]));
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
    ringer.attach(listChPeriod, hourDing);
    }
}

void playHour(int h) {
  instance->logs[h-1] = String(h)+" ";
  hourCount = h;
  delCount = 2;
  if (instance->settings.clock_mode == CLOCK_HOUR)
    // just play hour, so play it right away
    ringer.attach(listChPeriod, hourDing);
  else
    ringer.attach(1.0, delay1);
}


void hourDing() {
  instance->printdbgLog();
  if (hourCount == 0) {
    ringer.detach();
    isRinging = false;
  } else {
    if (instance->mute.dynMute==0) // maybe someone pressed mute?
      actuator.play(listCh, listChLen);
    hourCount--;
  }
}
