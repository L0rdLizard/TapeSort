#include "TapeDevice.cpp"
#include <iostream>

int main() {
    try {
        TapeDevice tape("tape.bin", 10);

        tape.changeCurrentCell(42);
        tape.moveToNextCell();
        tape.changeCurrentCell(100);

        tape.moveToPreviousCell();
        int value = tape.getCurrentCell();
        std::cout << "Value at current cell: " << value << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}