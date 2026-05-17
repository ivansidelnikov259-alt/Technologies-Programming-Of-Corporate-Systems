#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include "Error.h"
#include <vector>
#include <string>
#include <fstream>

namespace linter {

class ReportManager {
public:
    // Паттерн Одиночка
    static ReportManager& getInstance() {
        static ReportManager instance;
        return instance;
    }
    
    ReportManager(const ReportManager&) = delete;
    ReportManager& operator=(const ReportManager&) = delete;
    
    void addError(const Error& error);
    void addErrors(const std::vector<Error>& errors);
    void clearErrors();
    
    std::string generateReport() const;
    
    // Паттерн Фасад
    void printToConsole() const;
    bool saveToFile(const std::string& filename) const;
    
    const std::vector<Error>& getAllErrors() const { return errors; }
    
    size_t getErrorCount() const { return errors.size(); }
    size_t getErrorCountByType(const std::string& type) const;
    
private:
    ReportManager() = default;
    ~ReportManager() = default;
    
    std::vector<Error> errors;
};

}

#endif