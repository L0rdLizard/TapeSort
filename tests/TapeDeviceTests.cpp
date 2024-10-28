#include "TapeDevice.h"
#include "gtest/gtest.h"
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

class TapeDeviceTest : public ::testing::Test {
protected:
    void SetUp() override {
        tapeFilename = "testTape";
        delays = {{"read_delay", 0}, {"write_delay", 0}, {"shift_delay", 0}, {"rewind_delay", 0}};
        length = 10;

        fs::remove("../data/" + tapeFilename + ".bin");
    }

    void TearDown() override {
        fs::remove("../data/" + tapeFilename + ".bin");
    }

    std::string tapeFilename;
    size_t length;
    std::unordered_map<std::string, int> delays;
};

TEST_F(TapeDeviceTest, ConstructorWithLength) {
    ASSERT_NO_THROW(TapeDevice device(tapeFilename, length, delays));
    ASSERT_TRUE(fs::exists("../data/" + tapeFilename + ".bin"));
}

TEST_F(TapeDeviceTest, GetCurrentPosition) {
    TapeDevice device(tapeFilename, length, delays);
    EXPECT_EQ(device.getCurrentPosition(), 0);
}

TEST_F(TapeDeviceTest, GetLength) {
    TapeDevice device(tapeFilename, length, delays);
    EXPECT_EQ(device.getLength(), length);
}

TEST_F(TapeDeviceTest, GetCurrentCell) {
    TapeDevice device(tapeFilename, length, delays);

    int initialValue = device.getCurrentCell_impl();
    EXPECT_EQ(initialValue, 0);
}

TEST_F(TapeDeviceTest, ChangeCurrentCell) {
    TapeDevice device(tapeFilename, length, delays);

    device.changeCurrentCell_impl(42);
    int newValue = device.getCurrentCell_impl();
    EXPECT_EQ(newValue, 42);
}

TEST_F(TapeDeviceTest, MoveToNextCell) {
    TapeDevice device(tapeFilename, length, delays);

    device.moveToNextCell_impl();
    EXPECT_EQ(device.getCurrentPosition(), 1);

    device.changeCurrentCell_impl(99);
    device.rewind_impl();
    device.moveToNextCell_impl();
    EXPECT_EQ(device.getCurrentCell_impl(), 99);
}

TEST_F(TapeDeviceTest, MoveToNextCellOutOfBounds) {
    TapeDevice device(tapeFilename, length, delays);

    for (size_t i = 0; i < length - 1; ++i) {
        device.moveToNextCell_impl();
    }

    EXPECT_THROW(device.moveToNextCell_impl(), std::out_of_range);
}

TEST_F(TapeDeviceTest, MoveToPreviousCell) {
    TapeDevice device(tapeFilename, length, delays);

    device.moveToNextCell_impl();
    device.moveToPreviousCell_impl();
    EXPECT_EQ(device.getCurrentPosition(), 0);
}

TEST_F(TapeDeviceTest, MoveToPreviousCellOutOfBounds) {
    TapeDevice device(tapeFilename, length, delays);

    EXPECT_THROW(device.moveToPreviousCell_impl(), std::out_of_range);
}

TEST_F(TapeDeviceTest, Rewind) {
    TapeDevice device(tapeFilename, length, delays);

    device.moveToNextCell_impl();
    device.moveToNextCell_impl();
    device.rewind_impl();
    EXPECT_EQ(device.getCurrentPosition(), 0);
}

TEST_F(TapeDeviceTest, DestructorConvertsToText) {
    {
        TapeDevice device(tapeFilename, length, delays);
        device.changeCurrentCell_impl(123);
    }
    std::string textFile = "../data_txt/" + tapeFilename + ".txt";
    EXPECT_TRUE(fs::exists(textFile));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
