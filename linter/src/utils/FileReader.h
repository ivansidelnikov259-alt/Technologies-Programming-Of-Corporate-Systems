#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <vector>

namespace linter {

class FileReader {
public:
    static std::vector<std::string> readFile(const std::string& filename);
    static bool fileExists(const std::string& filename);
};

}

#endif