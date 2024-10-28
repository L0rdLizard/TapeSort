#include "TapeSorter.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <chrono>

TapeSorter::TapeSorter(TapeDevice& inputTape, TapeDevice& outputTape, size_t memoryLimit,
    TimeManager& timeManager, std::unordered_map<std::string, int> delays)
    : inputTape(inputTape), outputTape(outputTape), memoryLimit(memoryLimit), timeManager(timeManager), delays(delays) {
        inputTape.rewind();
        outputTape.rewind();
    }

void TapeSorter::sort() {
    size_t chunkSize = memoryLimit;
    std::vector<int> buffer;

    while (inputTape.getCurrentPosition() < inputTape.getLength() - 1) {
        buffer.clear();
        try {
            for (size_t i = 0; i < chunkSize && inputTape.getCurrentPosition() < inputTape.getLength() - 1; ++i) {
                // buffer.push_back(inputTape.getCurrentCell_impl());
                buffer.push_back(timeManager.run_single_task(inputTape.getCurrentCell()));
                inputTape.moveToNextCell();
            }
            if (inputTape.getCurrentPosition() == inputTape.getLength() - 1){
                // buffer.push_back(inputTape.getCurrentCell_impl());
                buffer.push_back(timeManager.run_single_task(inputTape.getCurrentCell()));
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
}

void TapeSorter::createTempTape(const std::vector<int>& buffer) {
    static int tempTapeIndex = 0;
    std::string tempTapeName = "temp_tape_" + std::to_string(tempTapeIndex++);

    auto tempTape = std::make_unique<TempTapeDevice>(tempTapeName, buffer.size(), delays);
    tempTape->rewind();

    for (int value : buffer) {
        tempTape->changeCurrentCell(value);
        if (tempTape->getCurrentPosition() < tempTape->getLength() - 1) {
            timeManager.run_single_task(tempTape->moveToNextCell());
        }
    }
    timeManager.run_single_task(tempTape->rewind());
    tempTapes.push_back(std::move(tempTape));
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

    size_t activeTapeCount = std::min(memoryLimit, tempTapes.size());
    std::vector<size_t> activeTapes;

    for (size_t i = 0; i < activeTapeCount; ++i) {
        Task<int> task(tempTapes[i]->getCurrentCell());

        timeManager.add_task(std::move(task));
    }
    timeManager.run_tasks();

    const auto& results = timeManager.get_results();
    for (size_t i = 0; i < results.size(); ++i) {
        minHeap.push({ results[i], i });
        activeTapes.push_back(i);
    }

    while (!minHeap.empty()) {
        TapeElement smallest = minHeap.top();
        minHeap.pop();

        timeManager.run_single_task(outputTape.changeCurrentCell(smallest.value));
        if (outputTape.getCurrentPosition() < outputTape.getLength() - 1) {
            timeManager.run_single_task(outputTape.moveToNextCell());
        }

        if (tempTapes[smallest.tapeIndex]->getCurrentPosition() < tempTapes[smallest.tapeIndex]->getLength() - 1) {
            timeManager.run_single_task(tempTapes[smallest.tapeIndex]->moveToNextCell());
            int nextValue = timeManager.run_single_task(tempTapes[smallest.tapeIndex]->getCurrentCell());
            minHeap.push({ nextValue, smallest.tapeIndex });
        } else {
            activeTapes.erase(std::remove(activeTapes.begin(), activeTapes.end(), smallest.tapeIndex), activeTapes.end());

            if (activeTapes.size() < activeTapeCount && activeTapeCount < tempTapes.size()) {
                size_t nextTapeIndex = activeTapeCount++;
                int value = timeManager.run_single_task(tempTapes[nextTapeIndex]->getCurrentCell());
                minHeap.push({ value, nextTapeIndex });
                activeTapes.push_back(nextTapeIndex);
            }
        }
    }

    outputTape.rewind();
}

