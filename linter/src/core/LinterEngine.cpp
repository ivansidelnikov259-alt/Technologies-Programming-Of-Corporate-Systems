#include "LinterEngine.h"
#include "../rules/Rule.h"
#include "../utils/FileReader.h"
#include <algorithm>

namespace linter {

LinterEngine::LinterEngine() = default;
LinterEngine::~LinterEngine() = default;

void LinterEngine::addRule(std::unique_ptr<Rule> rule) {
    rules.push_back(std::move(rule));
}

void LinterEngine::removeRule(const std::string& ruleName) {
    auto it = std::remove_if(rules.begin(), rules.end(),
        [&ruleName](const std::unique_ptr<Rule>& rule) {
            return rule->getName() == ruleName;
        });
    rules.erase(it, rules.end());
}

void LinterEngine::clearRules() {
    rules.clear();
}

std::vector<Error> LinterEngine::analyzeFile(const std::string& filename) {
    std::vector<Error> allErrors;
    
    std::vector<std::string> lines = FileReader::readFile(filename);
    if (lines.empty() && !FileReader::fileExists(filename)) {
        Error err;
        err.filename = filename;
        err.message = "File not found or empty";
        err.type = "IOError";
        allErrors.push_back(err);
        return allErrors;
    }
    
    for (const auto& rule : rules) {
        std::vector<Error> errors = rule->check(lines, filename);
        allErrors.insert(allErrors.end(), errors.begin(), errors.end());
    }
    
    return allErrors;
}

std::vector<Error> LinterEngine::analyzeFiles(const std::vector<std::string>& filenames) {
    std::vector<Error> allErrors;
    
    for (const auto& filename : filenames) {
        std::vector<Error> fileErrors = analyzeFile(filename);
        allErrors.insert(allErrors.end(), fileErrors.begin(), fileErrors.end());
    }
    
    return allErrors;
}

}