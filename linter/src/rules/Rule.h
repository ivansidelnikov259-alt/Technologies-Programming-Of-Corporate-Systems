#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include "../core/Error.h"

namespace linter {

class Rule {
public:
    virtual ~Rule() = default;
    
    virtual std::vector<Error> check(const std::vector<std::string>& lines, 
                                      const std::string& filename = "") = 0;
    
    virtual std::string getName() const = 0;
};

}

#endif