#ifndef UTILS 
#define UTILS 

#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std; 

#define NUM_THREADS 8


struct Node{
    uint64_t nodeID; 
    bool autoCycle = false;
    vector<uint64_t> adyNodes;
    vector<uint64_t> cacheAdyNodes;  
};

struct SignNode{
    Node* ptrNode;
    vector<uint64_t> minHash; 
};


//typedef pair<Node*, vector<uint64_t>> SignNode; 
//typedef pair<uint64_t, vector<uint64_t>> Node;
typedef pair<vector<Node*>*, vector<uint64_t>> Biclique;

struct TrieNode{
    TrieNode *parent;
    //Node* ptrNode;
    uint64_t vertex;
    //vector<uint64_t> indices;
    vector<Node*>* indices;
    vector<TrieNode*>* childrens;
    uint32_t depth;
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