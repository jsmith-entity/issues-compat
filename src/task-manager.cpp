#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <optional>
#include <cassert>

#include "task-manager.hpp"
#include "utils.hpp"

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

void TaskManager::create_issues() {
    for (auto& task : this->tasks) {
        auto labels = task.get_tags();
        for (const auto& label : labels) {
            check_label(trim(label));
        }

        std::string issue_cmd_open = task.find_cmd(false);
        std::string issue_cmd_closed = task.find_cmd(true);
        auto found_open = exec(issue_cmd_open);
        auto found_closed = exec(issue_cmd_closed);
        if (!found_open || !found_closed) {
            return;
        }

        bool close_check = false;
        if (found_open == "" && found_closed == "") { // create issue
            create_issue(task);
            close_check = true;
        } else if (found_open != "" && found_closed == "") { // update issue
            update_issue(task, found_open.value());
            close_check = true;
        } else {
          std::cout << "[info] task is closed: '" << task.get_title()
              << "' skipping.." << std::endl;
        }

        if (close_check) {
            update_state(task);
        }
    }
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

void TaskManager::create_issue(Task& task) const {
    std::cout << "[info] creating issue '" << task.get_title() 
        << "'.." << std::endl;

    std::string issue_cmd = task.create_issue_cmd();
    auto issue_url = exec(issue_cmd);
    if (!issue_url) {
        std::cerr << "[error] failed to create issue" << std::endl;
        return;
    }

    task.set_issue_num(issue_url.value());
}

void TaskManager::update_issue(Task &task, const std::string &issue_line) const {
    size_t pos = issue_line.find_first_of("\t ");
    assert(pos != std::string::npos);
    std::string issue_num = issue_line.substr(0, pos);
    task.set_issue_num(issue_num);

    std::string view_cmd = task.view_cmd();
    auto issue_details = exec(view_cmd);
    if (!issue_details) {
        std::cerr << "[error] failed to view issue" << std::endl;
        return;
    }

    auto update_cmd = task.update_cmd(issue_details.value());
    if (update_cmd) {
        auto update_result = exec(update_cmd.value());
        if (!update_result) {
            std::cerr << "[error] failed to update issue" << std::endl;
            return;
        }
        std::cout << "[info] updated task: '" << task.get_title() << "'"
            << std::endl;
    }
}

std::string TaskManager::extract_issue_num(std::string issue_url) const {
    size_t last_slash_pos = issue_url.find_last_of('/');
    assert(last_slash_pos != std::string::npos);

    return issue_url.substr(last_slash_pos + 1);
}

void TaskManager::update_state(const Task& task) const {
    if (task.get_status() == "COMPLETE") {
        std::cout << "[info] issue completed: '" << task.get_title()
            << "' closing.." << std::endl;

        std::string close_cmd = task.close_cmd();
        exec(close_cmd);
    }
}
