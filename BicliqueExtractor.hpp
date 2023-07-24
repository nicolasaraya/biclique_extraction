#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <type_traits>

#include "Graph/Graph.hpp"
#include "Graph/GraphWeighted.hpp"
#include "Cluster.hpp"
#include "Shingle.hpp"

using namespace std;

typedef vector<SignNode*> Signatures;

template <typename GraphType> 
class BicliqueExtractor {
    public:
        //BicliqueExtractor(const string, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t, uint16_t);
        BicliqueExtractor(uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t, uint16_t);
        //path, num_signatures, minClusterSize, minAdyNodes, biclique_size, bs_decrease, shingleSize, selfLoop, threshold, iterations) 
        ~BicliqueExtractor();
        void extract();
        void setGraph(GraphType*); 

    private:
        uint16_t num_signatures;
        uint32_t minClusterSize;
        uint32_t biclique_size;
        uint32_t minAdyNodes;
        uint16_t bs_decrease;
        uint32_t shingleSize;
        uint32_t threshold;
        uint16_t iterations;
        mutex mtxWriteBiclique;
        mutex mtxSignatures;

        bool selfLoop;
        string path;
        uint16_t iteration;
        uint64_t total_biclique;
        uint64_t biclique_s_size;
        uint64_t biclique_c_size;
        uint64_t biclique_sxc_size;
        uint64_t cluster_size;
        uint64_t n_bicliques_iter;
        GraphType* graph = nullptr;
        bool weighted = false; 

        void extractBicliques(Cluster*);
        bool compareMinHash(const SignNode*, const SignNode*, int);
        bool compareBicliqueRank(const Biclique*, const Biclique*);

        vector<Signatures*>* makeGroups(Signatures*, int);
        //void computeClusters(vector<Signatures*> *, unsigned int);
        void computeClusters(Signatures*, unsigned int);
        Signatures* computeShingles();
        void getBicliques(Cluster*);
        void printSignatures(Signatures*);
        void sortBySignatures(Signatures*, int);
        void sortBicliques(vector<Biclique*> *);
        bool sortC(uint64_t, uint64_t);
        bool sortC_w(pair<uInt, uInt>, pair<uInt,uInt>);
        bool sortS(Node*, Node*);

};

template class BicliqueExtractor<Graph>;
template class BicliqueExtractor<GraphWeighted>;


#endif
