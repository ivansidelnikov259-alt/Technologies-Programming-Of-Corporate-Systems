#ifndef LINTER_ENGINE_H
#define LINTER_ENGINE_H

#include <vector>
#include <memory>
#include "Error.h"

namespace linter {

class Rule;

class LinterEngine {
public:
    LinterEngine();
    ~LinterEngine();
    
    // Паттерн Стратегия
    void addRule(std::unique_ptr<Rule> rule);
    void removeRule(const std::string& ruleName);
    void clearRules();
    
    std::vector<Error> analyzeFile(const std::string& filename);
    std::vector<Error> analyzeFiles(const std::vector<std::string>& filenames);
    
    const std::vector<std::unique_ptr<Rule>>& getRules() const { return rules; }
    
private:
    std::vector<std::unique_ptr<Rule>> rules;
};

}

#endif