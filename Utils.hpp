#ifndef UTILS 
#define UTILS 

#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std; 

#define NUM_THREADS 8

typedef pair<uint64_t, vector<uint64_t>> Node;
typedef pair<Node*, vector<uint64_t>> SignNode; 
typedef pair<vector<uint64_t>, vector<uint64_t>> biclique;
/*
typedef struct{
    uint64_t ID; 
    bool autoCycle;
    vector<uint64_t> Ady; 
}Node;
*/

struct TrieNode{
    TrieNode *parent;
    uint64_t vertex;
    vector<uint64_t> indices;
    vector<TrieNode*> childrens;
};


#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::high_resolution_clock> a##label, b##label; \
	a##label = std::chrono::high_resolution_clock::now();


#define TIMERSTOP(label)                                                   \
    b##label = std::chrono::high_resolution_clock::now();                           \
    std::chrono::duration<double> delta##label = b##label-a##label;        \
    std::cout << "# elapsed time ("<< #label <<"): "                       \
                  << delta##label.count()  << "s" << std::endl;

#endif 