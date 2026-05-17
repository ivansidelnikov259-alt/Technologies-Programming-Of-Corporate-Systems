#ifndef INDENT_RULE_H
#define INDENT_RULE_H

#include "Rule.h"

namespace linter {

class IndentRule : public Rule {
public:
    std::vector<Error> check(const std::vector<std::string>& lines, 
                              const std::string& filename = "") override;
    
    std::string getName() const override { return "IndentRule"; }
};

}

#endif