#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "Graph/Node.hpp"
#include "Graph/NodeWeighted.hpp"

#include "Trie.hpp"

using namespace std;




template <typename NodeType> class Cluster
{

public:
    Cluster(vector<NodeType *> *);
    ~Cluster();
    void computeTrie();
    vector<Biclique<NodeType>*> *getBicliques();
    void printCluster();
    void printMap();
    Trie<NodeType> *t;
private:
    vector<NodeType*> *nodes;
    unordered_map<uInt, uint32_t> mapFrecuency; // Valor Nodo, Frecuencia
    unordered_map<string, uint32_t> mapFrecuencyWeighted; // Valor Nodo, Frecuencia
    uint16_t minFreq = 1;
    bool sortSizeComp(NodeType *, NodeType *);
    void computeFrecuency();
    void computeHistogram();


};

template class Cluster<Node>;
template class Cluster<NodeWeighted>;

#endif