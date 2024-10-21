#include "TapeDevice.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>

TapeDevice::TapeDevice(const std::string& filename, size_t length)
    : length(length), currentPos(0) {
    readConfig();
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

TapeDevice::TapeDevice(const std::string& filename) : currentPos(0) {
    readConfig();
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

void TapeDevice::readConfig() {
    std::ifstream file("../config/delays.cfg");

    if (!file) {
        std::cerr << "Failed to open configuration file" << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) continue;

        std::string key = line.substr(0, delimiterPos - 1);
        int value = std::stoi(line.substr(delimiterPos + 1));

        std::cout << key << ": " << value << std::endl;
        delays[key] = value;
    }
    std::cout << std::endl;
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
