#ifndef TAP_DEVICE_H
#define TAP_DEVICE_H

#include "ITapeDevice.h"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>

class TapeDevice : public ITapeDevice {
private:
    std::fstream file;
    size_t length;
    size_t currentPos;
    std::unordered_map<std::string, int> delays;

public:
    TapeDevice(const std::string& filename, size_t length, const std::string& configFilename);

    TapeDevice(const std::string& filename, const std::string& configFilename);

    ~TapeDevice();

    int getCurrentCell() override;

    size_t getCurrentPosition() override;

    size_t getLength() override;

    void changeCurrentCell(int value) override;

    void moveToNextCell() override;

    void moveToPreviousCell() override;

    void readConfig(const std::string& configFilename);

    void simulateDelay(int delayMs);
};

#endif // TAP_DEVICE_H
