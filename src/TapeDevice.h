#ifndef TAPE_DEVICE_H
#define TAPE_DEVICE_H

#include "ITapeDevice.h"
#include "Task.h"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <memory>

class TapeDevice : public ITapeDevice {
private:
    std::fstream file;
    size_t length;
    size_t currentPos;
    std::unordered_map<std::string, int> delays;
    std::string tapeFilename;

    int getCurrentCell_impl();
    void changeCurrentCell_impl(int value);
    void moveToNextCell_impl();
    void moveToPreviousCell_impl();
    void rewind_impl();
public:
    TapeDevice(const std::string& filename, size_t length, const std::string& configFilename);

    TapeDevice(const std::string& filename, const std::string& configFilename);

    ~TapeDevice();

    size_t getCurrentPosition() override;

    size_t getLength() override;

    void readConfig(const std::string& configFilename) override;

    void simulateDelay(int delayMs) override;

    Task<int> getCurrentCell() override {
        return Task<int>{ std::async(std::launch::async, [this]() { return getCurrentCell_impl(); }), delays["read_delay"] };
    }

    Task<void> changeCurrentCell(int value) override {
        return Task<void>{ std::async(std::launch::async, [this, value]() { changeCurrentCell_impl(value); }), delays["write_delay"] };
    }

    Task<void> moveToNextCell() override {
        return Task<void>{ std::async(std::launch::async, [this]() { moveToNextCell_impl(); }), delays["shift_delay"] };
    }

    Task<void> moveToPreviousCell() override {
        return Task<void>{ std::async(std::launch::async, [this]() { moveToPreviousCell_impl(); }), delays["shift_delay"] };
    }

    Task<void> rewind() override {
        if (currentPos == 0) {
            return Task<void>{ std::future<void>(), 0 };
        }
        return Task<void>{ std::async(std::launch::async, [this]() { rewind_impl(); }), delays["rewind_delay"] };
    }
};

#endif // TAPE_DEVICE_H
