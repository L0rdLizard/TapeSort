#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

class Config {
public:
    Config(const std::string& configFilePath);

    std::unordered_map<std::string, int> loadConfig();

private:
    std::string configFilePath;

};

#endif // CONFIG_H