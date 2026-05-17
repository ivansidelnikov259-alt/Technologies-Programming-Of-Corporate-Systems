#ifndef UNINIT_VAR_RULE_H
#define UNINIT_VAR_RULE_H

#include "Rule.h"
#include <set>

namespace linter {

class UninitVarRule : public Rule {
public:
    std::vector<Error> check(const std::vector<std::string>& lines,
                              const std::string& filename = "") override;
    
    std::string getName() const override { return "UninitVarRule"; }
    
private:
    bool isInitialized(const std::string& line, const std::string& varName);
};

}

#endif