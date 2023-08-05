#include "Task.hpp"

Task::~Task() {}

SimpleTask::SimpleTask(RoutineType routine) : routine(routine) {}

SimpleTask::~SimpleTask() {
    std::cout << "delete task" << std::endl;
}

void SimpleTask::run() 
{
    if (routine) {
        routine();
    }
}