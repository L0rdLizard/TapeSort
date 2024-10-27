#ifndef I_TAPE_DEVICE_H
#define I_TAPE_DEVICE_H

#include <cstddef>
#include <string>

#include "Task.h"

class ITapeDevice {
public:
    virtual size_t getCurrentPosition() = 0;
    virtual size_t getLength() = 0;
    virtual ~ITapeDevice() = default;
    virtual void readConfig(const std::string& configFilename) = 0;
    virtual void simulateDelay(int delayMs) = 0;

    virtual Task<int> getCurrentCell() = 0;
    virtual Task<void> changeCurrentCell(int value) = 0;
    virtual Task<void> moveToNextCell() = 0;
    virtual Task<void> moveToPreviousCell() = 0;
    virtual Task<void> rewind() = 0;

private:
    virtual int getCurrentCell_impl() = 0;
    virtual void changeCurrentCell_impl(int value) = 0;
    virtual void moveToNextCell_impl() = 0;
    virtual void moveToPreviousCell_impl() = 0;
    virtual void rewind_impl() = 0;

};

#endif // ITAPEDEVICE_H