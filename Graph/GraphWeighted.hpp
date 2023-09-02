#ifndef ADJ_MATRIXW
#define ADJ_MATRIXW

#include "GraphADT.hpp"
#include "Node.hpp"

using namespace std;

typedef vector<Node*>::iterator GraphWeightedIterator;

class GraphWeighted : public GraphADT
{
public:
    GraphWeighted(const string);
    GraphWeighted();
    ~GraphWeighted();
    void buildBin();
    void buildTxt();
    void insert(Node*);
    uint64_t size();
    Node* back();
    uint64_t all_edges_size();
    void print();
    void printAsMatrix();
    void writeAdjacencyList(string);
    void restoreNodes();
    GraphWeightedIterator begin();
    GraphWeightedIterator end();
    Node* at(uInt);
    Node* find(uInt);
    string getPath();
    void sort();
    //void addBicliques(string);
    uint64_t maxValueEdge();
private:
    vector<Node*> matrix;
    Node* binarySearch(uInt, uInt, uInt);
    uint64_t maxEdge = 0;
    bool compareNodes(Node* a, Node* b);
};

#endif