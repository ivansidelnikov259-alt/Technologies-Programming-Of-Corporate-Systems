/**
 * Сценарий: Рекурсивный анализ директории
 * 
 * Демонстрирует:
 * 1. Сканирование директории с подпапками
 * 2. Анализ всех найденных файлов
 * 3. Сводный отчёт
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include "core/LinterEngine.h"
#include "core/ReportManager.h"
#include "rules/IndentRule.h"
#include "rules/VarNameRule.h"
#include "utils/FileIterator.h"

using namespace linter;

void createTestProject() {
    std::filesystem::create_directory("test_project");
    std::filesystem::create_directory("test_project/src");
    std::filesystem::create_directory("test_project/include");
    
    std::ofstream file1("test_project/src/main.cpp");
    file1 << "int main() { return 0; }\n";
    file1.close();
    
    std::ofstream file2("test_project/include/MyClass.h");
    file2 << "class MyClass { int x; };\n";
    file2.close();
    
    std::ofstream file3("test_project/src/MyClass.cpp");
    file3 << "#include \"MyClass.h\"\n";
    file3 << "void test() { int badVar; }\n";
    file3.close();
}

int main() {
    std::cout << "=== СЦЕНАРИЙ 2: Рекурсивный анализ директории ===\n\n";
    
    createTestProject();
    
    // Поиск файлов
    FileIterator iterator;
    iterator.scanDirectory("test_project");
    
    std::cout << "Найдено файлов: " << iterator.size() << "\n";
    for (const auto& file : iterator.getAllFiles()) {
        std::cout << "  - " << file << "\n";
    }
    std::cout << "\n";
    
    // Анализ
    LinterEngine engine;
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    
    std::vector<Error> allErrors = engine.analyzeFiles(iterator.getAllFiles());
    
    // Отчёт
    ReportManager::getInstance().clearErrors();
    ReportManager::getInstance().addErrors(allErrors);
    ReportManager::getInstance().printToConsole();
    
    // Очистка
    std::filesystem::remove_all("test_project");
    
    return 0;
}