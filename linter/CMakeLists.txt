cmake_minimum_required(VERSION 3.14)
project(Linter VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(linter_core STATIC
    src/core/LinterEngine.cpp
    src/core/ReportManager.cpp
    src/rules/IndentRule.cpp
    src/rules/VarNameRule.cpp
    src/rules/LineLengthRule.cpp
    src/rules/UninitVarRule.cpp
    src/rules/MemoryLeakRule.cpp
    src/utils/FileReader.cpp
    src/utils/FileIterator.cpp
)

target_include_directories(linter_core PRIVATE src)


add_executable(linter src/main.cpp)
target_link_libraries(linter linter_core)


enable_testing()
add_subdirectory(tests)