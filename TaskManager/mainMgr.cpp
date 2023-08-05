#include <iostream>

#include "Task.hpp"
#include "Worker.hpp"
#include "TaskManager.hpp"

using namespace std; 

void example() 
{
    cout << "hola" << endl;
}


int main(int argc, char const *argv[])
{
    //s->run();

    TaskManager::get().submit(example);
    //delete s;
    //sleep(2);
    return 0;
}



// g++ mainMgr.cpp Task.cpp Worker.cpp TaskManager.cpp Task.hpp Worker.hpp TaskManager.hpp
