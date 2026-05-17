#include <gtest/gtest.h>
#include "core/LinterEngine.h"
#include "rules/IndentRule.h"
#include "rules/VarNameRule.h"
#include <memory>

using namespace linter;

class LinterEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine.clearRules();
    }
    
    LinterEngine engine;
};

// Тест 1: Добавление правила
TEST_F(LinterEngineTest, AddRule) {
    EXPECT_EQ(engine.getRules().size(), 0);
    engine.addRule(std::make_unique<IndentRule>());
    EXPECT_EQ(engine.getRules().size(), 1);
}

// Тест 2: Удаление правила
TEST_F(LinterEngineTest, RemoveRule) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    EXPECT_EQ(engine.getRules().size(), 2);
    
    engine.removeRule("IndentRule");
    EXPECT_EQ(engine.getRules().size(), 1);
    EXPECT_EQ(engine.getRules()[0]->getName(), "VarNameRule");
}

// Тест 3: Очистка всех правил
TEST_F(LinterEngineTest, ClearAllRules) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.clearRules();
    EXPECT_EQ(engine.getRules().size(), 0);
}

// Тест 4: Анализ несуществующего файла
TEST_F(LinterEngineTest, AnalyzeNonExistentFile) {
    engine.addRule(std::make_unique<IndentRule>());
    auto errors = engine.analyzeFile("non_existent_file.cpp");
    ASSERT_GE(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "IOError");
}

// Тест 5: Анализ без правил
TEST_F(LinterEngineTest, AnalyzeWithNoRules) {
    auto errors = engine.analyzeFile(__FILE__);
    EXPECT_TRUE(errors.empty());
}

// Тест 6: Анализ нескольких файлов
TEST_F(LinterEngineTest, AnalyzeMultipleFiles) {
    engine.addRule(std::make_unique<IndentRule>());
    std::vector<std::string> files = {__FILE__, __FILE__};
    auto errors = engine.analyzeFiles(files);
    EXPECT_GE(errors.size(), 0);
}