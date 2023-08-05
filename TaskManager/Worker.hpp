#ifndef WORKER_HPP
#define WORKER_HPP

#include <queue>
#include <iostream>

#include "Task.hpp"
#include "TaskManager.hpp"

class TaskManager; 

class Worker {
    public:
        Worker();
        Worker(TaskManager*);
        ~Worker();
        bool state();
        void work();
        friend class TaskManager;
    private:
        bool working = false;
        std::thread workThread; 
        TaskManager* manager = nullptr; 
};  

#endif