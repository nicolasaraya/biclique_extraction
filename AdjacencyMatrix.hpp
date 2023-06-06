#ifndef ADJ_MATRIX
#define ADJ_MATRIX
#include "Node.hpp"

using namespace std;

typedef vector<Node *>::iterator AdjMatrixIterator;
typedef int BinVar;
//typedef long long int BinVar; 

class AdjacencyMatrix
{
public:
    // PUBLIC METHODS
    AdjacencyMatrix(const string, bool);
    AdjacencyMatrix();
    ~AdjacencyMatrix();

    void addBicliques(string);
    void standardize(vector<uInt>*);
    void buildTxt();
    void buildBin();
    void insert(Node *);
    uint64_t size();
    Node* back();
    uint64_t all_edges_size();
    void print();
    void writeAdjacencyList(string);
    void restoreNodes();
    AdjMatrixIterator begin();
    AdjMatrixIterator end();
    Node* at(uInt);
    // PUBLIC VARIABLES

private:
    // PRIVATE METHODS
    vector<Node*> matrix;
    string path;
    bool selfLoops = false;
    uint64_t num_nodes;
    string format;
};

#endif