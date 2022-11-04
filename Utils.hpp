#ifndef UTILS 
#define UTILS 

#include <vector>
#include <cstdint>

using namespace std; 

typedef pair<uint64_t, vector<uint64_t>> Node;

/*
typedef struct{
    uint64_t ID; 
    bool autoCycle;
    vector<uint64_t> Ady; 
}Node;
*/
typedef pair<Node*, vector<uint64_t>> SignNode; 

#endif 