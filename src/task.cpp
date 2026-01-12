#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

#include "task.hpp"

Task::Task(const std::filesystem::path& file_path) {
    const std::string TITLE = "# ";
    const std::string STATUS = "- STATUS: ";
    const std::string PRIORITY = "- PRIORITY: ";
    const std::string TAGS = "- TAGS: ";

    std::ifstream task_file(file_path);    
    if (!task_file) {
        std::cerr << "[error] unable to open file: " << file_path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(task_file, line)) {
        if (line.find(TITLE) == 0) {
            this->title = line.substr(TITLE.size());
        } else if (line.find(STATUS) == 0) {
            this->status = line.substr(STATUS.size());
        } else if (line.find(PRIORITY) == 0) {
            this->priority = line.substr(PRIORITY.size());
        } else if (line.find(TAGS) == 0) {
            this->tags = extract_tags(line.substr(TAGS.size()));
        } else {
            this->description += line;
        }
    }
}

std::vector<std::string> Task::extract_tags(const std::string& line) {
    std::vector<std::string> tags;
    std::stringstream ss(line);
    std::string tag;
    while (std::getline(ss, tag, ',')) {
        tags.push_back(tag);
    }

    return tags;
}

void Task::print() const {
    std::cout << this->title << std::endl;
    std::cout << this->status << std::endl;
    std::cout << this->priority << std::endl;
    std::cout << "tags:" << std::endl;
    for (const auto& tag : this->tags) {
        std::cout << "  " << tag << std::endl;
    }
    std::cout << this->description << std::endl;
} 

std::string Task::create_issue_cmd() const {
    std::ostringstream cmd;
    cmd << "gh issue create --title '" << this->title << "'";
    cmd << " --body 'PRIORITY: " << this->priority << std::endl
        << this->description << "'";

    for (const auto& tag : this->tags) {
        cmd << " --label '" << tag << "'";
    }

    return cmd.str();
}

std::string Task::get_title() const {
    return this->title;
}

std::vector<std::string> Task::get_tags() const {
    return this->tags;
}
