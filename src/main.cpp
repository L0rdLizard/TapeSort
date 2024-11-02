#include "TapeDevice.h"
#include "TapeSorter.h"
#include "utils/TimeManager.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include "Config.h"

int main(int argc, char* argv[])
{   
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl << "Write filenames without extension!" << std::endl;
        return 1;
    }
    std::string path = "tmp/";
    try {
        if (std::filesystem::create_directory(path)) {
            std::cout << "The'tmp' directory has been successfully created" << std::endl;
        } else {
            
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::string filename1 = argv[1];
    std::string filename2 = argv[2];

    try
    {
        TimeManager timeManager(false);

        std::string configFilename = "delays.cfg";
        Config config(configFilename);
        std::unordered_map<std::string, int> delays = config.loadConfig();

        TapeDevice tape1(filename1, delays);

        TapeDevice tape2(filename2, tape1.getLength(), delays);

        size_t memorySize = 64;

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

    return 0;
}