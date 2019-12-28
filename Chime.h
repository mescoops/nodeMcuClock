// Chime
// Definition of chime style
#ifndef chime_h
#define chime_h

#include "Pin.h"
#include "PinDefs.h"

class Chime {
  public:
    static const int A = BELL_A_PIN;
    static const int B = BELL_B_PIN;
    static const int C = BELL_C_PIN;
    static const int D = BELL_D_PIN;
    static const int E = BELL_E_PIN;
    static const int F = BELL_F_PIN;
    static const int G = BELL_G_PIN;
    static const int PAUSE = -1;
    static const int END = -100;
    
    static char bellToChar(int b);

    const int *Q1;
    const int *Hf;
    const int *Q3;
    const int *Hr;
    float strikePeriod;

    const int *strikeHr;
    int strikeHrLen;
    float strikeHrPeriod;

    static Chime westminster();
    static Chime quarters_3();
    static Chime quarters_2();
    static Chime halfs_3();
    static Chime halfs_1_2();
    static Chime hours();

    static Pin NULLPIN;

    // Ship's
    static constexpr int shipBase[12] = {A, A, PAUSE, A, A, PAUSE, A, A, PAUSE, A, A, END};
    static constexpr float shipChPeriod = 0.75;
    
    String toString();    
  private:
    //////////////////////////////////
    // Westminster
    // G♯4, F♯4, E4, B3
    // D    C    B   A
    static constexpr int westQ1[5]  = {D, C, B, A, END};
    static constexpr int westHf[10] = {B, D, C, A, PAUSE, B, C, D, B, END};
    static constexpr int westQ3[15] = {D, B, C, A, PAUSE, A, C, D, B, PAUSE, D, C, B, A, END};
    static constexpr int westHr[20] = {B, D, C, A, PAUSE, B, C, D, B, PAUSE, D, B, C, A, PAUSE, A, C, D, B, END};
    // Pair of chimes for Hour chiming
    static constexpr int westHrStrk[2] = {A, B};

    // 3 bell chime on quarters
    static constexpr int qrt3_Q1[5] = {C, C, PAUSE, A, END};
    static constexpr int qrt3_Hf[6] = {B, B, PAUSE, A, C, END};
    static constexpr int qrt3_Q3[7] = {A, A, PAUSE, C, B, C, END};
    static constexpr int qrt3_Hr[6] = {C, B, C, B, A, END};
    // Pair of chimes for Hour chiming
    static constexpr int qrt3_HrStrk[2] = {A, C};

    // 2 bell chime on quarters
    static constexpr int qrt2_Q1[3] = {B, A, END};
    static constexpr int qrt2_Hf[4] = {A, B, A, END};
    static constexpr int qrt2_Q3[4] = {B, A, B, END};
    static constexpr int qrt2_Hr[5] = {B, A, A, B, END};
    // Pair of chimes for Hour chiming
    static constexpr int qrt2_HrStrk[2] = {A, B};


    static constexpr int null_ch[1] = {END};

    // Single on half, double on hour
    static constexpr int halfMHf[2] = {B, END};
    static constexpr int halfMHrStrk[2] = {A, C};

    // Single (or double) on half, single (or double) on hour
    static constexpr int halfSHf[3] = {A, B, END};
    static constexpr int halfSHrStrk[2] = {A, B};

    // Single (or double) on hour
    static constexpr int hourHrStrk[2] = {A, B};

};

#endif
