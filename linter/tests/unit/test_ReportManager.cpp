#include <gtest/gtest.h>
#include "core/ReportManager.h"
#include <fstream>

using namespace linter;

class ReportManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        ReportManager::getInstance().clearErrors();
    }
    
    Error createTestError() {
        return Error("test.cpp", 10, 5, "TestError", "Test message");
    }
};

// Тест 1: Одиночка (один экземпляр)
TEST_F(ReportManagerTest, SingletonInstance) {
    ReportManager& instance1 = ReportManager::getInstance();
    ReportManager& instance2 = ReportManager::getInstance();
    EXPECT_EQ(&instance1, &instance2);
}

// Тест 2: Добавление одной ошибки
TEST_F(ReportManagerTest, AddSingleError) {
    auto& rm = ReportManager::getInstance();
    rm.addError(createTestError());
    EXPECT_EQ(rm.getErrorCount(), 1);
}

// Тест 3: Добавление нескольких ошибок
TEST_F(ReportManagerTest, AddMultipleErrors) {
    auto& rm = ReportManager::getInstance();
    std::vector<Error> errors = {createTestError(), createTestError()};
    rm.addErrors(errors);
    EXPECT_EQ(rm.getErrorCount(), 2);
}

// Тест 4: Очистка ошибок
TEST_F(ReportManagerTest, ClearErrors) {
    auto& rm = ReportManager::getInstance();
    rm.addError(createTestError());
    rm.clearErrors();
    EXPECT_EQ(rm.getErrorCount(), 0);
}

// Тест 5: Подсчёт по типам
TEST_F(ReportManagerTest, CountByType) {
    auto& rm = ReportManager::getInstance();
    Error err1("a.cpp", 1, 1, "TypeA", "msg");
    Error err2("a.cpp", 2, 1, "TypeA", "msg");
    Error err3("a.cpp", 3, 1, "TypeB", "msg");
    rm.addErrors({err1, err2, err3});
    
    EXPECT_EQ(rm.getErrorCountByType("TypeA"), 2);
    EXPECT_EQ(rm.getErrorCountByType("TypeB"), 1);
    EXPECT_EQ(rm.getErrorCountByType("TypeC"), 0);
}

// Тест 6: Генерация отчёта
TEST_F(ReportManagerTest, GenerateReport) {
    auto& rm = ReportManager::getInstance();
    rm.addError(createTestError());
    
    std::string report = rm.generateReport();
    EXPECT_TRUE(report.find("TestError") != std::string::npos);
    EXPECT_TRUE(report.find("test.cpp") != std::string::npos);
}