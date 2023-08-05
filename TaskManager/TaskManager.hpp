#ifndef TASKMGR_HPP
#define TASKMGR_HPP

#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <condition_variable>

#include "Task.hpp"
#include "Worker.hpp"

class Worker;

class TaskManager {
    public:
        typedef std::function<void()> RoutineType;
    public:
        static TaskManager& get();
        void submit(Task*);
        void submit(RoutineType);

        TaskManager(const TaskManager& other) = delete;
        TaskManager& operator=(const TaskManager& other) = delete; 
        friend class Worker;

    private:
        TaskManager();
        ~TaskManager();
        void checkWorkers();
        void createWorker();
        void deleteWorker(Worker*);
        bool empty();
        Task* getTask();
        static TaskManager* instance;

        std::queue<Task*> tasks;
        std::vector<Worker*> workers;
        std::mutex mtxQueue; 
        std::mutex mtxWorkers; 
        std::thread mainThread; 
        std::mutex freeMutex;
        bool ready = false;
        std::condition_variable cv; 
        unsigned num_threads = 4; 
        unsigned currentWorkers = 0; 


};

#endif