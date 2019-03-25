#include <TimeLib.h>
#include "Mute.h"


int Mute::calcCurMute() {
  int res = dynMute;
  // calculations in "minutes since 1970"
  long nowM = now() / 60; // minutes
  long calcMute = nowM + dynMute;
  if (mute_enab) {
    long nextMuteStart = calcMuteStart() + nowM; 
    long nextMuteEnd   = calcMuteEnd()   + nowM;
    if (nextMuteStart < nextMuteEnd) { 
      if (calcMute >= nextMuteStart && calcMute < nextMuteEnd)
        res = (nextMuteEnd - nowM);
    } else {
      if (calcMute >= nextMuteStart || calcMute < nextMuteEnd)
        res = (nextMuteEnd - nowM);
    }
  }
  return res;
}

long Mute::calcMuteStart() {
  // calculations in "minutes since 1970"
  int S = currMinFromMidnight(mute_start);
  int C = hour() * 60 + minute();
  if (C<S)
    return S - C;
  else
    return 1440 - C + S;
}

long Mute::calcMuteEnd() {
  // calculations in "minutes since 1970"
  int E = currMinFromMidnight(mute_end);
  int C = hour() * 60 + minute();
  if (E<C)
    return 1440 - C + E;
  else
    return E - C;
}

int Mute::currMinFromMidnight(int muteV) {
  int muteH = muteV / 10;      // 145 -> 14
  int muteM = (muteV % 10)*6;  // 145 -> 5 -> 30
  return muteH * 60 + muteM;
}


