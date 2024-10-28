#ifndef TEMP_TAPE_DEVICE_H
#define TEMP_TAPE_DEVICE_H

#include "ITapeDevice.h"
#include "Task.h"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <memory>

class TempTapeDevice : public ITapeDevice {
private:
    std::fstream file;
    size_t length;
    size_t currentPos;
    std::unordered_map<std::string, int> delays;
    std::string tempFilename;

    int getCurrentCell_impl();
    void changeCurrentCell_impl(int value);
    void moveToNextCell_impl();
    void moveToPreviousCell_impl();
    void rewind_impl();
public:
    TempTapeDevice(const std::string& filename, size_t length, std::unordered_map<std::string, int> delays);

    TempTapeDevice(const std::string& filename, std::unordered_map<std::string, int> delays);

    ~TempTapeDevice();

    size_t getCurrentPosition() override;

    size_t getLength() override;

    void readConfig(const std::string& configFilename) override;

    void simulateDelay(int delayMs) override;

    Task<int> getCurrentCell() override;

    Task<void> changeCurrentCell(int value) override;

    Task<void> moveToNextCell() override;

    Task<void> moveToPreviousCell() override;

    Task<void> rewind() override;
};

#endif // TEMP_TAPE_DEVICE_H
