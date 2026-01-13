#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <algorithm>

#include "task.hpp"
#include "utils.hpp"

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
        tags.push_back(trim(tag));
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

std::string Task::get_title() const {
    return this->title;
}

std::string Task::get_status() const {
    return this->status;
}

std::vector<std::string> Task::get_tags() const {
    return this->tags;
}

void Task::set_issue_num(std::string issue_num) {
    int num = std::stoi(issue_num);
    this->issue_num = num;
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

std::string Task::find_cmd(bool closed) const {
    std::ostringstream cmd;
    cmd << "gh issue list ";
    if (closed) {
        cmd << " --state closed";
    }
    cmd << " | grep '" << this->title << "'";
    return cmd.str();
}

std::string Task::close_cmd() const {
    std::ostringstream cmd;
    cmd << "gh issue close " << this->issue_num;
    return cmd.str();
}

std::string Task::view_cmd() const {
    std::ostringstream cmd;
    cmd << "gh issue view " << this->issue_num;
    return cmd.str();
}

std::optional<std::string> Task::update_cmd(const std::string& issue_details) const {
    std::ostringstream default_cmd;
    default_cmd << "gh issue edit " << this->issue_num << " ";

    std::ostringstream cmd;
    cmd << default_cmd.str();

    const std::string TITLE = "title: ";
    const std::string PRIORITY = "PRIORITY: ";
    const std::string TAGS = "labels:";

    std::istringstream ss(issue_details);
    std::string desc;
    std::string line;
    bool desc_reached = false;
    while (std::getline(ss, line)) {
        if (line.find(TITLE) == 0) {
            auto gh_title = line.substr(TITLE.size());
            if (this->title != gh_title) {
                cmd << "--title '" << this->title << "' ";
            }
        } else if (line.find(PRIORITY) == 0) {
            auto gh_prio = line.substr(PRIORITY.size());
            if (this->priority != gh_prio) {
                desc = desc + this->priority + "\n";
            }
            desc_reached = true;
        } else if (line.find(TAGS) == 0) {
            std::string gh_labels = line.substr(TAGS.size());
            auto gh_labels_vec = split(gh_labels, ',');
            for (const auto& tag : this->tags) {
                if (!vec_has_value(gh_labels_vec, tag)) {
                    cmd << "--add-label '" << tag << "' ";
                }
            }
        }

        if (desc_reached) {
            desc = desc + line + "\n";
        }
    }

    const std::string full_desc = "PRIORITY: " + this->priority + "\n" + this->description + "\n";
    if (desc.size() != 0 && full_desc != desc) {
        cmd << "--body '" << this->description << "' ";
    }

    if (cmd.str() != default_cmd.str()) {
        return cmd.str();
    } else {
        return {};
    }
}
