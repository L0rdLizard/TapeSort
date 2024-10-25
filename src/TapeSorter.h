#ifndef TAPESORTER_H
#define TAPESORTER_H

#include "TapeDevice.h"
#include <vector>
#include <string>
#include <queue>
#include <stdexcept>
#include <memory>

class TapeSorter {
public:
    TapeSorter(TapeDevice& inputTape, TapeDevice& outputTape, size_t memoryLimit);
    void sort();
private:
    TapeDevice& inputTape;
    TapeDevice& outputTape;
    size_t memoryLimit;
    std::vector<std::unique_ptr<TapeDevice>> tempTapes;

    // std::string createTempFile(const std::vector<int>& buffer);
    void createTempTape(const std::vector<int>& buffer);
    // void mergeTempFiles(const std::vector<std::string>& tempFiles);
    void mergeTempTapes();
};

#endif // TAPESORTER_H