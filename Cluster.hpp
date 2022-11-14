#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "Utils.hpp"

using namespace std;


class Cluster{
    public:
        Cluster(vector<Node*>*);
        ~Cluster();

        void computeHistogram();
        void printCluster();
        void computeFrecuency(); 

    private:
        vector<Node*>* nodes;
        unordered_map<uint64_t, uint32_t> mapFrecuency; //Valor Nodo, Frecuencia
        bool sortFrecuencyComp(const uint64_t&, const uint64_t&);
        bool sortSizeComp(const Node*, const Node*);
};


#endif