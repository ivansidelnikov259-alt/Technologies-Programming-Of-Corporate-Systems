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
};

// Тест 1: Сканирование директории
TEST_F(FileIteratorTest, ScanDirectory) {
    FileIterator it;
    it.scanDirectory("test_dir");
    EXPECT_GE(it.size(), 3); 
}

// Тест 2: Итератор hasNext/next
TEST_F(FileIteratorTest, IteratorHasNextAndNext) {
    FileIterator it;
    it.scanDirectory("test_dir");
    
    int count = 0;
    while (it.hasNext()) {
        std::string file = it.next();
        EXPECT_FALSE(file.empty());
        count++;
    }
    EXPECT_EQ(count, it.size());
}

// Тест 3: Сброс итератора
TEST_F(FileIteratorTest, IteratorReset) {
    FileIterator it;
    it.scanDirectory("test_dir");
    
    while (it.hasNext()) {
        it.next();
    }
    EXPECT_FALSE(it.hasNext());
    
    it.reset();
    EXPECT_TRUE(it.hasNext());
}

// Тест 4: Сканирование одного файла
TEST_F(FileIteratorTest, ScanSingleFile) {
    FileIterator it;
    it.scanDirectory("test_dir/file1.cpp");
    EXPECT_EQ(it.size(), 1);
    EXPECT_TRUE(it.getAllFiles()[0].find("file1.cpp") != std::string::npos);
}

// Тест 5: Невалидный путь
TEST_F(FileIteratorTest, InvalidPath) {
    FileIterator it;
    it.scanDirectory("non_existent_path");
    EXPECT_EQ(it.size(), 0);
}