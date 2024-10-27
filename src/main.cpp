#include "TapeDevice.h"
#include "TapeSorter.h"
#include "utils/TimeManager.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include "Config.h"

// void test1()
// {
//     try
//     {
//         TimeManager timeManager(false);
//         TapeDevice tape1("tape3", "delays.cfg");

//         TapeDevice tape2("tape4", tape1.getLength(), "delays.cfg");

//         size_t memorySize = 2;
//         TapeSorter tapeSorter(tape1, tape2, memorySize, timeManager);
        
//         auto start = std::chrono::high_resolution_clock::now();
//         tapeSorter.sort();
//         auto end = std::chrono::high_resolution_clock::now();

//         std::chrono::duration<double> duration = end - start;
//         std::cout << "Sorting took " << duration.count() << " seconds." << std::endl;

//         tape2.rewind();
//         // std::cout << tape2.getCurrentCell() << std::endl;
//         // for (int i = 1; i < tape2.getLength(); i++)
//         // {
//         //     tape2.moveToNextCell();
//         //     std::cout << tape2.getCurrentCell() << std::endl;
//         // }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
// }

void testTime()
{
    try
    {
        TimeManager timeManager(false);
        
        std::string configFilename = "../config/delays.cfg";
        Config config(configFilename);
        std::unordered_map<std::string, int> delays = config.loadConfig();

        TapeDevice tape1("tape3", delays);

        TapeDevice tape2("tape4", tape1.getLength(), delays);

        size_t memorySize = 2;

        TapeSorter tapeSorter(tape1, tape2, memorySize, timeManager, delays);

        auto start = std::chrono::high_resolution_clock::now();
        tapeSorter.sort();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        std::cout << "Real time: " << duration.count() << " seconds." << std::endl;

        std::cout << "Model time with delays: " << timeManager.get_global_execution_time() << " ms" << std::endl;
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
    testTime();

    return 0;
}