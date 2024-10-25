#include "TapeSorter.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <chrono>

TapeSorter::TapeSorter(TapeDevice& inputTape, TapeDevice& outputTape, size_t memoryLimit)
    : inputTape(inputTape), outputTape(outputTape), memoryLimit(memoryLimit) {
        inputTape.rewind();
        outputTape.rewind();
    }

void TapeSorter::sort() {
    auto start = std::chrono::high_resolution_clock::now();
    size_t chunkSize = memoryLimit;
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

            createTempTape(buffer);
        }
        catch (const std::out_of_range&) {
            break;
        }
    }
    
    mergeTempTapes();

    inputTape.rewind();
    outputTape.rewind();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    // std::cout << "Total sort time: " << elapsed.count() << " seconds" << std::endl;
}

void TapeSorter::createTempTape(const std::vector<int>& buffer) {
    auto start = std::chrono::high_resolution_clock::now();

    static int tempTapeIndex = 0;
    std::string tempTapeName = "temp_tape_" + std::to_string(tempTapeIndex++);

    auto tempTape = std::make_unique<TempTapeDevice>(tempTapeName, buffer.size(), "delays.cfg");
    tempTape->rewind();

    for (int value : buffer) {
        tempTape->changeCurrentCell(value);
        if (tempTape->getCurrentPosition() < tempTape->getLength() - 1) {
            tempTape->moveToNextCell();
        }
    }
    tempTape->rewind();
    tempTapes.push_back(std::move(tempTape));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    // std::cout << "Time for creating temp tape: " << elapsed.count() << " seconds" << std::endl;
}

void TapeSorter::mergeTempTapes() {
    auto start = std::chrono::high_resolution_clock::now();

    struct TapeElement {
        int value;
        size_t tapeIndex;
        bool operator>(const TapeElement& other) const {
            return value > other.value;
        }
    };

    std::priority_queue<TapeElement, std::vector<TapeElement>, std::greater<TapeElement>> minHeap;
    // TODO: check chunk size
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

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    // std::cout << "Time for merging temp tapes: " << elapsed.count() << " seconds" << std::endl;
}
