#ifndef GRAPHADT
#define GRAPHADT

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <cstdint>

using namespace std;

typedef int BinVar;
//typedef long long int BinVar; 

class GraphADT
{
public:
    GraphADT(string path, bool selfLoops): path(path), selfLoops(selfLoops){};
    GraphADT(){};
    // virtual void addBicliques(string) = 0;
    virtual void buildTxt() = 0;
    virtual void buildBin() = 0;
    virtual uint64_t size() = 0;
    virtual uint64_t all_edges_size() = 0;
    virtual void print() = 0;
    virtual void writeAdjacencyList(string) = 0;
    virtual void restoreNodes() = 0;
    virtual string getPath() = 0;
protected:
    string path;
    string format;
    bool selfLoops = false;
    bool weighted = false;
    uint64_t num_nodes = 0;
};

#endif