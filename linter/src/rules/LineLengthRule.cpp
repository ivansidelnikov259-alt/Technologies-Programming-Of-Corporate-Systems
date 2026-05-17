#include "LineLengthRule.h"

namespace linter {

std::vector<Error> LineLengthRule::check(const std::vector<std::string>& lines,
                                          const std::string& filename) {
    std::vector<Error> errors;
    
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];

        size_t length = line.length();
        if (length > 0 && line.back() == '\r') length--;
        
        if (static_cast<int>(length) > MAX_LINE_LENGTH) {
            errors.emplace_back(filename, i + 1, MAX_LINE_LENGTH + 1,
                                "LineLengthError",
                                "Line exceeds " + std::to_string(MAX_LINE_LENGTH) + 
                                " characters (actual: " + std::to_string(length) + ")");
        }
    }
    
    return errors;
}

}