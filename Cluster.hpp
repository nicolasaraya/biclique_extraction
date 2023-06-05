#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "Trie.hpp"
#include "Define.hpp"
#include "Node.hpp"

using namespace std;

class Cluster
{
public:
    // PUBLIC METHODS
    Cluster(vector<Node *> *);
    ~Cluster();

    void computeTrie();
    vector<Biclique *> *getBicliques();
    void printCluster();
    void printMap();

    // PUBLIC VARIABLES
    Trie *t;

private:
    // PRIVATE VARIABLES
    vector<Node *> *nodes;

    unordered_map<uint64_t, uint32_t> mapFrecuency; // Valor Nodo, Frecuencia
    uint16_t minFreq = 1;

    // PRIVATE METHODS
    bool sortSizeComp(Node *, Node *);
    void computeFrecuency();
    void computeHistogram();
};

#endif