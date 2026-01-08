#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <filesystem>

class Task {
public:
    Task(const std::filesystem::path& file_path);
    void print() const;
private:
    std::string title;
    std::string status;
    std::string priority;
    std::vector<std::string> tags;
    std::string description;

    std::vector<std::string> extract_tags(const std::string& line);
};

#endif
