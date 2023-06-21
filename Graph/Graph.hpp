#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "GraphADT.hpp"
#include "Node.hpp"

using namespace std;

typedef vector<Node *>::iterator AdjMatrixIterator;
class Graph : public GraphADT
{
public:
    Graph(const string, bool);
    Graph();
    ~Graph();

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
    Node* find(uInt);
    string getPath();

private:
    // PRIVATE METHODS
    vector<Node*> matrix;
    Node* binarySearch(uInt, uInt, uInt);
};

#endif