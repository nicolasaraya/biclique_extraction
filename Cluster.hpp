#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "Graph/Node.hpp"
#include "Trie.hpp"

using namespace std;

class Cluster {
    public:
        Cluster(vector<Node*> *);
        ~Cluster();
        void computeTrie();
        vector<Biclique*> *getBicliques();
        void printCluster();
        void printMap();
        
    private:
        vector<Node*>* nodes = nullptr;
        unordered_map<uInt, uint32_t> mapFrecuency; // Valor Nodo, Frecuencia
        unordered_map<string, uint32_t> mapFrecuencyWeighted; // Valor Nodo, Frecuencia
        uint16_t minFreq = 1;
        Trie* t = nullptr;
        bool weighted = false;
        
        bool sortSizeComp(Node*, Node*);
        void computeFrecuency();
        void computeHistogram();
};


#endif