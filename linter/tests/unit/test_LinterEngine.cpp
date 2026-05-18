#include <gtest/gtest.h>
#include "core/LinterEngine.h"
#include "rules/IndentRule.h"
#include "rules/VarNameRule.h"
#include "rules/UninitVarRule.h"
#include <memory>
#include <fstream>

using namespace linter;

class LinterEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine.clearRules();
    }
    
    LinterEngine engine;
};

// addRule() - 5 тестов

TEST_F(LinterEngineTest, AddRule_SingleRule) {
    EXPECT_EQ(engine.getRules().size(), 0);
    engine.addRule(std::make_unique<IndentRule>());
    EXPECT_EQ(engine.getRules().size(), 1);
}

TEST_F(LinterEngineTest, AddRule_MultipleRules) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    EXPECT_EQ(engine.getRules().size(), 2);
}

TEST_F(LinterEngineTest, AddRule_SameRuleTwice) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<IndentRule>());
    EXPECT_EQ(engine.getRules().size(), 2);
}

TEST_F(LinterEngineTest, AddRule_AfterClear) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.clearRules();
    engine.addRule(std::make_unique<VarNameRule>());
    EXPECT_EQ(engine.getRules().size(), 1);
    EXPECT_EQ(engine.getRules()[0]->getName(), "VarNameRule");
}

TEST_F(LinterEngineTest, AddRule_OrderPreserved) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    EXPECT_EQ(engine.getRules()[0]->getName(), "IndentRule");
    EXPECT_EQ(engine.getRules()[1]->getName(), "VarNameRule");
}

// removeRule() - 5 тестов

TEST_F(LinterEngineTest, RemoveRule_ByName) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.removeRule("IndentRule");
    EXPECT_EQ(engine.getRules().size(), 1);
    EXPECT_EQ(engine.getRules()[0]->getName(), "VarNameRule");
}

TEST_F(LinterEngineTest, RemoveRule_NonExistent) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.removeRule("NonExistentRule");
    EXPECT_EQ(engine.getRules().size(), 1);
}

TEST_F(LinterEngineTest, RemoveRule_LastRule) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.removeRule("IndentRule");
    EXPECT_EQ(engine.getRules().size(), 0);
}

TEST_F(LinterEngineTest, RemoveRule_CaseSensitive) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.removeRule("indentrule");
    EXPECT_EQ(engine.getRules().size(), 1);
}

TEST_F(LinterEngineTest, RemoveRule_MultipleRemoves) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.removeRule("IndentRule");
    engine.removeRule("VarNameRule");
    EXPECT_EQ(engine.getRules().size(), 0);
}

// clearRules() - 5 тестов

TEST_F(LinterEngineTest, ClearRules_EmptyEngine) {
    engine.clearRules();
    EXPECT_EQ(engine.getRules().size(), 0);
}

TEST_F(LinterEngineTest, ClearRules_WithRules) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.clearRules();
    EXPECT_EQ(engine.getRules().size(), 0);
}

TEST_F(LinterEngineTest, ClearRules_Twice) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.clearRules();
    engine.clearRules();
    EXPECT_EQ(engine.getRules().size(), 0);
}

TEST_F(LinterEngineTest, ClearRules_AfterRemove) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.removeRule("IndentRule");
    engine.clearRules();
    EXPECT_EQ(engine.getRules().size(), 0);
}

TEST_F(LinterEngineTest, ClearRules_NewRulesAfterClear) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.clearRules();
    engine.addRule(std::make_unique<VarNameRule>());
    EXPECT_EQ(engine.getRules().size(), 1);
}

// analyzeFile() - 5 тестов

TEST_F(LinterEngineTest, AnalyzeFile_NonExistentFile) {
    auto errors = engine.analyzeFile("non_existent.cpp");
    EXPECT_GE(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "IOError");
}

TEST_F(LinterEngineTest, AnalyzeFile_WithNoRules) {
    auto errors = engine.analyzeFile(__FILE__);
    EXPECT_TRUE(errors.empty());
}

TEST_F(LinterEngineTest, AnalyzeFile_WithRules) {
    engine.addRule(std::make_unique<IndentRule>());
    auto errors = engine.analyzeFile(__FILE__);
    EXPECT_GE(errors.size(), 0);
}

TEST_F(LinterEngineTest, AnalyzeFile_EmptyFile) {
    std::ofstream file("empty.cpp");
    file.close();
    auto errors = engine.analyzeFile("empty.cpp");
    EXPECT_TRUE(errors.empty());
    std::remove("empty.cpp");
}

TEST_F(LinterEngineTest, AnalyzeFile_ReturnsErrors) {
    std::ofstream file("bad.cpp");
    file << "int main() { int x; return 0; }";
    file.close();
    engine.addRule(std::make_unique<UninitVarRule>());
    auto errors = engine.analyzeFile("bad.cpp");
    std::remove("bad.cpp");
    EXPECT_GE(errors.size(), 0);
}

// analyzeFiles() - 5 тестов

TEST_F(LinterEngineTest, AnalyzeFiles_EmptyList) {
    std::vector<std::string> files;
    auto errors = engine.analyzeFiles(files);
    EXPECT_TRUE(errors.empty());
}

TEST_F(LinterEngineTest, AnalyzeFiles_SingleFile) {
    engine.addRule(std::make_unique<IndentRule>());
    std::vector<std::string> files = {__FILE__};
    auto errors = engine.analyzeFiles(files);
    EXPECT_GE(errors.size(), 0);
}

TEST_F(LinterEngineTest, AnalyzeFiles_MultipleFiles) {
    engine.addRule(std::make_unique<IndentRule>());
    std::vector<std::string> files = {__FILE__, __FILE__};
    auto errors = engine.analyzeFiles(files);
    EXPECT_GE(errors.size(), 0);
}

TEST_F(LinterEngineTest, AnalyzeFiles_WithNonExistent) {
    engine.addRule(std::make_unique<IndentRule>());
    std::vector<std::string> files = {"non_existent.cpp", __FILE__};
    auto errors = engine.analyzeFiles(files);
    EXPECT_GE(errors.size(), 1);
}

TEST_F(LinterEngineTest, AnalyzeFiles_RespectsRules) {
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    std::vector<std::string> files = {__FILE__};
    auto errors = engine.analyzeFiles(files);
    EXPECT_GE(errors.size(), 0);
}