#include <gtest/gtest.h>
#include "utils/FileIterator.h"
#include <fstream>
#include <filesystem>

using namespace linter;

class FileIteratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::create_directory("test_dir");
        std::ofstream file1("test_dir/file1.cpp");
        file1.close();
        std::ofstream file2("test_dir/file2.h");
        file2.close();
        std::ofstream file3("test_dir/file3.txt");
        file3.close();
        std::filesystem::create_directory("test_dir/subdir");
        std::ofstream file4("test_dir/subdir/file4.hpp");
        file4.close();
    }
    
    void TearDown() override {
        std::filesystem::remove_all("test_dir");
    }
    
    FileIterator iterator;
};

// scanDirectory() - 5 тестов

TEST_F(FileIteratorTest, ScanDirectory_ValidDirectory) {
    iterator.scanDirectory("test_dir");
    EXPECT_GE(iterator.size(), 3);
}

TEST_F(FileIteratorTest, ScanDirectory_EmptyDirectory) {
    std::filesystem::create_directory("empty_dir");
    iterator.scanDirectory("empty_dir");
    EXPECT_EQ(iterator.size(), 0);
    std::filesystem::remove("empty_dir");
}

TEST_F(FileIteratorTest, ScanDirectory_SingleFile) {
    iterator.scanDirectory("test_dir/file1.cpp");
    EXPECT_EQ(iterator.size(), 1);
}

TEST_F(FileIteratorTest, ScanDirectory_InvalidPath) {
    iterator.scanDirectory("non_existent_path");
    EXPECT_EQ(iterator.size(), 0);
}

TEST_F(FileIteratorTest, ScanDirectory_NestedDirectories) {
    iterator.scanDirectory("test_dir");
    int cppCount = 0;
    for (const auto& f : iterator.getAllFiles()) {
        if (f.find(".cpp") != std::string::npos) cppCount++;
        if (f.find(".hpp") != std::string::npos) cppCount++;
    }
    EXPECT_GE(cppCount, 2);
}

// hasNext() - 5 тестов

TEST_F(FileIteratorTest, HasNext_EmptyIterator) {
    iterator.scanDirectory("empty_dir");
    std::filesystem::create_directory("empty_dir");
    EXPECT_FALSE(iterator.hasNext());
    std::filesystem::remove("empty_dir");
}

TEST_F(FileIteratorTest, HasNext_NonEmptyIterator) {
    iterator.scanDirectory("test_dir/file1.cpp");
    EXPECT_TRUE(iterator.hasNext());
}

TEST_F(FileIteratorTest, HasNext_AfterConsumingAll) {
    iterator.scanDirectory("test_dir");
    while (iterator.hasNext()) {
        iterator.next();
    }
    EXPECT_FALSE(iterator.hasNext());
}

TEST_F(FileIteratorTest, HasNext_AfterReset) {
    iterator.scanDirectory("test_dir");
    while (iterator.hasNext()) {
        iterator.next();
    }
    EXPECT_FALSE(iterator.hasNext());
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
}

TEST_F(FileIteratorTest, HasNext_MultipleCalls) {
    iterator.scanDirectory("test_dir/file1.cpp");
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_TRUE(iterator.hasNext());
    EXPECT_TRUE(iterator.hasNext());
}

// next() - 5 тестов

TEST_F(FileIteratorTest, Next_ReturnsFileName) {
    iterator.scanDirectory("test_dir/file1.cpp");
    std::string file = iterator.next();
    EXPECT_FALSE(file.empty());
    EXPECT_TRUE(file.find("file1.cpp") != std::string::npos);
}

TEST_F(FileIteratorTest, Next_ReturnsDifferentFiles) {
    iterator.scanDirectory("test_dir");
    std::string first = iterator.next();
    std::string second = iterator.next();
    EXPECT_NE(first, second);
}

TEST_F(FileIteratorTest, Next_EmptyWhenNoMoreFiles) {
    iterator.scanDirectory("test_dir/file1.cpp");
    iterator.next();
    std::string empty = iterator.next();
    EXPECT_TRUE(empty.empty());
}

TEST_F(FileIteratorTest, Next_OrderConsistent) {
    iterator.scanDirectory("test_dir");
    std::string first1 = iterator.next();
    iterator.reset();
    std::string first2 = iterator.next();
    EXPECT_EQ(first1, first2);
}

TEST_F(FileIteratorTest, Next_AfterResetReturnsFirstAgain) {
    iterator.scanDirectory("test_dir");
    iterator.next();
    iterator.next();
    iterator.reset();
    std::string first = iterator.next();
    EXPECT_FALSE(first.empty());
}

// reset() - 5 тестов

TEST_F(FileIteratorTest, Reset_PositionToStart) {
    iterator.scanDirectory("test_dir");
    iterator.next();
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
}

TEST_F(FileIteratorTest, Reset_MultipleTimes) {
    iterator.scanDirectory("test_dir");
    iterator.reset();
    iterator.reset();
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
}

TEST_F(FileIteratorTest, Reset_EmptyIterator) {
    iterator.scanDirectory("empty_dir");
    std::filesystem::create_directory("empty_dir");
    iterator.reset();
    EXPECT_FALSE(iterator.hasNext());
    std::filesystem::remove("empty_dir");
}

TEST_F(FileIteratorTest, Reset_AfterFullConsumption) {
    iterator.scanDirectory("test_dir/file1.cpp");
    iterator.next();
    EXPECT_FALSE(iterator.hasNext());
    iterator.reset();
    EXPECT_TRUE(iterator.hasNext());
}

TEST_F(FileIteratorTest, Reset_DoesNotChangeFileList) {
    iterator.scanDirectory("test_dir");
    size_t sizeBefore = iterator.size();
    iterator.reset();
    size_t sizeAfter = iterator.size();
    EXPECT_EQ(sizeBefore, sizeAfter);
}

// size() - 5 тестов

TEST_F(FileIteratorTest, Size_ReturnsCorrectCount) {
    iterator.scanDirectory("test_dir");
    EXPECT_EQ(iterator.size(), 3);
}

TEST_F(FileIteratorTest, Size_AfterReset_Same) {
    iterator.scanDirectory("test_dir");
    size_t sizeBefore = iterator.size();
    iterator.reset();
    EXPECT_EQ(sizeBefore, iterator.size());
}

TEST_F(FileIteratorTest, Size_AfterNext_Same) {
    iterator.scanDirectory("test_dir");
    iterator.next();
    EXPECT_EQ(iterator.size(), 3);
}

TEST_F(FileIteratorTest, Size_EmptyDirectory) {
    std::filesystem::create_directory("empty_dir2");
    iterator.scanDirectory("empty_dir2");
    EXPECT_EQ(iterator.size(), 0);
    std::filesystem::remove("empty_dir2");
}

TEST_F(FileIteratorTest, Size_SingleFile) {
    iterator.scanDirectory("test_dir/file1.cpp");
    EXPECT_EQ(iterator.size(), 1);
}

// getAllFiles() - 5 тестов

TEST_F(FileIteratorTest, GetAllFiles_ReturnsVector) {
    iterator.scanDirectory("test_dir");
    auto files = iterator.getAllFiles();
    EXPECT_EQ(files.size(), 3);
}

TEST_F(FileIteratorTest, GetAllFiles_EmptyDirectory) {
    std::filesystem::create_directory("empty_dir3");
    iterator.scanDirectory("empty_dir3");
    auto files = iterator.getAllFiles();
    EXPECT_TRUE(files.empty());
    std::filesystem::remove("empty_dir3");
}

TEST_F(FileIteratorTest, GetAllFiles_ReadOnly) {
    iterator.scanDirectory("test_dir");
    auto files = iterator.getAllFiles();
    files.clear();
    EXPECT_EQ(iterator.size(), 3);
}

TEST_F(FileIteratorTest, GetAllFiles_AfterReset_Same) {
    iterator.scanDirectory("test_dir");
    auto before = iterator.getAllFiles();
    iterator.reset();
    auto after = iterator.getAllFiles();
    EXPECT_EQ(before.size(), after.size());
}

TEST_F(FileIteratorTest, GetAllFiles_ValidExtensions) {
    iterator.scanDirectory("test_dir");
    auto files = iterator.getAllFiles();
    for (const auto& f : files) {
        bool hasValidExt = (f.find(".cpp") != std::string::npos) ||
                           (f.find(".h") != std::string::npos) ||
                           (f.find(".hpp") != std::string::npos);
        EXPECT_TRUE(hasValidExt);
    }
}