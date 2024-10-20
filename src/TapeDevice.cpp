#include "ITapeDevice.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <iostream>

class TapeDevice : public ITapeDevice {
private:
    std::fstream file;
    size_t length;
    size_t currentPos;

public:
    TapeDevice(const std::string& filename, size_t length) : length(length), currentPos(0) {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
            if (!file) {
                throw std::runtime_error("Failed to create tape file");
            }

            int zero = 0;
            for (size_t i = 0; i < length; ++i) {
                file.write(reinterpret_cast<const char*>(&zero), sizeof(int));
            }
            file.flush();

            file.close();
            file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
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

    TapeDevice(const std::string& filename) : currentPos(0) {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
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


    ~TapeDevice() {
        if (file.is_open()) {
            file.close();
        }
    }

    int getCurrentCell() override {
        int value;
        file.seekg(currentPos * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char*>(&value), sizeof(int));

        if (!file) {
            throw std::runtime_error("Failed to read from tape");
        }

        return value;
    }

    void changeCurrentCell(int value) override {
        file.seekp(currentPos * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<const char*>(&value), sizeof(int));

        if (!file) {
            throw std::runtime_error("Failed to write to tape");
        }

        file.flush();
    }

    void moveToNextCell() override {
        if (currentPos + 1 >= length) {
            throw std::out_of_range("End of tape reached");
        }
        ++currentPos;
    }

    void moveToPreviousCell() override {
        if (currentPos == 0) {
            throw std::out_of_range("Beginning of tape reached");
        }
        --currentPos;
    }
};