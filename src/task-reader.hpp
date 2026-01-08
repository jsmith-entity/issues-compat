#ifndef TASK_READER_H
#define TASK_READER_H

#include <filesystem>
#include <vector>

#include "task.hpp"

typedef std::filesystem::path path;
typedef std::filesystem::directory_iterator dir_iter;

class TaskReader {
public:
    TaskReader(const std::filesystem::path& task_dir);
    void read();
    void print() const;
private:
    std::filesystem::path task_path;
    std::vector<Task> tasks;

    void read_dir(const path& dir_path);
    void read_task(const path& file_path);
};

#endif
