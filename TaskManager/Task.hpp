#ifndef TASK_HPP
#define TASK_HPP

#include <functional>
#include <iostream>

class Task {
    public:
        Task() {}
        virtual void run() = 0; 
        virtual ~Task() = 0;
};

class SimpleTask : public Task {
    public:
        typedef std::function<void()> RoutineType;
    public:
        SimpleTask(RoutineType);
        virtual ~SimpleTask();
        virtual void run();
    private:
        RoutineType routine; 
};

#endif