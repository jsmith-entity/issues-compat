#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <optional>
#include <cassert>

#include "task-manager.hpp"
#include "utils.h"

TaskManager::TaskManager(const path& task_path) {
    std::filesystem::directory_entry task_dir(task_path); 
    if (!task_dir.is_directory()) {
        std::cerr << "[error] task path is not a directory: " << task_path
            << std::endl;
    }

    this->task_path = task_path;
}

void TaskManager::read() {
    read_dir(this->task_path);
}

void TaskManager::read_dir(const path& dir_path) {
    for (const auto& entry : dir_iter(dir_path)) {
        if (entry.is_directory()) {
            read_dir(entry.path());
        } else {
            read_task(entry.path());
        }
    }
}

void TaskManager::read_task(const path& dir_path) {
    auto filename = dir_path.filename();
    if (filename != "task.md") {
        return;
    }

    this->tasks.push_back(Task(dir_path));    
}

void TaskManager::print() const {
    for (const auto& task : this->tasks) {
        task.print();
    }
}

int TaskManager::size() const {
    return this->tasks.size();
}

void TaskManager::create_issues() const {
    const std::string list_cmd_template = "gh issue list | grep ";
    for (const auto& task : this->tasks) {
        auto labels = task.get_tags();
        for (const auto& label : labels) {
            check_label(label);
        }

        std::string task_title = task.get_title();
        std::ostringstream list_cmd;
        list_cmd << list_cmd_template << "'" << task_title << "'";
        auto found_issues = exec(list_cmd.str());
        if (!found_issues) {
            return;
        }

        if (found_issues == "") {
            std::cout << "[info] creating issue '" << task_title
                << "'.." << std::endl;

            std::string issue_cmd = task.create_issue_cmd();
            auto issue_url = exec(issue_cmd);
            if (!issue_url) {
                return;
            }
        } else {
          // update existing issue
            std::cout << "[info] issue '" << task_title
                << "' already exists. skipping.." << std::endl;
        }
    }
}

std::string TaskManager::extract_issue_num(std::string issue_url) const {
    size_t last_slash_pos = issue_url.find_last_of('/');
    assert(last_slash_pos != std::string::npos);

    return issue_url.substr(last_slash_pos + 1);
}

void TaskManager::check_label(const std::string label) const {
    std::ostringstream find_cmd;
    find_cmd << "gh label list | grep '" << label << "'";

    auto found_label = exec(find_cmd.str());
    if (!found_label) {
        return;
    }

    if (found_label == "") {
      std::cout << "[info] label '" << label << "' not found. creating.."
                << std::endl;
      create_label(label);
    }
}

void TaskManager::create_label(const std::string label) const {
    std::ostringstream creation_cmd;
    creation_cmd << "gh label create '" << label << "'";
    try {
        exec(creation_cmd.str());
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}
