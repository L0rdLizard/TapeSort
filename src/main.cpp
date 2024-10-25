#include "TapeDevice.h"
#include "TapeSorter.h"
#include <iostream>
#include <filesystem>

void test1()
{
    try
    {
        TapeDevice tape("tape", "delays.cfg");

        tape.changeCurrentCell(42);
        tape.moveToNextCell();
        tape.changeCurrentCell(100);

        tape.moveToPreviousCell();
        int value = tape.getCurrentCell();
        std::cout << "Value at current cell: " << value << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test2()
{
    try
    {
        TapeDevice tape("tape", "delays.cfg");

        // tape.moveToNextCell();

        int value = tape.getCurrentCell();
        std::cout << "Value at current cell: " << value << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test3()
{
    try
    {
        TapeDevice tape1("tape1", "delays.cfg");

        tape1.changeCurrentCell(42);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(100);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(1);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(6);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(3);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(4);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(2);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(-2);
        tape1.moveToNextCell();
        tape1.changeCurrentCell(-3);

        TapeDevice tape2("tape2", "delays.cfg");

        size_t memorySize = 15;
        TapeSorter tapeSorter(tape1, tape2, memorySize);
        tapeSorter.sort();

        tape2.rewind();
        std::cout << tape2.getCurrentCell() << std::endl;
        for (int i = 1; i < tape2.getLength(); i++)
        {
            tape2.moveToNextCell();
            std::cout << tape2.getCurrentCell() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test4()
{
    try
    {
        TapeDevice tape1("tape3", "delays.cfg");

        TapeDevice tape2("tape4", tape1.getLength(), "delays.cfg");

        size_t memorySize = 15;
        TapeSorter tapeSorter(tape1, tape2, memorySize);
        tapeSorter.sort();

        tape2.rewind();
        std::cout << tape2.getCurrentCell() << std::endl;
        for (int i = 1; i < tape2.getLength(); i++)
        {
            tape2.moveToNextCell();
            std::cout << tape2.getCurrentCell() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main()
{   
    std::string path = "../tmp/";
    try {
        if (std::filesystem::create_directory(path)) {
            std::cout << "Папка 'tmp' успешно создана." << std::endl;
        } else {
            
        }
    } catch (const std::filesystem::filesystem_error& e) {
        
    }
    test4();

    return 0;
}