#include <Arduino.h>
#include "Chime.h"
#include "Pin.h"

constexpr int Chime::null_ch[];
constexpr int Chime::halfSHf[];
constexpr int Chime::hourHrStrk[];
constexpr int Chime::halfMHf[];
constexpr int Chime::halfSHrStrk[];
constexpr int Chime::westQ1[];
constexpr int Chime::westHf[];
constexpr int Chime::westQ3[];
constexpr int Chime::westHr[];
constexpr int Chime::westHrStrk[];
constexpr int Chime::qrt3_Q1[];
constexpr int Chime::qrt3_Hf[];
constexpr int Chime::qrt3_Q3[];
constexpr int Chime::qrt3_Hr[];
constexpr int Chime::qrt3_HrStrk[];
constexpr int Chime::qrt2_Q1[];
constexpr int Chime::qrt2_Hf[];
constexpr int Chime::qrt2_Q3[];
constexpr int Chime::qrt2_Hr[];
constexpr int Chime::qrt2_HrStrk[];
constexpr int Chime::halfMHrStrk[];
constexpr int Chime::shipBase[];


char Chime::bellToChar(int b) {
  if (b == A) return 'A';
  if (b == B) return 'B';
  if (b == C) return 'C';
  if (b == D) return 'D';
  if (b == E) return 'E';
  if (b == F) return 'F';
  if (b == G) return 'G';
  if (b == END) return 'n';
  if (b == PAUSE) return '.';
  return '?';
}



Chime Chime::westminster() {
  Chime res;
  res.Q1 = westQ1;
  res.Hf = westHf;
  res.Q3 = westQ3;
  res.Hr = westHr;
  res.strikePeriod = 1.0;
  res.strikeHr = westHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 1.2;
  return res;
}

Chime Chime::quarters_3() {
  Chime res;
  res.Q1 = qrt3_Q1;
  res.Hf = qrt3_Hf;
  res.Q3 = qrt3_Q3;
  res.Hr = qrt3_Hr;
  res.strikePeriod = 0.7;
  res.strikeHr = qrt3_HrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::quarters_2() {
  Chime res;
  res.Q1 = qrt2_Q1;
  res.Hf = qrt2_Hf;
  res.Q3 = qrt2_Q3;
  res.Hr = qrt2_Hr;
  res.strikePeriod = 0.7;
  res.strikeHr = qrt2_HrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::halfs_3() {
  Chime res;
  res.Q1 = null_ch;
  res.Hf = halfMHf;
  res.Q3 = null_ch;
  res.Hr = null_ch;
  res.strikePeriod = 0.7;
  res.strikeHr = halfMHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::halfs_1_2() {
  Chime res;
  res.Q1 = null_ch;
  res.Hf = halfSHf;
  res.Q3 = null_ch;
  res.Hr = null_ch;
  res.strikePeriod = 0.7;
  res.strikeHr = halfSHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::hours() {
  Chime res;
  res.Q1 = null_ch;
  res.Hf = null_ch;
  res.Q3 = null_ch;
  res.Hr = null_ch;
  res.strikePeriod = 0.0;
  res.strikeHr = hourHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

String Chime::toString() {
  return String(" q1 ") + (Q1 != null_ch) + "  hf " + (Hf != null_ch) + "  q3 " + (Q3 != null_ch) + "  hr "+(Hr != null_ch);
}
