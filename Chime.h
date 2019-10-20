// Chime
// Definition of chime style
#ifndef chime_h
#define chime_h

class Chime {
  public:
    static int END;
    static int PAUSE;
    static char bellToChar(int b);

    int *Q1;
    int *Hf;
    int *Q3;
    int *Hr;
    float strikePeriod;

    int *strikeHr;
    int strikeHrLen;
    float strikeHrPeriod;

    static Chime westminster();
    static Chime quarters();
    static Chime halfsMulti();
    static Chime halfsSingle();
    static Chime hours();

    static int shipBase[];
    static float shipChPeriod;
    
    String toString();    
  private:
};

#endif
