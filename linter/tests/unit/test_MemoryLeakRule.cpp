#include <gtest/gtest.h>
#include "rules/MemoryLeakRule.h"

using namespace linter;

class MemoryLeakRuleTest : public ::testing::Test {
protected:
    MemoryLeakRule rule;
    const std::string filename = "test.cpp";
};

TEST_F(MemoryLeakRuleTest, NewAndDeleteCorrect) {
    std::vector<std::string> lines = {
        "int* ptr = new int(5);",
        "delete ptr;"
    };
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(MemoryLeakRuleTest, NewWithoutDeleteError) {
    std::vector<std::string> lines = {"int* ptr = new int(5);"};
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].type, "MemoryLeakError");
}

TEST_F(MemoryLeakRuleTest, MultipleNewOneDelete) {
    std::vector<std::string> lines = {
        "int* ptr1 = new int(1);",
        "int* ptr2 = new int(2);",
        "delete ptr1;"
    };
    auto errors = rule.check(lines, filename);
    EXPECT_EQ(errors.size(), 1);
    EXPECT_TRUE(errors[0].message.find("ptr2") != std::string::npos);
}

TEST_F(MemoryLeakRuleTest, RawNewWithoutAssignment) {
    std::vector<std::string> lines = {"new int(5);"};
    auto errors = rule.check(lines, filename);
    EXPECT_GE(errors.size(), 1);
}

TEST_F(MemoryLeakRuleTest, DeleteWithoutNew) {
    std::vector<std::string> lines = {"delete ptr;"};
    auto errors = rule.check(lines, filename);
    bool hasLeakError = false;
    for (const auto& err : errors) {
        if (err.type == "MemoryLeakError") hasLeakError = true;
    }
    EXPECT_FALSE(hasLeakError);
}

TEST_F(MemoryLeakRuleTest, NestedScopes) {
    std::vector<std::string> lines = {
        "if (true) {",
        "    int* ptr = new int(5);",
        "    delete ptr;",
        "}"
    };
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}

TEST_F(MemoryLeakRuleTest, NewInClass) {
    std::vector<std::string> lines = {
        "class MyClass {",
        "    int* data;",
        "    MyClass() { data = new int(5); }",
        "    ~MyClass() { delete data; }",
        "};"
    };
    auto errors = rule.check(lines, filename);
    EXPECT_TRUE(errors.empty());
}