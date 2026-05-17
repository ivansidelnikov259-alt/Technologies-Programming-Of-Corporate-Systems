/**
 * Сценарий: Полный анализ одного файла
 * 
 * Этот сценарий демонстрирует:
 * 1. Создание LinterEngine со всеми правилами
 * 2. Анализ тестового файла
 * 3. Вывод отчёта
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

void createTestFile() {
    std::ofstream file("test_sample.cpp");
    file << "#include <iostream>\n";
    file << "\n";
    file << "int main() {\n";
    file << "  int x;  // Ошибка: табуляция? нет, но 2 пробела\n";
    file << "    int y = 10;  // Корректный отступ\n";
    file << "    int badVar = 20;  // Ошибка: CamelCase\n";
    file << "    int* ptr = new int(5);  // new без delete -> утечка\n";
    file << "    std::string very_long_line_that_exceeds_eighty_characters_limit_and_should_cause_an_error();\n";
    file << "    return 0;\n";
    file << "}\n";
    file.close();
}

int main() {
    std::cout << "=== СЦЕНАРИЙ 1: Полный анализ файла ===\n\n";
    
    createTestFile();
    
    // Настройка движка
    LinterEngine engine;
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.addRule(std::make_unique<LineLengthRule>());
    engine.addRule(std::make_unique<UninitVarRule>());
    engine.addRule(std::make_unique<MemoryLeakRule>());
    
    // Анализ
    std::vector<Error> errors = engine.analyzeFile("test_sample.cpp");
    
    // Отчёт
    ReportManager::getInstance().clearErrors();
    ReportManager::getInstance().addErrors(errors);
    ReportManager::getInstance().printToConsole();
    
    // Ожидаемый результат: несколько ошибок
    std::cout << "\nСтатистика:\n";
    std::cout << "Всего ошибок: " << errors.size() << "\n";
    
    std::remove("test_sample.cpp");
    
    return 0;
}