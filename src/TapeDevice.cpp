#include "TapeDevice.h"
#include "Config.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>

TapeDevice::TapeDevice(const std::string& filename, size_t length, const std::string& configFilename)
    : length(length), currentPos(0) {
    readConfig(configFilename);
    std::string path = "../data/" + filename;
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file) {
            throw std::runtime_error("Failed to create tape file");
        }

        int zero = 0;
        for (size_t i = 0; i < length; ++i) {
            file.write(reinterpret_cast<const char*>(&zero), sizeof(int));
        }
        file.flush();

        file.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to reopen tape file");
        }
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    if (fileSize != length * sizeof(int)) {
        throw std::runtime_error("File size does not match the expected tape length");
    }

    file.seekg(0, std::ios::beg);
}

TapeDevice::TapeDevice(const std::string& filename, const std::string& configFilename) : currentPos(0) {
    readConfig(configFilename);
    std::string path = "../data/" + filename;
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Tape file not exists. To create a new file - specify length");
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();

    if (fileSize == 0) {
        throw std::runtime_error("Tape file is empty");
    }
    if (fileSize % sizeof(int) != 0) {
        throw std::runtime_error("Invalid file size: not aligned with integer size");
    }

    length = fileSize / sizeof(int);
    std::cout << "File length is: " << length << std::endl;
    file.seekg(0, std::ios::beg);
}

void TapeDevice::readConfig(const std::string& configFilename) {
    Config config(configFilename);

    delays = config.loadConfig();
}

void TapeDevice::simulateDelay(int delayMs) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
}

TapeDevice::~TapeDevice() {
    if (file.is_open()) {
        file.close();
    }
}

int TapeDevice::getCurrentCell() {
    int value;
    file.seekg(currentPos * sizeof(int), std::ios::beg);
    file.read(reinterpret_cast<char*>(&value), sizeof(int));

    if (!file) {
        throw std::runtime_error("Failed to read from tape");
    }

    return value;
}

size_t TapeDevice::getCurrentPosition() {
    return currentPos;
}

size_t TapeDevice::getLength() {
    return length;
}

void TapeDevice::changeCurrentCell(int value) {
    file.seekp(currentPos * sizeof(int), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&value), sizeof(int));

    if (!file) {
        throw std::runtime_error("Failed to write to tape");
    }

    file.flush();
}

void TapeDevice::moveToNextCell() {
    if (currentPos + 1 >= length) {
        throw std::out_of_range("End of tape reached");
    }
    ++currentPos;
}

void TapeDevice::moveToPreviousCell() {
    if (currentPos == 0) {
        throw std::out_of_range("Beginning of tape reached");
    }
    --currentPos;
}

void TapeDevice::rewind() {
    currentPos = 0;
}
