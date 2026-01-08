#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include "task-reader.hpp"

TaskReader::TaskReader(const path& task_path) {
    std::filesystem::directory_entry task_dir(task_path); 
    if (!task_dir.is_directory()) {
        std::cerr << "[error] task path is not a directory: " << task_path
            << std::endl;
    }

    this->task_path = task_path;
}

void TaskReader::read() {
    read_dir(this->task_path);
}

void TaskReader::read_dir(const path& dir_path) {
    for (const auto& entry : dir_iter(dir_path)) {
        if (entry.is_directory()) {
            read_dir(entry.path());
        } else {
            read_task(entry.path());
        }
    }
}

void TaskReader::read_task(const path& dir_path) {
    auto filename = dir_path.filename();
    if (filename != "task.md") {
        return;
    }

    this->tasks.push_back(Task(dir_path));    
}

void TaskReader::print() const {
    for (const auto& task : this->tasks) {
        task.print();
    }
}
