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
    src/TapeSorter.cpp
    src/Config.cpp
    # src/utils/FileUtils.cpp
)

enable_testing()
add_executable(TapeDeviceTests tests/TapeDeviceTests.cpp)
target_link_libraries(
    TapeDeviceTests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TapeDeviceTests)
# add_executable(TapeSorterTests tests/TapeSorterTests.cpp)