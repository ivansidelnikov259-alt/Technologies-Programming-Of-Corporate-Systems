#include "MemoryLeakRule.h"
#include <regex>
#include <map>
#include <set>
#include <sstream>

namespace linter {

std::vector<Error> MemoryLeakRule::check(const std::vector<std::string>& lines,
                                          const std::string& filename) {
    std::vector<Error> errors;
    
    std::map<std::string, int> allocated;
    std::set<std::string> deleted;
    
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        
        size_t commentPos = line.find("//");
        std::string code = (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;
        
        std::regex rawNewRegex{R"((?:^|;)\s*new\s+[a-zA-Z_][a-zA-Z0-9_]*\s*[;\(])"};
        std::smatch match;
        std::string::const_iterator rawSearchStart(code.cbegin());
        
        while (std::regex_search(rawSearchStart, code.cend(), match, rawNewRegex)) {
            errors.emplace_back(filename, i + 1, match.position() + 1,
                                "MemoryLeakWarning",
                                "Potential memory leak: 'new' without assignment");
            rawSearchStart = match[0].second;
        }
        
        std::regex newAssignRegex{R"(([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*new\s+[a-zA-Z_][a-zA-Z0-9_]*\s*[;\(])"};
        std::string::const_iterator searchStart(code.cbegin());
        
        while (std::regex_search(searchStart, code.cend(), match, newAssignRegex)) {
            std::string varName = match[1];
            allocated[varName] = i + 1;
            searchStart = match[0].second;
        }
        
        std::regex deleteRegex{R"(delete\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)"};
        searchStart = code.cbegin();
        while (std::regex_search(searchStart, code.cend(), match, deleteRegex)) {
            std::string varName = match[1];
            deleted.insert(varName);
            searchStart = match[0].second;
        }
        
        std::regex deleteArrayRegex{R"(delete\[\]\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)"};
        searchStart = code.cbegin();
        while (std::regex_search(searchStart, code.cend(), match, deleteArrayRegex)) {
            std::string varName = match[1];
            deleted.insert(varName);
            searchStart = match[0].second;
        }
    }
    
    for (const auto& [varName, lineNum] : allocated) {
        if (deleted.find(varName) == deleted.end()) {
            errors.emplace_back(filename, lineNum, 1,
                                "MemoryLeakError",
                                "Memory leak: '" + varName + "' allocated with 'new' but never deleted");
        }
    }
    
    return errors;
}

}