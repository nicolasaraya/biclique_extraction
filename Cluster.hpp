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


class Cluster{
    public:
        Cluster(vector<Node*>*);
        ~Cluster();

        Trie *t;

        void computeTrie();
        vector<Biclique*> getBicliques();
        void printCluster();

    private:
        vector<Node*>* nodes;

        unordered_map<uint64_t, uint32_t> mapFrecuency; //Valor Nodo, Frecuencia
        uint16_t minFreq = 1;

        bool sortFrecuencyComp(const uint64_t&, const uint64_t&);
        bool sortSizeComp(Node*, Node*);

        void computeFrecuency();
        void computeHistogram();
        //void printCluster();
};


#endif