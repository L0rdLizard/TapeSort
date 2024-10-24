#include "TapeSorter.h"
#include <fstream>
#include <algorithm>
#include <iostream>

TapeSorter::TapeSorter(TapeDevice& inputTape, TapeDevice& outputTape, size_t memoryLimit)
    : inputTape(inputTape), outputTape(outputTape), memoryLimit(memoryLimit) {}

void TapeSorter::sort() {
    size_t chunkSize = memoryLimit / sizeof(int);
    std::vector<int> buffer;

    std::vector<std::string> tempFiles;

    while (true) {
        buffer.clear();
        try {
            for (size_t i = 0; i < chunkSize && inputTape.getCurrentPosition() < inputTape.getLength(); ++i) {
                buffer.push_back(inputTape.getCurrentCell());
                inputTape.moveToNextCell();
            }
            if (buffer.empty()) break;

            std::sort(buffer.begin(), buffer.end());

            std::string tempFileName = createTempFile(buffer);
            tempFiles.push_back(tempFileName);
        }
        catch (const std::out_of_range&) {
            break;
        }
    }
    mergeTempFiles(tempFiles);

    for (const std::string& tempFile : tempFiles) {
        std::remove(tempFile.c_str());
    }
}

std::string TapeSorter::createTempFile(const std::vector<int>& buffer) {
    static int tempFileIndex = 0;
    std::string tempFileName = "../tmp/temp_tape_" + std::to_string(tempFileIndex++) + ".bin";

    std::ofstream tempFile(tempFileName, std::ios::binary);
    if (!tempFile) {
        throw std::runtime_error("Failed to create temporary file");
    }

    for (int value : buffer) {
        tempFile.write(reinterpret_cast<const char*>(&value), sizeof(int));
    }

    tempFile.close();
    return tempFileName;
}

void TapeSorter::mergeTempFiles(const std::vector<std::string>& tempFiles) {
    std::vector<std::ifstream> tempStreams(tempFiles.size());
    for (size_t i = 0; i < tempFiles.size(); ++i) {
        tempStreams[i].open(tempFiles[i], std::ios::binary);
        if (!tempStreams[i]) {
            throw std::runtime_error("Failed to open temporary file: " + tempFiles[i]);
        }
    }

    struct TapeElement {
        int value;
        size_t fileIndex;
        bool operator>(const TapeElement& other) const {
            return value > other.value;
        }
    };

    std::priority_queue<TapeElement, std::vector<TapeElement>, std::greater<TapeElement>> minHeap;

    for (size_t i = 0; i < tempFiles.size(); ++i) {
        int value;
        tempStreams[i].read(reinterpret_cast<char*>(&value), sizeof(int));
        if (tempStreams[i]) {
            minHeap.push({ value, i });
        }
    }

    while (!minHeap.empty()) {
        TapeElement smallest = minHeap.top();
        minHeap.pop();

        outputTape.changeCurrentCell(smallest.value);
        if (outputTape.getCurrentPosition() < outputTape.getLength() - 1) {
            outputTape.moveToNextCell();
        }

        int value;
        tempStreams[smallest.fileIndex].read(reinterpret_cast<char*>(&value), sizeof(int));
        if (tempStreams[smallest.fileIndex]) {
            minHeap.push({ value, smallest.fileIndex });
        }
    }

    for (auto& tempStream : tempStreams) {
        tempStream.close();
    }
}
