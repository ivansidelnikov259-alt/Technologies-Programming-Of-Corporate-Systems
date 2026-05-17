#ifndef MEMORY_LEAK_RULE_H
#define MEMORY_LEAK_RULE_H

#include "Rule.h"
#include <set>
#include <map>

namespace linter {

class MemoryLeakRule : public Rule {
public:
    std::vector<Error> check(const std::vector<std::string>& lines,
                              const std::string& filename = "") override;
    
    std::string getName() const override { return "MemoryLeakRule"; }
    
private:
    struct VarInfo {
        std::string name;
        int lineDeclared;
        bool hasDelete;
    };
};

}

#endif