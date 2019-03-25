#include <Arduino.h>
#include <TimeLib.h>
#include <Ticker.h>
#include "Ring.h"
#include "PinDefs.h"

int A = BELL_A_PIN;
int B = BELL_B_PIN;
int C = BELL_C_PIN;
int END = -100;

int listQ1[5] = {C, C, -1, A, END};
int listHf[6] = {B, B, -1, A, C, END};
int listQ3[7] = {A, A, -1, C, B, C, END};
int listHr[6] = {C, B, C, B, A, END};
int listCh[2] = {C, A};

int listHHf[2] = {B, END};
int listHHr[1] = {END};
int listHCh[3] = {A, C, END};


int pIdx = 0;
int *pList;
int pLen = 0;
    Ticker ringer;
    Ticker toggle;
bool isRinging = false;
    int doHour = HR_NONE;
int hourCount;
bool skipOne = false;

void playHour(int h);
    void hourDing();


Ring::Ring(Mute &m, Settings &s) : mute(m), settings(s){ }

void Ring::checkForRing() {
  static int last = -100;

  int secs = second();
  if (mute.dynMute>0 && (secs >58 || secs <5)) {
    mute.dynMute -= 1;
    Serial.print("Dec MUte ");
    Serial.println(mute.dynMute);
  }

  // Check every 5 seconds if we need to ring
  //   Are we ringing?
  //   Is it less than 5 seconds? (close to the minute)
  //   Is it 0 minutes? (The hour)
  //   Is it 30 minutes?
  //   Is it 15 or 45 minutes?

  if (mute.calcCurMute()!=0) return;
  if (isRinging) return;

  // only within 6 seconds after the hour
  // or we already rang, so skip for now
  if (secs>6 || last>=0) {
    last = -100; // Next time will be OK
    return;
  }
  last = secs; // 0-59

  int mins = minute();
  bool isHour = mins == 0;
  bool isHalf = mins == 30;
  bool is1Qtr = mins == 15;
  bool is3Qtr = mins == 45;

  if (!(isHour || isHalf || is1Qtr || is3Qtr)) return;

  doAllRing(isHour, isHalf, is1Qtr, is3Qtr);
}

void Ring::doAllRing(bool hr, bool hf, bool q1, bool q3) {
  if (settings.clock_mode == CLOCK_ALL)
    doRing(hr, hf, q1, q3);
  else if (settings.clock_mode == CLOCK_HALF)
    doRing(hr, hf);
  else if (settings.clock_mode == CLOCK_HOUR)
    doRing(hr);
}



int Ring::findHrType() {
  if (settings.clock_mode == CLOCK_ALL)
    return HR_ALL;
  else if (settings.clock_mode == CLOCK_HALF)
    return HR_HALF;
  else
    return HR_HOUR;
}

void Ring::doRing(bool hr, bool hf, bool q1, bool q3) {
  isRinging = true;
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
  isRinging = true;
  doHour = hr ? findHrType() : HR_NONE;
  if (hf)
    play(listHHf);
  else if (hr)
    play(listHHr);
}

void Ring::doRing(bool hr) {
  isRinging = true;
  doHour = hr ? findHrType() : HR_NONE;
  play(listHHr);
}


void Ring::play(int p[]) {
  pIdx = 0;
  pList = p;
  // Find len
  pLen = 20; // 'default'
  for (int i=0; i<20; i++) {
    if (p[i] == END) {
      pLen = i;
      break;
    }
  }
  //Serial.print("Play Len ");
  //Serialx.println(ll);
  ringer.attach(0.7, ding);
}

void Ring::ding() {
  //  Serial.print("Ding ");
  //  Serial.print(pIdx);
  //  Serial.print(" ");
  if (pIdx>=pLen) {
    ringer.detach();
    isRinging = false;
    if (doHour != HR_NONE) {
      int hh = hour();
      if (hh>12) hh -= 12;
      if (hh==0) hh = 12;
      playHour(hh);
    }
  } else
    toggle.attach(SOLEN_TIME, togg);
}

void Ring::togg() {
  //toggle bell
  int state = 0;
  int pin = pList[pIdx];
  //  Serial.print("pin ");
  //  Serial.print(pin);
  //  Serial.println(" ");
  if (pin>=0) { // pin -1 means pause
    state = digitalRead(pin);  // get the current state of GPI pin
    digitalWrite(pin, !state);     // set pin to the opposite state
  }
  if (state==1 || pin<0) {
    toggle.detach();
    pIdx++;
  }
}

void playHour(int h) {
  isRinging = true;
  hourCount = h;
  skipOne = true;
  ringer.attach(1.0, hourDing);
}

void hourDing() {
  if (hourCount == 0) {
    ringer.detach();
    isRinging = false;
  } else if (skipOne) {
    // make delay by no ringing first time
    skipOne = false;
  } else {
    toggle.attach(SOLEN_TIME_2, Ring::multiTogg);
    hourCount--;
  }
}


void Ring::multiTogg() {
  //toggle multiple bell
  int state = digitalRead(listCh[0]);  // get the current state of GPI pin
  for (int i=0; i<2; i++)
    digitalWrite(listCh[i], !state);     // set pin to the opposite state
  if (state == 1)
    toggle.detach();
}

