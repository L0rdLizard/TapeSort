cmake_minimum_required(VERSION 3.12)

project(TapeSort)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(TapeSort
    src/main.cpp
    src/TapeDevice.cpp
    src/TempTapeDevice.cpp
    src/TapeSorter.cpp
    src/Config.cpp
    src/utils/FileUtils.cpp
    src/utils/TimeManager.cpp
)

enable_testing()
add_executable(TapeDeviceTests
    tests/TapeDeviceTests.cpp
    src/TapeDevice.cpp
    src/utils/FileUtils.cpp
    src/Config.cpp
)
target_link_libraries(
    TapeDeviceTests
    GTest::gtest_main
)

add_executable(TapeSorterTests
    tests/TapeSorterTests.cpp
    src/TapeSorter.cpp
    src/TapeDevice.cpp
    src/utils/FileUtils.cpp
    src/Config.cpp
    src/utils/TimeManager.cpp
    src/TempTapeDevice.cpp
)
target_link_libraries(
    TapeSorterTests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TapeDeviceTests)
gtest_discover_tests(TapeSorterTests)

# add_executable(TapeSorterTests tests/TapeSorterTests.cpp)