cmake_minimum_required(VERSION 3.12)

project(TapeSort)

set(CMAKE_CXX_STANDARD 17)

add_executable(TapeSort
    src/main.cpp
    # src/TapeDevice.cpp
    # src/TapeSorter.cpp
    # src/Config.cpp
    # src/utils/FileUtils.cpp
)

# enable_testing()
# add_executable(TapeDeviceTests tests/TapeDeviceTests.cpp)
# add_executable(TapeSorterTests tests/TapeSorterTests.cpp)