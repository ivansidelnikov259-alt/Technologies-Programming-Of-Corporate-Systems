#include "ReportManager.h"
#include <iostream>
#include <sstream>
#include <map>

namespace linter {

void ReportManager::addError(const Error& error) {
    errors.push_back(error);
}

void ReportManager::addErrors(const std::vector<Error>& newErrors) {
    errors.insert(errors.end(), newErrors.begin(), newErrors.end());
}

void ReportManager::clearErrors() {
    errors.clear();
}

std::string ReportManager::generateReport() const {
    std::stringstream ss;
    
    if (errors.empty()) {
        ss << "=== LINTER REPORT ===\n";
        ss << "No issues found. Code looks good!\n";
        return ss.str();
    }
    
    ss << "=== LINTER REPORT ===\n";
    ss << "Total errors: " << errors.size() << "\n\n";
    
    std::map<std::string, int> typeCount;
    for (const auto& err : errors) {
        typeCount[err.type]++;
    }
    
    ss << "Summary by type:\n";
    for (const auto& [type, count] : typeCount) {
        ss << "  " << type << ": " << count << "\n";
    }
    ss << "\n";
    
    ss << "Details:\n";
    for (const auto& err : errors) {
        ss << err.toString() << "\n";
    }
    
    return ss.str();
}

void ReportManager::printToConsole() const {
    std::cout << generateReport();
}

bool ReportManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << generateReport();
    file.close();
    return true;
}

size_t ReportManager::getErrorCountByType(const std::string& type) const {
    size_t count = 0;
    for (const auto& err : errors) {
        if (err.type == type) {
            count++;
        }
    }
    return count;
}

}