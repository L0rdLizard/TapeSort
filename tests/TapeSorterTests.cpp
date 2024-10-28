#include <gtest/gtest.h>
#include "TapeSorter.h"
#include "TapeDevice.h"
#include "utils/TimeManager.h"
#include "Config.h"
#include "TempTapeDevice.h"
#include <memory>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class TapeSorterTest : public ::testing::Test {
protected:
    void SetUp() override {
        tapeFilenameInput = "testSortInput";
        tapeFilenameOutput = "testSortOutput";
        delays = {{"read_delay", 0}, {"write_delay", 0}, {"shift_delay", 0}, {"rewind_delay", 0}};
        length = 10;

        fs::remove("data/" + tapeFilenameInput + ".bin");
        fs::remove("data/" + tapeFilenameOutput + ".bin");
    }

    void TearDown() override {
        fs::remove("data/" + tapeFilenameInput + ".bin");
        fs::remove("data/" + tapeFilenameOutput + ".bin");
        fs::remove("data_txt/" + tapeFilenameOutput + ".txt");
    }

    std::string tapeFilenameInput;
    std::string tapeFilenameOutput;
    size_t length;
    std::unordered_map<std::string, int> delays;
};

TEST_F(TapeSorterTest, TestLengthAfterSort) {
    TimeManager timeManager(false);

    TapeDevice tape1(tapeFilenameInput, delays);

    TapeDevice tape2(tapeFilenameOutput, tape1.getLength(), delays);

    size_t memorySize = 5;

    TapeSorter tapeSorter(tape1, tape2, memorySize, timeManager, delays);

    tapeSorter.sort();

    EXPECT_EQ(tape1.getLength(), tape2.getLength());
}

TEST_F(TapeSorterTest, TestSorting) {
    TimeManager timeManager(false);

    TapeDevice tape1(tapeFilenameInput, delays);

    TapeDevice tape2(tapeFilenameOutput, tape1.getLength(), delays);

    size_t memorySize = 5;

    TapeSorter tapeSorter(tape1, tape2, memorySize, timeManager, delays);

    tapeSorter.sort();

    tape2.rewind();
    int value = tape2.getCurrentCell_impl();
    EXPECT_EQ(value, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
