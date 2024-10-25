// FileUtils.h
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

class FileUtils {
public:
    static size_t convertTextToBinary(const std::string& filename);
    static void convertBinaryToText(const std::string& filename);
};

#endif // FILE_UTILS_H
