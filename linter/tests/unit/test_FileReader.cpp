#include <gtest/gtest.h>
#include "utils/FileReader.h"
#include <fstream>

using namespace linter;

class FileReaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream file("temp_test_file.cpp");
        file << "line1\n";
        file << "line2\n";
        file << "line3";
        file.close();
    }
    
    void TearDown() override {
        std::remove("temp_test_file.cpp");
    }
};

// Тест 1: Существующий файл
TEST_F(FileReaderTest, FileExists) {
    EXPECT_TRUE(FileReader::fileExists("temp_test_file.cpp"));
}

// Тест 2: Несуществующий файл
TEST_F(FileReaderTest, FileDoesNotExist) {
    EXPECT_FALSE(FileReader::fileExists("non_existent.cpp"));
}

// Тест 3: Чтение существующего файла
TEST_F(FileReaderTest, ReadExistingFile) {
    auto lines = FileReader::readFile("temp_test_file.cpp");
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "line2");
    EXPECT_EQ(lines[2], "line3");
}

// Тест 4: Чтение несуществующего файла
TEST_F(FileReaderTest, ReadNonExistentFile) {
    auto lines = FileReader::readFile("non_existent.cpp");
    EXPECT_TRUE(lines.empty());
}

// Тест 5: Пустой файл
TEST_F(FileReaderTest, ReadEmptyFile) {
    std::ofstream file("empty.txt");
    file.close();
    auto lines = FileReader::readFile("empty.txt");
    EXPECT_TRUE(lines.empty());
    std::remove("empty.txt");
}