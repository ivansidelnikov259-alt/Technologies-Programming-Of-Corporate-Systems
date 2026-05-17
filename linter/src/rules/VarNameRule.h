#ifndef VAR_NAME_RULE_H
#define VAR_NAME_RULE_H

#include "Rule.h"
#include <regex>

namespace linter {

class VarNameRule : public Rule {
public:
    std::vector<Error> check(const std::vector<std::string>& lines,
                              const std::string& filename = "") override;
    
    std::string getName() const override { return "VarNameRule"; }
    
private:
    std::regex snakeCasePattern{"^[a-z][a-z0-9_]*$"};
    std::regex varDeclPattern{R"((?:int|float|char|bool)\s+([a-zA-Z_][a-zA-Z0-9_]*))"};
};

}

#endif