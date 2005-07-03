#ifndef sharedobjects
#define sharedobjects

#include "configure.h"

extern configure * conf;

string ntos(unsigned long k);
string numtostring(unsigned long k);
FXColor readcolor(string col);
FXColor readcolor2(string col);


#endif
