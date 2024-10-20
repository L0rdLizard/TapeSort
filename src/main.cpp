#include "TapeDevice.cpp"
#include <iostream>

void test1() {
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
}

void test2() {
    try {
        TapeDevice tape("tape.bin");

        // tape.moveToNextCell();

        int value = tape.getCurrentCell();
        std::cout << "Value at current cell: " << value << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    test2();

    return 0;
}