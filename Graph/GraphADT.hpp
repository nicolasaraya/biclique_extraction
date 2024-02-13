#ifndef GRAPHADT
#define GRAPHADT

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <cstdint>

using namespace std;

class GraphADT
{
public:
    GraphADT(string path): path(path) {};
    GraphADT() {};
    virtual ~GraphADT() {};
    virtual void buildTxt() = 0;
    virtual void buildBin() = 0;
    virtual uint64_t size() = 0;
    virtual uint64_t all_edges_size() = 0;
    virtual void print() = 0;
    virtual void writeAdjacencyList() = 0;
    virtual void restoreNodes() = 0;
    virtual string getPath() = 0;
    virtual void writeBinaryFile() = 0;
    void setCompressed(bool b) { compressed = b; }
protected:
    string path = "graph.txt";
    string format;
    bool selfLoops = false;
    bool weighted = false;
    uint64_t num_nodes = 0;
    bool compressed = false;
    uint64_t num_edges = 0;
};

#endif