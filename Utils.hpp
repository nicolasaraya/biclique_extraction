#ifndef UTILS 
#define UTILS 

#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std; 

#define NUM_THREADS 1

typedef pair<uint64_t, vector<uint64_t>> Node;
typedef pair<Node*, vector<uint64_t>> SignNode; 
/*
typedef struct{
    uint64_t ID; 
    bool autoCycle;
    vector<uint64_t> Ady; 
}Node;
*/

#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::high_resolution_clock> a##label, b##label; \
	a##label = std::chrono::high_resolution_clock::now();


#define TIMERSTOP(label)                                                   \
    b##label = std::chrono::high_resolution_clock::now();                           \
    std::chrono::duration<double> delta##label = b##label-a##label;        \
    std::cout << "# elapsed time ("<< #label <<"): "                       \
                  << delta##label.count()  << "s" << std::endl;

#endif 