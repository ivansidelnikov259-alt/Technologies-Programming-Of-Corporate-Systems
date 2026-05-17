/**
 * Сценарий: Корректный код без ошибок
 * 
 * Демонстрирует:
 * 1. Анализ идеального кода
 * 2. Отчёт "No issues found"
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

void createPerfectCode() {
    std::ofstream file("perfect_code.cpp");
    file << "#include <iostream>\n";
    file << "\n";
    file << "int main() {\n";
    file << "    int initialized_var = 10;\n";
    file << "    int* ptr = new int(5);\n";
    file << "    delete ptr;\n";
    file << "    std::cout << \"Hello, World!\" << std::endl;\n";
    file << "    return 0;\n";
    file << "}\n";
    file.close();
}

int main() {
    std::cout << "=== СЦЕНАРИЙ 5: Корректный код (нет ошибок) ===\n\n";
    
    createPerfectCode();
    
    LinterEngine engine;
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.addRule(std::make_unique<LineLengthRule>());
    engine.addRule(std::make_unique<UninitVarRule>());
    engine.addRule(std::make_unique<MemoryLeakRule>());
    
    std::vector<Error> errors = engine.analyzeFile("perfect_code.cpp");
    
    ReportManager::getInstance().clearErrors();
    ReportManager::getInstance().addErrors(errors);
    ReportManager::getInstance().printToConsole();
    
    std::cout << "\nОжидаемый результат: \"No issues found. Code looks good!\"\n";
    
    std::remove("perfect_code.cpp");
    
    return 0;
}