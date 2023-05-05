#ifndef ADJ_MATRIX
#define ADJ_MATRIX

#include "Node.hpp"

using namespace std;

typedef vector<Node *>::iterator AdjMatrixIterator;

class AdjacencyMatrix
{
public:
    // PUBLIC METHODS
    AdjacencyMatrix(const string, bool);
    AdjacencyMatrix();
    ~AdjacencyMatrix();

    void addBicliques(string);
    void build();
    void insert(Node *);
    uint64_t size();
    uint64_t all_edges_size();
    void print();
    void writeAdjacencyList();
    void restoreNodes();
    AdjMatrixIterator begin();
    AdjMatrixIterator end();
    AdjMatrixIterator find(Node *);
    Node *find_value(uint64_t);
    // PUBLIC VARIABLES

private:
    // PRIVATE METHODS
    vector<Node *> matrix;
    string path;
    bool selfLoops = false;
    uint64_t last_node;

    Node *find_value_recursive(uint64_t, uint64_t, uint64_t);
    bool compareNodes(Node *, Node *);
};

#endif