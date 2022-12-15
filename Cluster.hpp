#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "Trie.hpp"
#include "Utils.hpp"

using namespace std;


class Cluster{
    public:
        Trie *t;
        vector<Node*>* nodes;
        Cluster(vector<Node*>*);
        ~Cluster();

        void computeTrie();
        Biclique* getBiclique();
        void printCluster();

    private:
        //vector<Node*>* nodes;

        unordered_map<uint64_t, uint32_t> mapFrecuency; //Valor Nodo, Frecuencia

        bool sortFrecuencyComp(const uint64_t&, const uint64_t&);
        bool sortSizeComp(const Node*, const Node*);

        void computeFrecuency();
        void computeHistogram();
        //void printCluster();
};


#endif