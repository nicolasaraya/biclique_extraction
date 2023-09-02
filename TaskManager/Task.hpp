#ifndef TASK_HPP
#define TASK_HPP

#include <functional>
#include <iostream>

#include "../Utils/Utils.hpp"

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

/*
class NodeTask : public Task {
    public:
        typedef std::function<void(unordered_map<uInt, uint32_t>*)> RoutineSortByFrecuency;
        typedef std::function<void(unordered_map<string, uint32_t>*)> RoutineSortByFrecuencyWeighted;
        typedef std::function<void(unordered_map<uInt, uint32_t> *, uint16_t)> RoutineMoveToCache;
        typedef std::function<void(unordered_map<string, uint32_t> *, uint16_t)> RoutineMoveToCacheWeighted;
    public:
        NodeTask(RoutineSortByFrecuency);
        NodeTask(RoutineSortByFrecuencyWeighted);
        NodeTask(RoutineMoveToCache);
        NodeTask(RoutineMoveToCacheWeighted);
        virtual ~NodeTask();
        virtual void run();
    private:
        RoutineMoveToCache rmtc;
        RoutineMoveToCacheWeighted rmtcw;
        RoutineSortByFrecuency rsbf;
        RoutineSortByFrecuencyWeighted rsbfw; 
};
*/
#endif