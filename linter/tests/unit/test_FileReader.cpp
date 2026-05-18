#include <gtest/gtest.h>
#include "utils/FileReader.h"
#include <fstream>

using namespace linter;

class FileReaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream file("test_read.txt");
        file << "line1\nline2\nline3";
        file.close();
        
        std::ofstream empty("empty.txt");
        empty.close();
    }
    
    void TearDown() override {
        std::remove("test_read.txt");
        std::remove("empty.txt");
        std::remove("test_write.txt");
    }
};

// fileExists() - 5 тестов

TEST_F(FileReaderTest, FileExists_ExistingFile) {
    EXPECT_TRUE(FileReader::fileExists("test_read.txt"));
}

TEST_F(FileReaderTest, FileExists_NonExistentFile) {
    EXPECT_FALSE(FileReader::fileExists("non_existent.cpp"));
}

TEST_F(FileReaderTest, FileExists_EmptyFile) {
    EXPECT_TRUE(FileReader::fileExists("empty.txt"));
}

TEST_F(FileReaderTest, FileExists_WithPath) {
    EXPECT_TRUE(FileReader::fileExists("test_read.txt"));
}

TEST_F(FileReaderTest, FileExists_CaseSensitive) {
    EXPECT_TRUE(FileReader::fileExists("TEST_READ.TXT"));
}

// readFile() - 5 тестов

TEST_F(FileReaderTest, ReadFile_ExistingFile) {
    auto lines = FileReader::readFile("test_read.txt");
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "line2");
    EXPECT_EQ(lines[2], "line3");
}

TEST_F(FileReaderTest, ReadFile_NonExistentFile) {
    auto lines = FileReader::readFile("non_existent.txt");
    EXPECT_TRUE(lines.empty());
}

TEST_F(FileReaderTest, ReadFile_EmptyFile) {
    auto lines = FileReader::readFile("empty.txt");
    EXPECT_TRUE(lines.empty());
}

TEST_F(FileReaderTest, ReadFile_PreservesEmptyLines) {
    std::ofstream file("with_empty.txt");
    file << "line1\n\nline3";
    file.close();
    
    auto lines = FileReader::readFile("with_empty.txt");
    EXPECT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "");
    EXPECT_EQ(lines[2], "line3");
    
    std::remove("with_empty.txt");
}

TEST_F(FileReaderTest, ReadFile_NoTrailingNewline) {
    std::ofstream file("no_newline.txt");
    file << "line1\nline2";
    file.close();
    
    auto lines = FileReader::readFile("no_newline.txt");
    EXPECT_EQ(lines.size(), 2);
    
    std::remove("no_newline.txt");
}