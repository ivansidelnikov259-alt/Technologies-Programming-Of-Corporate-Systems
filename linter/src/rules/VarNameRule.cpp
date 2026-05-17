#include "VarNameRule.h"
#include <regex>
#include <sstream>
#include <vector>
#include <algorithm>

namespace linter {

std::vector<Error> VarNameRule::check(const std::vector<std::string>& lines,
                                       const std::string& filename) {
    std::vector<Error> errors;
    
    std::regex snakeCasePattern{"^[a-z][a-z0-9_]*$"};
    
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        
        size_t commentPos = line.find("//");
        std::string code = (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;
        
        std::regex typeRegex{R"((int|float|char|bool)\s+)"};
        std::smatch typeMatch;
        
        if (std::regex_search(code, typeMatch, typeRegex)) {
            size_t typeEndPos = typeMatch.position(0) + typeMatch[0].length();
            std::string rest = code.substr(typeEndPos);
            
            std::vector<std::string> varDecls;
            std::stringstream ss(rest);
            std::string item;
            
            while (std::getline(ss, item, ',')) {
                item.erase(0, item.find_first_not_of(" \t"));
                item.erase(item.find_last_not_of(" \t") + 1);
                if (item.back() == ';') item.pop_back();
                if (!item.empty()) {
                    varDecls.push_back(item);
                }
            }
            
            for (const auto& decl : varDecls) {
                std::regex varNameRegex{R"(^([a-zA-Z_][a-zA-Z0-9_]*))"};
                std::smatch varMatch;
                
                if (std::regex_search(decl, varMatch, varNameRegex)) {
                    std::string varName = varMatch[1];
                    
                    if (!std::regex_match(varName, snakeCasePattern)) {
                        size_t pos = line.find(varName);
                        if (pos == std::string::npos) pos = 0;
                        errors.emplace_back(filename, i + 1, pos + 1,
                                            "VarNameError",
                                            "Variable '" + varName + 
                                            "' should be in snake_case");
                    }
                }
            }
        }
    }
    
    return errors;
}

}