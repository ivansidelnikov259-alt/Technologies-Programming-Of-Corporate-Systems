#include <gtest/gtest.h>
#include "rules/VarNameRule.h"

using namespace linter;

class VarNameRuleTest : public ::testing::Test {
protected:
    VarNameRule rule;
    const std::string filename = "test.cpp";
};

TEST_F(VarNameRuleTest, CorrectSnakeCase) {
    std::vector<std::string> lines = {"int my_variable = 10;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(VarNameRuleTest, CamelCaseError) {
    std::vector<std::string> lines = {"int myVariable = 10;"};
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "VarNameError");
}

TEST_F(VarNameRuleTest, UpperCaseStartError) {
    std::vector<std::string> lines = {"int MyVariable = 10;"};
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
}

TEST_F(VarNameRuleTest, SnakeCaseWithNumber) {
    std::vector<std::string> lines = {"int var1 = 10;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(VarNameRuleTest, MultipleDeclarations) {
    std::vector<std::string> lines = {"int good_var = 1, badVar = 2;"};
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_TRUE(errors[0].message.find("badVar") != std::string::npos);
}

TEST_F(VarNameRuleTest, VariableInComment) {
    std::vector<std::string> lines = {"// int badVar = 10;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(VarNameRuleTest, ConstructorInitialization) {
    std::vector<std::string> lines = {"MyClass(int good_var) : good_var(good_var) {}"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}