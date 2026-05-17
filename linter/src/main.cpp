#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "core/LinterEngine.h"
#include "core/ReportManager.h"
#include "rules/IndentRule.h"
#include "rules/VarNameRule.h"
#include "rules/LineLengthRule.h"
#include "rules/UninitVarRule.h"
#include "rules/MemoryLeakRule.h"
#include "utils/FileIterator.h"

using namespace linter;

void printHelp() {
    std::cout << "Linter - Static code analyzer for educational C++ subset\n\n";
    std::cout << "Usage: linter <path> [options]\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  <path>               Path to file or directory\n\n";
    std::cout << "Options:\n";
    std::cout << "  --output <file>      Save report to file\n";
    std::cout << "  --help               Show this help message\n";
    std::cout << "  --version            Show version information\n\n";
    std::cout << "Supported extensions: .cpp, .cpr, .h, .hpp\n";
    std::cout << "Rules:\n";
    std::cout << "  - IndentRule: 4 spaces, no tabs\n";
    std::cout << "  - VarNameRule: snake_case\n";
    std::cout << "  - LineLengthRule: max 80 characters\n";
    std::cout << "  - UninitVarRule: detect uninitialized variables\n";
    std::cout << "  - MemoryLeakRule: detect new without delete\n";
}

void printVersion() {
    std::cout << "Linter version 1.0.0\n";
    std::cout << "Educational C++ static analyzer\n";
    std::cout << "Built with C++17\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No path specified.\n";
        printHelp();
        return 1;
    }
    
    std::string path = argv[1];
    std::string outputFile;
    
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            printHelp();
            return 0;
        }
        if (strcmp(argv[i], "--version") == 0) {
            printVersion();
            return 0;
        }
        if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            outputFile = argv[++i];
        }
    }
    
    LinterEngine engine;
    engine.addRule(std::make_unique<IndentRule>());
    engine.addRule(std::make_unique<VarNameRule>());
    engine.addRule(std::make_unique<LineLengthRule>());
    engine.addRule(std::make_unique<UninitVarRule>());
    engine.addRule(std::make_unique<MemoryLeakRule>());
    
    // Паттерн Итератор
    FileIterator iterator;
    iterator.scanDirectory(path);
    
    if (iterator.size() == 0) {
        std::cerr << "Error: No supported files found.\n";
        return 1;
    }
    
    std::cout << "Scanning " << iterator.size() << " file(s)...\n";
    
    // Паттерн Фасад
    std::vector<Error> allErrors = engine.analyzeFiles(iterator.getAllFiles());
    
    // Паттерн Одиночка
    ReportManager& report = ReportManager::getInstance();
    report.clearErrors();
    report.addErrors(allErrors);
    
    if (!outputFile.empty()) {
        if (report.saveToFile(outputFile)) {
            std::cout << "Report saved to " << outputFile << "\n";
        } else {
            std::cerr << "Error: Cannot write to " << outputFile << "\n";
            report.printToConsole();
        }
    } else {
        report.printToConsole();
    }
    
    return report.getErrorCount() > 0 ? 1 : 0;
}