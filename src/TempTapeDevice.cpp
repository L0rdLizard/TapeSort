#include "TempTapeDevice.h"
#include "Config.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>

TempTapeDevice::TempTapeDevice(const std::string& filename, size_t length, std::unordered_map<std::string, int> delays)
    : length(length), currentPos(0), tempFilename(filename), delays(delays) {
    
    std::string path = "tmp/" + filename + ".bin";
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file) {
            throw std::runtime_error("Failed to create temp tape file");
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

    file.seekg(0, std::ios::beg);
}

TempTapeDevice::TempTapeDevice(const std::string& filename, std::unordered_map<std::string, int> delays)
    : currentPos(0), tempFilename(filename), delays(delays) {

    std::string path = "tmp/" + filename + ".bin";
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

void TempTapeDevice::readConfig(const std::string& configFilename) {
    Config config(configFilename);

    delays = config.loadConfig();
}

void TempTapeDevice::simulateDelay(int delayMs) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
}

TempTapeDevice::~TempTapeDevice() {
    if (file.is_open()) {
        file.close();
    }
    std::string path = "tmp/" + tempFilename + ".bin";
    std::remove(path.c_str());
}

size_t TempTapeDevice::getCurrentPosition() {
    return currentPos;
}

size_t TempTapeDevice::getLength() {
    return length;
}

int TempTapeDevice::getCurrentCell_impl() {
    int value;
    file.seekg(currentPos * sizeof(int), std::ios::beg);
    file.read(reinterpret_cast<char*>(&value), sizeof(int));

    if (!file) {
        throw std::runtime_error("Failed to read from tape");
    }

    return value;
}

void TempTapeDevice::changeCurrentCell_impl(int value) {
    file.seekp(currentPos * sizeof(int), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&value), sizeof(int));

    if (!file) {
        throw std::runtime_error("Failed to write to tape");
    }

    file.flush();
}

void TempTapeDevice::moveToNextCell_impl() {
    if (currentPos + 1 >= length) {
        throw std::out_of_range("End of tape reached");
    }
    ++currentPos;
}

void TempTapeDevice::moveToPreviousCell_impl() {
    if (currentPos == 0) {
        throw std::out_of_range("Beginning of tape reached");
    }
    --currentPos;
}

void TempTapeDevice::rewind_impl() {
    currentPos = 0;
}

Task<int> TempTapeDevice::getCurrentCell() {
    return Task<int>{ std::async(std::launch::async, [this]() { return getCurrentCell_impl(); }), delays["read_delay"] };
}

Task<void> TempTapeDevice::changeCurrentCell(int value) {
    return Task<void>{ std::async(std::launch::async, [this, value]() { changeCurrentCell_impl(value); }), delays["write_delay"] };
}

Task<void> TempTapeDevice::moveToNextCell() {
    return Task<void>{ std::async(std::launch::async, [this]() { moveToNextCell_impl(); }), delays["shift_delay"] };
}

Task<void> TempTapeDevice::moveToPreviousCell() {
    return Task<void>{ std::async(std::launch::async, [this]() { moveToPreviousCell_impl(); }), delays["shift_delay"] };
}

Task<void> TempTapeDevice::rewind() {
    if (currentPos == 0) {
        return Task<void>{ std::future<void>(), 0 };
    }
    return Task<void>{ std::async(std::launch::async, [this]() { rewind_impl(); }), delays["rewind_delay"] };
}