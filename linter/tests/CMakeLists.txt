include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Unit-тесты
add_executable(linter_unit_tests
    test_main.cpp
    
    # Правила
    unit/test_IndentRule.cpp
    unit/test_VarNameRule.cpp
    unit/test_LineLengthRule.cpp
    unit/test_UninitVarRule.cpp
    unit/test_MemoryLeakRule.cpp
    
    # Core
    unit/test_LinterEngine.cpp
    unit/test_ReportManager.cpp
    
    # Utils
    unit/test_FileReader.cpp
    unit/test_FileIterator.cpp
)

target_link_libraries(linter_unit_tests linter_core gtest_main gtest)
target_include_directories(linter_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)


foreach(scenario
    full_analysis
    recursive_scan
    output_to_file
    mixed_errors
    no_errors
)
    add_executable(scenario_${scenario} scenarios/scenario_${scenario}.cpp)
    target_link_libraries(scenario_${scenario} linter_core)
    target_include_directories(scenario_${scenario} PRIVATE ${CMAKE_SOURCE_DIR}/src)
endforeach()


include(GoogleTest)
gtest_discover_tests(linter_unit_tests)