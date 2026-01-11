#include <iostream>
#include <cstdlib>

#include "task-reader.hpp"

void create_issues(TaskReader& task_reader);
void retrieve_issues();

int main(void) {
    const std::string task_dir = "./.todo";

    TaskReader task_reader(task_dir);
    task_reader.read();
    create_issues(task_reader);
}

void create_issues(TaskReader& task_reader) {
    int task_len = task_reader.size();
    for (auto i = 0; i < task_len; i++) {
        auto curr_task = task_reader.retrieve_task(i);
        if (!curr_task.has_value()) {
            continue;
        }

        std::string task_cmd = curr_task.value().create_cmd();
        int ret = system(task_cmd.c_str());
        if (ret != 0) {
            std::cerr << "[error] issue creation failed" << std::endl;
        }
    }
}
