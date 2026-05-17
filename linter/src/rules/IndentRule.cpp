#include "IndentRule.h"

namespace linter {

std::vector<Error> IndentRule::check(const std::vector<std::string>& lines,
                                      const std::string& filename) {
    std::vector<Error> errors;
    
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty()) continue;
        
        bool hasTab = false;
        int spaceCount = 0;
        size_t pos = 0;
        
        while (pos < line.length() && (line[pos] == ' ' || line[pos] == '\t')) {
            if (line[pos] == '\t') {
                hasTab = true;
                break;
            }
            if (line[pos] == ' ') {
                spaceCount++;
            }
            pos++;
        }
        
        if (hasTab) {
            errors.emplace_back(filename, i + 1, pos + 1,
                                "IndentError",
                                "Tab character detected. Use 4 spaces instead.");
        } 
        else if (spaceCount > 0 && spaceCount % 4 != 0) {
            errors.emplace_back(filename, i + 1, 1,
                                "IndentError",
                                "Indent must be multiple of 4 spaces (found " + 
                                std::to_string(spaceCount) + " spaces).");
        }
    }
    
    return errors;
}

}