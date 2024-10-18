#ifndef ITAPEDEVICE_H
#define ITAPEDEVICE_H

class ITapeDevice {
public:
    virtual int getCurrentCell() = 0;
    virtual void changeCurrentCell(int value) = 0;
    virtual void moveToNextCell() = 0;
    virtual void moveToPreviousCell() = 0;
    virtual ~ITapeDevice() = default;
};

#endif ITAPEDEVICE_H