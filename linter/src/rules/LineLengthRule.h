#ifndef LINE_LENGTH_RULE_H
#define LINE_LENGTH_RULE_H

#include "Rule.h"

namespace linter {

class LineLengthRule : public Rule {
public:
    static constexpr int MAX_LINE_LENGTH = 80;
    
    std::vector<Error> check(const std::vector<std::string>& lines,
                              const std::string& filename = "") override;
    
    std::string getName() const override { return "LineLengthRule"; }
};

}

#endif