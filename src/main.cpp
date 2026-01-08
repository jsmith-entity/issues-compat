#include <iostream>

#include "task-reader.hpp"

int main(void) {
    const std::string task_dir = "./.todo";

    TaskReader task_reader(task_dir);
    task_reader.read();
    task_reader.print();
}
