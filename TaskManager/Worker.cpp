#include "Worker.hpp"

using namespace std;

Worker::Worker() {}

Worker::Worker(TaskManager* manager) : manager(manager) {
    cout << "Worker created" << endl;
    workThread = std::thread(&Worker::work, this);
    workThread.detach();
}

Worker::~Worker() 
{
    cout << "delete worker" << endl;
}


bool Worker::state()
{
    return working; 
}

void Worker::work()
{    
    while (not manager->empty()) {
        auto task = manager->getTask();
        if (task) {
            cout << "Running Task" << endl;
            task->run();
            delete task;
        }
    }
    manager->deleteWorker(this);
}