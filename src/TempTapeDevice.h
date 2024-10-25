#ifndef TEMP_TAPE_DEVICE_H
#define TEMP_TAPE_DEVICE_H

#include "ITapeDevice.h"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>

class TempTapeDevice : public ITapeDevice {
private:
    std::fstream file;
    size_t length;
    size_t currentPos;
    std::unordered_map<std::string, int> delays;
    std::string tempFilename;

public:
    TempTapeDevice(const std::string& filename, size_t length, const std::string& configFilename);

    TempTapeDevice(const std::string& filename, const std::string& configFilename);

    ~TempTapeDevice();

    int getCurrentCell() override;

    size_t getCurrentPosition() override;

    size_t getLength() override;

    void changeCurrentCell(int value) override;

    void moveToNextCell() override;

    void moveToPreviousCell() override;

    void rewind() override;

    void readConfig(const std::string& configFilename) override;

    void simulateDelay(int delayMs) override;
};

#endif // TEMP_TAPE_DEVICE_H
