#include "TapeDevice.h"
#include <iostream>

void test1() {
    try {
        TapeDevice tape("tape.bin", 10, "delays.cfg");

        tape.changeCurrentCell(42);
        tape.moveToNextCell();
        tape.changeCurrentCell(100);

        tape.moveToPreviousCell();
        int value = tape.getCurrentCell();
        std::cout << "Value at current cell: " << value << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test2() {
    try {
        TapeDevice tape("tape.bin", "delays.cfg");

        // tape.moveToNextCell();

        int value = tape.getCurrentCell();
        std::cout << "Value at current cell: " << value << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    test1();

    return 0;
}