#ifndef TAPESORTER_H
#define TAPESORTER_H

#include "TapeDevice.h"

class TapeSorter {
public:
    TapeSorter();
    void sortTape(TapeDevice& d1, TapeDevice& d2);
    ~TapeSorter();
};

#endif // TAPESORTER_H