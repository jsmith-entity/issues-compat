#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <filesystem>
#include <vector>
#include <optional>

#include "task.hpp"

typedef std::filesystem::path path;
typedef std::filesystem::directory_iterator dir_iter;

class TaskManager {
public:
    TaskManager(const std::filesystem::path& task_dir);

    void read();
    void print() const;
    int size() const;

    void create_issues() const;
private:
    std::filesystem::path task_path;
    std::vector<Task> tasks;

    void read_dir(const path& dir_path);
    void read_task(const path& file_path);

    void check_label(const std::string label) const;
    void create_label(const std::string label) const;
    std::string extract_issue_num(std::string issue_url) const;
};

#endif
