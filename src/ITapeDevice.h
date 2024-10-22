#ifndef I_TAPE_DEVICE_H
#define I_TAPE_DEVICE_H

class ITapeDevice {
public:
    virtual int getCurrentCell() = 0;
    virtual size_t getCurrentPosition() = 0;
    virtual size_t getLength() = 0;
    virtual void changeCurrentCell(int value) = 0;
    virtual void moveToNextCell() = 0;
    virtual void moveToPreviousCell() = 0;
    virtual ~ITapeDevice() = default;
};

#endif // ITAPEDEVICE_H