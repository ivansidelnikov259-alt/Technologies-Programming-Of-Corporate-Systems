#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace linter {

struct Error {
    std::string filename;
    int line;
    int column;
    std::string type;
    std::string message;
    
    Error() : line(0), column(0) {}
    
    Error(const std::string& file, int ln, int col, 
          const std::string& t, const std::string& msg)
        : filename(file), line(ln), column(col), type(t), message(msg) {}
    
    std::string toString() const {
        return filename + ":" + std::to_string(line) + ":" + 
               std::to_string(column) + ":" + type + ":" + message;
    }
};

}

#endif