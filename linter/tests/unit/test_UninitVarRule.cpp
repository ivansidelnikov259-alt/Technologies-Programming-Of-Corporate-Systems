#include <gtest/gtest.h>
#include "rules/UninitVarRule.h"

using namespace linter;

class UninitVarRuleTest : public ::testing::Test {
protected:
    UninitVarRule rule;
    const std::string filename = "test.cpp";
};

TEST_F(UninitVarRuleTest, InitializedVariableCorrect) {
    std::vector<std::string> lines = {"int x = 5;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(UninitVarRuleTest, UninitializedVariableError) {
    std::vector<std::string> lines = {"int x;"};
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "UninitVarError");
}

TEST_F(UninitVarRuleTest, ConstructorInitialization) {
    std::vector<std::string> lines = {"int x(5);"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(UninitVarRuleTest, UniformInitialization) {
    std::vector<std::string> lines = {"int x{5};"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(UninitVarRuleTest, MultipleDeclarationsOneUninit) {
    std::vector<std::string> lines = {"int a = 1, b, c = 3;"};
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_TRUE(errors[0].message.find("b") != std::string::npos);
}

TEST_F(UninitVarRuleTest, InsideFunction) {
    std::vector<std::string> lines = {
        "void foo() {",
        "    int uninit_var;",
        "    int init_var = 10;",
        "}"
    };
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_TRUE(errors[0].message.find("uninit_var") != std::string::npos);
}

TEST_F(UninitVarRuleTest, VariableInCommentIgnored) {
    std::vector<std::string> lines = {"// int commented_var;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}