#ifndef FILE_ITERATOR_H
#define FILE_ITERATOR_H

#include <string>
#include <vector>

namespace linter {

class FileIterator {
public:
    FileIterator() = default;
    
    void scanDirectory(const std::string& path);
    
    // Паттерн Итератор
    bool hasNext() const;
    std::string next();
    void reset();
    
    size_t size() const { return files.size(); }
    const std::vector<std::string>& getAllFiles() const { return files; }
    
    static const std::vector<std::string> validExtensions;
    
private:
    void scanDirectoryRecursive(const std::string& path);
    bool hasValidExtension(const std::string& filename) const;
    
    std::vector<std::string> files;
    size_t currentIndex = 0;
};

}

#endif