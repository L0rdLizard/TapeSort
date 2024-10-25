#ifndef I_TAPE_DEVICE_H
#define I_TAPE_DEVICE_H

#include <cstddef>
#include <string>

class ITapeDevice {
public:
    virtual int getCurrentCell() = 0;
    virtual size_t getCurrentPosition() = 0;
    virtual size_t getLength() = 0;
    virtual void changeCurrentCell(int value) = 0;
    virtual void moveToNextCell() = 0;
    virtual void moveToPreviousCell() = 0;
    virtual void rewind() = 0;
    virtual ~ITapeDevice() = default;
    virtual void readConfig(const std::string& configFilename) = 0;
    virtual void simulateDelay(int delayMs) = 0;
};

#endif // ITAPEDEVICE_H