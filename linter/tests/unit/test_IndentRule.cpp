#include <gtest/gtest.h>
#include "rules/IndentRule.h"

using namespace linter;

class IndentRuleTest : public ::testing::Test {
protected:
    IndentRule rule;
    const std::string filename = "test.cpp";
};

// Тест 1: Корректный отступ (4 пробела)
TEST_F(IndentRuleTest, CorrectFourSpaces) {
    std::vector<std::string> lines = {"    int x = 5;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 2: Пустая строка (не должно быть ошибок)
TEST_F(IndentRuleTest, EmptyLine) {
    std::vector<std::string> lines = {""};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 3: Табуляция вместо пробелов
TEST_F(IndentRuleTest, TabCharacterError) {
    std::vector<std::string> lines = {"\tint x = 5;"};
    auto errors = rule.check(lines, filename);
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "IndentError");
    EXPECT_TRUE(errors[0].message.find("Tab") != std::string::npos);
}

// Тест 4: Смешанные пробелы и табуляция
TEST_F(IndentRuleTest, MixedSpacesAndTabs) {
    std::vector<std::string> lines = {"  \tint x = 5;"};
    auto errors = rule.check(lines, filename);
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "IndentError");
}

// Тест 5: 2 пробела (ошибка, не кратно 4)
TEST_F(IndentRuleTest, TwoSpacesError) {
    std::vector<std::string> lines = {"  int x = 5;"};
    auto errors = rule.check(lines, filename);
    ASSERT_EQ(errors.size(), 1);
    EXPECT_TRUE(errors[0].message.find("multiple of 4") != std::string::npos);
}

// Тест 6: 8 пробелов (корректно, кратно 4)
TEST_F(IndentRuleTest, EightSpacesCorrect) {
    std::vector<std::string> lines = {"        int x = 5;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 7: Строка без отступа на верхнем уровне
TEST_F(IndentRuleTest, NoIndentAtTopLevel) {
    std::vector<std::string> lines = {"int x = 5;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}