#include "FileReader.h"
#include <fstream>
#include <iostream>

namespace linter {

std::vector<std::string> FileReader::readFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return lines;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    return lines;
}

bool FileReader::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

}