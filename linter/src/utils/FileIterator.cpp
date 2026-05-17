#include "FileIterator.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

namespace linter {

const std::vector<std::string> FileIterator::validExtensions = {".cpp", ".cpr", ".h", ".hpp"};

void FileIterator::scanDirectory(const std::string& path) {
    files.clear();
    currentIndex = 0;
    
    if (fs::exists(path) && fs::is_regular_file(path)) {
        // Это файл
        if (hasValidExtension(path)) {
            files.push_back(path);
        }
    } else if (fs::exists(path) && fs::is_directory(path)) {
        scanDirectoryRecursive(path);
    }
}

void FileIterator::scanDirectoryRecursive(const std::string& path) {
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().string();
            if (hasValidExtension(filename)) {
                files.push_back(filename);
            }
        }
    }
}

bool FileIterator::hasValidExtension(const std::string& filename) const {
    fs::path p(filename);
    std::string ext = p.extension().string();
    
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return std::find(validExtensions.begin(), validExtensions.end(), ext) != validExtensions.end();
}

bool FileIterator::hasNext() const {
    return currentIndex < files.size();
}

std::string FileIterator::next() {
    if (hasNext()) {
        return files[currentIndex++];
    }
    return "";
}

void FileIterator::reset() {
    currentIndex = 0;
}

}