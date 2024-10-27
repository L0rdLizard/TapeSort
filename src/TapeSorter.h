#ifndef TAPESORTER_H
#define TAPESORTER_H

#include "TapeDevice.h"
#include "TempTapeDevice.h"
#include "utils/TimeManager.h"
#include <vector>
#include <string>
#include <queue>
#include <stdexcept>
#include <memory>

class TapeSorter {
public:
    TapeSorter(TapeDevice& inputTape, TapeDevice& outputTape, size_t memoryLimit, 
    TimeManager& timeManager, std::unordered_map<std::string, int> delays);

    void sort();
private:
    TapeDevice& inputTape;
    TapeDevice& outputTape;
    size_t memoryLimit;
    std::vector<std::unique_ptr<TempTapeDevice>> tempTapes;
    TimeManager& timeManager;
    std::unordered_map<std::string, int> delays;

    void createTempTape(const std::vector<int>& buffer);
    void mergeTempTapes();
};

#endif // TAPESORTER_H