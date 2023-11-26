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
    void buildBin_alt();
    void buildTxt();
    void insert(Node*);
    uint64_t size();
    Node* back();
    uint64_t all_edges_size();
    void print();
    void printAsMatrix();
    void writeAdjacencyList();
    void writeBinaryFile();
    void writeBinaryFile_alt(); 
    void restoreNodes();
    double getAverageDegree();
    GraphWeightedIterator begin();
    GraphWeightedIterator end();
    Node* at(uInt);
    Node* find(uInt);
    string getPath();
    void setPath(string); 
    void sort();
    //void addBicliques(string);
    uint64_t maxValueEdge();
    void transpose();
    bool isTransposed();
private:
    vector<Node*>* matrix = nullptr;
    Node* binarySearch(uInt, uInt, uInt);
    uint64_t maxEdge = 0;
    bool transposed = false;
    bool compareNodes(Node* a, Node* b);
};

#endif