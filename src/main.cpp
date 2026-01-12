#include <iostream>
#include <cstdlib>

#include "task-manager.hpp"

int main(void) {
    const std::string task_dir = "./.todo";

    TaskManager task_manager(task_dir);
    task_manager.read();
    task_manager.create_issues();
}
