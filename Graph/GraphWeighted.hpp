#ifndef ADJ_MATRIXW
#define ADJ_MATRIXW

#include "GraphADT.hpp"
#include "NodeWeighted.hpp"

using namespace std;

typedef vector<NodeWeighted*>::iterator GraphWeightedIterator;

class GraphWeighted : public GraphADT
{
public:
    GraphWeighted(const string, bool);
    GraphWeighted(const string);
    GraphWeighted();
    ~GraphWeighted();

    //void addBicliques(string);
    void addBicliques(string);
    void standardize(vector<uInt>*);
    void buildBin();
    void buildTxt();
    void insert(NodeWeighted *);
    uint64_t size();
    NodeWeighted* back();
    uint64_t all_edges_size();
    void print();
    void writeAdjacencyList(string);
    void restoreNodes();
    GraphWeightedIterator begin();
    GraphWeightedIterator end();
    NodeWeighted* at(uInt);
    NodeWeighted* find(uInt);
    string getPath();
    // PUBLIC VARIABLES

private:
    // PRIVATE METHODS
    vector<NodeWeighted*> matrix;
    NodeWeighted* binarySearch(uInt, uInt, uInt);
};

#endif