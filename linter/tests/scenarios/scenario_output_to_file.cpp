/**
 * Сценарий: Экспорт отчёта в файл
 * 
 * Демонстрирует:
 * 1. Анализ файла с ошибками
 * 2. Сохранение отчёта в файл
 * 3. Проверка содержимого файла
 */

#include <iostream>
#include <fstream>
#include "core/LinterEngine.h"
#include "core/ReportManager.h"
#include "rules/IndentRule.h"
#include "rules/VarNameRule.h"

using namespace linter;

void createTestFileWithErrors() {
    std::ofstream file("error_example.cpp");
    file << "int main() {\n";
    file << "  int x;  // плохой отступ\n";
    file << "    int badVar;  // camelCase\n";
    file << "    return 0;\n";
    file << "}\n";
    file.close();
}

int main() {
    std::cout << "=== СЦЕНАРИЙ 3: Экспорт отчёта в файл ===\n\n";
    
    createTestFileWithErrors();
    
    LinterEngine engine;
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    
    std::vector<Error> errors = engine.analyzeFile("error_example.cpp");
    
    ReportManager::getInstance().clearErrors();
    ReportManager::getInstance().addErrors(errors);
    
    // Сохранение в файл
    std::string outputFile = "report_output.txt";
    if (ReportManager::getInstance().saveToFile(outputFile)) {
        std::cout << "Отчёт сохранён в файл: " << outputFile << "\n";
        
        // Показываем содержимое
        std::cout << "\nСодержимое отчёта:\n";
        std::cout << "-------------------\n";
        std::ifstream reportFile(outputFile);
        std::string line;
        while (std::getline(reportFile, line)) {
            std::cout << line << "\n";
        }
        reportFile.close();
    } else {
        std::cout << "Ошибка сохранения отчёта\n";
    }
    
    std::remove("error_example.cpp");
    std::remove("report_output.txt");
    
    return 0;
}