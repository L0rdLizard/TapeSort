// FileUtils.cpp
#include "FileUtils.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>

size_t FileUtils::convertTextToBinary(const std::string& filename) {
    std::ifstream textFile("../data_txt/" + filename + ".txt");
    if (!textFile.is_open()) {
        return 0;
    }

    std::string binFilePath = "../data/" + filename + ".bin";
    std::ofstream binFile;

    binFile.open(binFilePath, std::ios::binary | std::ios::trunc);
    
    if (!textFile.is_open() || !binFile.is_open()) {
        throw std::runtime_error("Failed to open files");
    }
    size_t count = 0;
    std::string line;

    while (std::getline(textFile, line)) {
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        if (line.empty()) {
            continue;
        }
        int value;
        std::istringstream(line) >> value;

        if (textFile.fail() && !textFile.eof()) {
            std::cerr << "Warning: Invalid number in line: \"" << line << "\"" << std::endl;
            continue;
        }

        binFile.write(reinterpret_cast<const char*>(&value), sizeof(int));
        count++;
    }

    textFile.close();
    binFile.close();

    return count;
}

void FileUtils::convertBinaryToText(const std::string& filename) {
    std::string binaryFilePath = "../data/" + filename + ".bin";
    std::string textFilePath = "../data_txt/" + filename + ".txt";

    std::ifstream inFile(binaryFilePath, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open binary file: " + binaryFilePath);
    }

    std::vector<int> data;
    int value;

    while (inFile.read(reinterpret_cast<char*>(&value), sizeof(int))) {
        data.push_back(value);
    }
    inFile.close();

    std::ofstream outFile(textFilePath);
    if (!outFile) {
        throw std::runtime_error("Unable to open text file for writing: " + textFilePath);
    }

    for (const int& val : data) {
        outFile << val << "\n";
    }
    outFile.close();
}
