#include <Arduino.h>
#include "Chime.h"
#include "PinDefs.h"

int A = BELL_A_PIN;
int B = BELL_B_PIN;
int C = BELL_C_PIN;
int D = BELL_D_PIN;
int E = BELL_E_PIN;
int F = BELL_F_PIN;
int G = BELL_G_PIN;

int Chime::END = -100;
int Chime::PAUSE = -1;

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

//////////////////////////////////
// Westminster
// G♯4, F♯4, E4, B3
// D    C    B   A
int westQ1[] = {D, C, B, A, Chime::END};
int westHf[] = {B, D, C, A, Chime::PAUSE, B, C, D, B, Chime::END};
int westQ3[] = {D, B, C, A, Chime::PAUSE, A, C, D, B, Chime::PAUSE, D, C, B, A, Chime::END};
int westHr[] = {B, D, C, A, Chime::PAUSE, B, C, D, B, Chime::PAUSE, D, B, C, A, Chime::PAUSE, A, C, D, B, Chime::END};
// Pair of chimes for Hour chiming
int westHrStrk[] = {B, A};

// 3 bell chime on quarters
int qrtQ1[] = {C, C, Chime::PAUSE, A, Chime::END};
int qrtHf[] = {B, B, Chime::PAUSE, A, C, Chime::END};
int qrtQ3[] = {A, A, Chime::PAUSE, C, B, C, Chime::END};
int qrtHr[] = {C, B, C, B, A, Chime::END};
// Pair of chimes for Hour chiming
int qrtHrStrk[] = {C, A};

// Single on half, double on hour
int halfMHf[] = {B, Chime::END};
int halfMHrStrk[] = {A, C};

// Single (or double) on half, single (or double) on hour
int halfSHf[] = {A, B, Chime::END};
int halfSHrStrk[] = {A, B};

// Single (or double) on hour
int hourHrStrk[] = {A, B};


// Ship's
int Chime::shipBase[] = {A, A, Chime::PAUSE, A, A, Chime::PAUSE, A, A, Chime::PAUSE, A, A, Chime::END};
float Chime::shipChPeriod = 0.5;


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

Chime Chime::quarters() {
  Chime res;
  res.Q1 = qrtQ1;
  res.Hf = qrtHf;
  res.Q3 = qrtQ3;
  res.Hr = qrtHr;
  res.strikePeriod = 0.7;
  res.strikeHr = qrtHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::halfsMulti() {
  Chime res;
  res.Q1 = NULL;
  res.Hf = halfMHf;
  res.Q3 = NULL;
  res.Hr = NULL;
  res.strikePeriod = 0.7;
  res.strikeHr = halfMHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::halfsSingle() {
  Chime res;
  res.Q1 = NULL;
  res.Hf = halfSHf;
  res.Q3 = NULL;
  res.Hr = NULL;
  res.strikePeriod = 0.7;
  res.strikeHr = halfSHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

Chime Chime::hours() {
  Chime res;
  res.Q1 = NULL;
  res.Hf = NULL;
  res.Q3 = NULL;
  res.Hr = NULL;
  res.strikePeriod = 0.0;
  res.strikeHr = hourHrStrk;
  res.strikeHrLen = 2;
  res.strikeHrPeriod = 0.9;
  return res;
}

String Chime::toString() {
  return String(" q1 ") + (Q1 != NULL) + "  hf " + (Hf != NULL) + "  q3 " + (Q3 != NULL) + "  hr "+(Hr != NULL);
}
