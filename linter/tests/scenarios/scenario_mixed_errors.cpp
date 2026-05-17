/**
 * Сценарий: Файл со смешанными ошибками всех типов
 * 
 * Демонстрирует:
 * 1. Обнаружение ошибок всех пяти правил
 * 2. Группировка отчёта по типам
 */

#include <iostream>
#include <fstream>
#include "core/LinterEngine.h"
#include "core/ReportManager.h"
#include "rules/IndentRule.h"
#include "rules/VarNameRule.h"
#include "rules/LineLengthRule.h"
#include "rules/UninitVarRule.h"
#include "rules/MemoryLeakRule.h"

using namespace linter;

void createMixedErrorsFile() {
    std::ofstream file("mixed_errors.cpp");
    file << "// File with all types of errors\n";
    file << "\tint globalVar = 10;  // TAB ERROR\n";
    file << "\n";
    file << "int main() {\n";
    file << "  int x;  // INDENT ERROR (2 spaces), UNINIT ERROR\n";
    file << "    int camelCaseVar = 20;  // VARNAME ERROR\n";
    file << "    int* ptr = new int(5);  // MEMORY LEAK (no delete)\n";
    file << "    std::string very_long_variable_name_that_exceeds_eighty_characters_limit_and_should_cause_line_length_error();\n";
    file << "    return 0;\n";
    file << "}\n";
    file.close();
}

int main() {
    std::cout << "=== СЦЕНАРИЙ 4: Файл со смешанными ошибками ===\n\n";
    
    createMixedErrorsFile();
    
    LinterEngine engine;
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.addRule(std::make_unique<LineLengthRule>());
    engine.addRule(std::make_unique<UninitVarRule>());
    engine.addRule(std::make_unique<MemoryLeakRule>());
    
    std::vector<Error> errors = engine.analyzeFile("mixed_errors.cpp");
    
    ReportManager::getInstance().clearErrors();
    ReportManager::getInstance().addErrors(errors);
    ReportManager::getInstance().printToConsole();
    
    std::cout << "\n=== РАСПРЕДЕЛЕНИЕ ПО ТИПАМ ===\n";
    std::cout << "IndentError:      " << ReportManager::getInstance().getErrorCountByType("IndentError") << "\n";
    std::cout << "VarNameError:     " << ReportManager::getInstance().getErrorCountByType("VarNameError") << "\n";
    std::cout << "LineLengthError:  " << ReportManager::getInstance().getErrorCountByType("LineLengthError") << "\n";
    std::cout << "UninitVarError:   " << ReportManager::getInstance().getErrorCountByType("UninitVarError") << "\n";
    std::cout << "MemoryLeakError:  " << ReportManager::getInstance().getErrorCountByType("MemoryLeakError") << "\n";
    
    std::remove("mixed_errors.cpp");
    
    return 0;
}