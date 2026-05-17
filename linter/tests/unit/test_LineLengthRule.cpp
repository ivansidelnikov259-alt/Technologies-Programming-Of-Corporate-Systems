#include <gtest/gtest.h>
#include "rules/LineLengthRule.h"

using namespace linter;

class LineLengthRuleTest : public ::testing::Test {
protected:
    LineLengthRule rule;
    const std::string filename = "test.cpp";
};

// Тест 1: Короткая строка (корректно)
TEST_F(LineLengthRuleTest, ShortLineCorrect) {
    std::vector<std::string> lines = {"int x = 5;"};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 2: Строка ровно 80 символов
TEST_F(LineLengthRuleTest, Exactly80Chars) {
    std::string line(80, 'a');
    std::vector<std::string> lines = {line};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 3: Строка 81 символ (ошибка)
TEST_F(LineLengthRuleTest, OneCharOverLimit) {
    std::string line(81, 'a');
    std::vector<std::string> lines = {line};
    auto errors = rule.check(lines, filename);
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "LineLengthError");
    EXPECT_TRUE(errors[0].message.find("81") != std::string::npos);
}

// Тест 4: Длинная строка с комментарием
TEST_F(LineLengthRuleTest, LongLineWithComment) {
    std::string line(90, 'a');
    line += " // comment";
    std::vector<std::string> lines = {line};
    auto errors = rule.check(lines, filename);
    ASSERT_EQ(errors.size(), 1);
}

// Тест 5: Пустая строка
TEST_F(LineLengthRuleTest, EmptyLine) {
    std::vector<std::string> lines = {""};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 6: Строка с пробелами в конце
TEST_F(LineLengthRuleTest, TrailingSpaces) {
    std::string line(78, 'a');
    line += "  ";
    std::vector<std::string> lines = {line};
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

// Тест 7: Несколько строк, одна длинная
TEST_F(LineLengthRuleTest, MultipleLinesOneLong) {
    std::vector<std::string> lines = {
        "short line",
        std::string(100, 'b'),
        "another short line"
    };
    auto errors = rule.check(lines, filename);
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].line, 2);
}