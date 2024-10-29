#include "TapeDevice.h"
#include "Config.h"
#include "utils/FileUtils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>

TapeDevice::TapeDevice(const std::string& filename, std::unordered_map<std::string, int> delays)
    : currentPos(0), tapeFilename(filename), delays(delays) {

    length = FileUtils::convertTextToBinary(tapeFilename);
    if (length == 0) {
        throw std::runtime_error("Input Txt file is empty or not exists");
    }

    std::string path = "data/" + tapeFilename + ".bin";
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.close();
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

TapeDevice::TapeDevice(const std::string& filename, size_t length, std::unordered_map<std::string, int> delays)
    : currentPos(0), length(length), tapeFilename(filename), delays(delays) {
    
    std::string path = "data/" + tapeFilename + ".bin";
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.close();
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
    } else {
        file.close();
        file.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
        file.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    // if (fileSize != length * sizeof(int)) {
    //     throw std::runtime_error("File size does not match the expected tape length");
    // }

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
    FileUtils::convertBinaryToText(tapeFilename);
}

size_t TapeDevice::getCurrentPosition() {
    return currentPos;
}

size_t TapeDevice::getLength() {
    return length;
}

int TapeDevice::getCurrentCell_impl() {
    int value;
    file.seekg(currentPos * sizeof(int), std::ios::beg);
    file.read(reinterpret_cast<char*>(&value), sizeof(int));

    if (!file) {
        throw std::runtime_error("Failed to read from tape");
    }

    return value;
}

void TapeDevice::changeCurrentCell_impl(int value) {
    file.seekp(currentPos * sizeof(int), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&value), sizeof(int));

    if (!file) {
        throw std::runtime_error("Failed to write to tape");
    }

    file.flush();
}

void TapeDevice::moveToNextCell_impl() {
    if (currentPos + 1 >= length) {
        throw std::out_of_range("End of tape reached");
    }
    ++currentPos;
}

void TapeDevice::moveToPreviousCell_impl() {
    if (currentPos == 0) {
        throw std::out_of_range("Beginning of tape reached");
    }
    --currentPos;
}

void TapeDevice::rewind_impl() {
    currentPos = 0;
}

Task<int> TapeDevice::getCurrentCell() {
    return Task<int>{ std::async(std::launch::async, [this]() { return getCurrentCell_impl(); }), delays["read_delay"] };
}

Task<void> TapeDevice::changeCurrentCell(int value) {
    return Task<void>{ std::async(std::launch::async, [this, value]() { changeCurrentCell_impl(value); }), delays["write_delay"] };
}

Task<void> TapeDevice::moveToNextCell() {
    return Task<void>{ std::async(std::launch::async, [this]() { moveToNextCell_impl(); }), delays["shift_delay"] };
}

Task<void> TapeDevice::moveToPreviousCell() {
    return Task<void>{ std::async(std::launch::async, [this]() { moveToPreviousCell_impl(); }), delays["shift_delay"] };
}

Task<void> TapeDevice::rewind() {
    if (currentPos == 0) {
        return Task<void>{ std::future<void>(), 0 };
    }
    return Task<void>{ std::async(std::launch::async, [this]() { rewind_impl(); }), delays["rewind_delay"] };
}