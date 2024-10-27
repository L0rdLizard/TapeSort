#include "Config.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

Config::Config(const std::string& configFilePath) : configFilePath(configFilePath) {}

std::unordered_map<std::string, int> Config::loadConfig() {
    std::string path = "../config/" + configFilePath;
    std::ifstream configFile(path);

    if (!configFile) {
        throw std::runtime_error("Failed to open configuration file: " + path);
    }

    std::unordered_map<std::string, int> delays;
    std::string line;

    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) continue;

        std::string key = line.substr(0, delimiterPos);
        key.erase(key.find_last_not_of(" \t") + 1);

        std::string valueStr = line.substr(delimiterPos + 1);
        valueStr.erase(0, valueStr.find_first_not_of(" \t"));

        try {
            int value = std::stoi(valueStr);
            delays[key] = value;
            // std::cout << key << ": " << value << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid value for key '" << key << "': " << valueStr << std::endl;
            continue;
        }
    }
    // std::cout << std::endl;

    if (delays.empty()) {
        throw std::runtime_error("No valid configuration entries found in file: " + path);
    }

    return delays;
}
