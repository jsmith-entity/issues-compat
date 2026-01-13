#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <filesystem>
#include <optional>

class Task {
public:
    Task(const std::filesystem::path& file_path);
    void print() const;
    std::string get_title() const;
    std::string get_status() const;
    std::vector<std::string> get_tags() const;

    void set_issue_num(std::string issue_num);

    std::string create_issue_cmd() const;
    std::string find_cmd(bool closed) const;
    std::string close_cmd() const;
    std::string view_cmd() const;
    std::optional<std::string> update_cmd(const std::string& issue_details) const;
private:
    int issue_num;
    std::string title;
    std::string status;
    std::string priority;
    std::vector<std::string> tags;
    std::string description;

    std::vector<std::string> extract_tags(const std::string& line);
};

#endif
