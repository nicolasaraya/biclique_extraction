#include "TaskManager.hpp"

using namespace std; 

//TaskManager* TaskManager::instance = nullptr;

TaskManager::TaskManager()
{
    cout << "Create Singleton Task Manager" << endl;
}

TaskManager::~TaskManager() 
{
    //deleteWorkers();
    std::unique_lock<std::mutex> lock(freeMutex);
    cv.wait(lock, [this]{ return ready; });
    cout << "delete TaskMgr" << endl;
    
    
}

TaskManager& TaskManager::get()
{
    static TaskManager instance2; 
    return instance2; 
   
   /* if (not instance) {
        instance = new TaskManager;
    }
    return *instance; 
    */
}


void TaskManager::submit(RoutineType routine)
{
    submit(new SimpleTask(routine));
}

void TaskManager::submit(Task* task)
{
    unique_lock<mutex> lck(mtxQueue);
    cout << "Push Task" << endl;
    tasks.push(task);
    if(tasks.size() > currentWorkers){
        createWorker();
    }

}

bool TaskManager::empty()
{
    unique_lock<mutex> lck(mtxQueue);
    return tasks.empty();
}

Task* TaskManager::getTask()
{
    unique_lock<mutex> lck(mtxQueue);
    if (tasks.empty()) {
        return nullptr; 
    } else {
        auto t = tasks.front();
        tasks.pop();
        return t; 
    }
}

void TaskManager::createWorker()
{
    unique_lock<mutex> lck(mtxWorkers);
    if(currentWorkers < num_threads){
        Worker* w = new Worker(this);
        workers.push_back(w);
        currentWorkers++; 
    }
}

void TaskManager::deleteWorker(Worker* worker)
{
    unique_lock<mutex> lck(mtxWorkers);
    for (auto it = workers.begin(); it != workers.end(); it++) {
        if (*it == worker) {
            workers.erase(it); 
            delete(*it);
            currentWorkers--;
            break;
        }
    }
    if (workers.empty()) {
        ready = true;
        cv.notify_one();
    }
    return;
}

