// ID
// Holds info about this hardware

#ifndef id_h
#define id_h

class ID {
  public:
    ID();
    String lastOfMac();
    String clientMac;
    const char *cMac;
    String myIP();
  private:
    String mac2Str(byte ar[], int len);
};

#endif
