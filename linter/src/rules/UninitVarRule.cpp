#include "UninitVarRule.h"
#include <regex>
#include <set>
#include <sstream>
#include <vector>
#include <algorithm>

namespace linter {

bool UninitVarRule::isInitialized(const std::string& line, const std::string& varName) {
    size_t varPos = line.find(varName);
    if (varPos == std::string::npos) return false;
    
    std::string afterVar = line.substr(varPos + varName.length());
    
    size_t start = afterVar.find_first_not_of(" \t");
    if (start == std::string::npos) return false;
    
    char nextChar = afterVar[start];
    return (nextChar == '=' || nextChar == '(' || nextChar == '{');
}

std::vector<Error> UninitVarRule::check(const std::vector<std::string>& lines,
                                         const std::string& filename) {
    std::vector<Error> errors;
    
    std::regex varDeclPattern{R"((int|float|char|bool)\s+([a-zA-Z_][a-zA-Z0-9_]*))"};
    
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        
        size_t commentPos = line.find("//");
        std::string code = (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;
        
        std::regex typeRegex{R"((int|float|char|bool)\s+)"};
        std::smatch typeMatch;
        
        if (std::regex_search(code, typeMatch, typeRegex)) {
            std::string type = typeMatch[1];
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
                    
                    std::string afterVarName = decl.substr(varMatch[0].length());
                    afterVarName.erase(0, afterVarName.find_first_not_of(" \t"));
                    
                    bool hasInit = false;
                    if (!afterVarName.empty() && (afterVarName[0] == '=' || afterVarName[0] == '(' || afterVarName[0] == '{')) {
                        hasInit = true;
                    }
                    
                    if (!hasInit) {
                        errors.emplace_back(filename, i + 1,
                                            line.find(varName) + 1,
                                            "UninitVarError",
                                            "Variable '" + varName + 
                                            "' declared but not initialized");
                    }
                }
            }
        }
    }
    
    return errors;
}

}