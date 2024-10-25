#include "TapeSorter.h"
#include <fstream>
#include <algorithm>
#include <iostream>

TapeSorter::TapeSorter(TapeDevice& inputTape, TapeDevice& outputTape, size_t memoryLimit)
    : inputTape(inputTape), outputTape(outputTape), memoryLimit(memoryLimit) {
        inputTape.rewind();
        outputTape.rewind();
    }

void TapeSorter::sort() {
    size_t chunkSize = memoryLimit / sizeof(int);
    std::vector<int> buffer;

    while (inputTape.getCurrentPosition() < inputTape.getLength() - 1) {
        buffer.clear();
        try {
            for (size_t i = 0; i < chunkSize && inputTape.getCurrentPosition() < inputTape.getLength() - 1; ++i) {
                buffer.push_back(inputTape.getCurrentCell());
                inputTape.moveToNextCell();
            }
            if (inputTape.getCurrentPosition() == inputTape.getLength() - 1){
                buffer.push_back(inputTape.getCurrentCell());
            }
            if (buffer.empty()) break;

            std::sort(buffer.begin(), buffer.end());

            // std::string tempFileName = createTempFile(buffer);
            // tempFiles.push_back(tempFileName);
            createTempTape(buffer); 
        }
        catch (const std::out_of_range&) {
            break;
        }
    }
    mergeTempTapes();

    inputTape.rewind();
    outputTape.rewind();
}

void TapeSorter::createTempTape(const std::vector<int>& buffer) {
    static int tempTapeIndex = 0;
    std::string tempTapeName = "temp_tape_" + std::to_string(tempTapeIndex++) + ".bin";

    auto tempTape = std::make_unique<TapeDevice>(tempTapeName, buffer.size(), "delays.cfg");
    tempTape->rewind();

    for (int value : buffer) {
        tempTape->changeCurrentCell(value);
        if (tempTape->getCurrentPosition() < tempTape->getLength() - 1) {
            tempTape->moveToNextCell();
        }
    }
    tempTape->rewind();
    tempTapes.push_back(std::move(tempTape));
}

void TapeSorter::mergeTempTapes() {
    struct TapeElement {
        int value;
        size_t tapeIndex;
        bool operator>(const TapeElement& other) const {
            return value > other.value;
        }
    };

    std::priority_queue<TapeElement, std::vector<TapeElement>, std::greater<TapeElement>> minHeap;

    for (size_t i = 0; i < tempTapes.size(); ++i) {
        int value = tempTapes[i]->getCurrentCell();
        minHeap.push({ value, i });
    }

    while (!minHeap.empty()) {
        TapeElement smallest = minHeap.top();
        minHeap.pop();

        outputTape.changeCurrentCell(smallest.value);
        if (outputTape.getCurrentPosition() < outputTape.getLength() - 1) {
            outputTape.moveToNextCell();
        }

        if (tempTapes[smallest.tapeIndex]->getCurrentPosition() < tempTapes[smallest.tapeIndex]->getLength() - 1) {
            tempTapes[smallest.tapeIndex]->moveToNextCell();
            int nextValue = tempTapes[smallest.tapeIndex]->getCurrentCell();
            minHeap.push({ nextValue, smallest.tapeIndex });
        }
    }

    outputTape.rewind();
}
