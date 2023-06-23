#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <type_traits>

#include "Graph/Graph.hpp"
#include "Graph/GraphWeighted.hpp"
#include "Cluster.hpp"
#include "Shingle.hpp"

using namespace std;

template <typename GraphType, typename NodeType> 
class BicliqueExtractor
{
public:
    // PUBLIC METHODS
    //BicliqueExtractor(const string, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t, uint16_t);
    BicliqueExtractor(uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t, uint16_t);
    //path, num_signatures, minClusterSize, minAdyNodes, biclique_size, bs_decrease, shingleSize, selfLoop, threshold, iterations) 
    ~BicliqueExtractor();
    void extract();
    void setGraph(GraphType*); 

private:
    typedef Shingle<NodeType>::Signatures SignaturesType;
    typedef Shingle<NodeType>::SignatureIterator SignIterator;
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
    GraphType *graph = nullptr;
    bool weighted = false; 

    void extractBicliques(Cluster<NodeType> *);
    bool compareMinHash(const SignNode<NodeType> *, const SignNode<NodeType> *, int);
    bool compareBicliqueRank(const Biclique<NodeType> *, const Biclique<NodeType> *);

    vector<typename Shingle<NodeType>::Signatures*> makeGroups(SignaturesType *, int);
    void writeBiclique(vector<NodeType *> *, vector<uInt> *);
    void computeClusters(vector<SignaturesType *> *, unsigned int);
    void computeClusters(SignaturesType *, unsigned int);
    typename Shingle<NodeType>::Signatures* computeShingles();
    void getBicliques(Cluster<NodeType> *);
    void printSignatures(SignaturesType *);
    void sortSignatures(SignaturesType *, int);
    void sortBicliques(vector<Biclique<NodeType> *> *);
    bool sortC(uint64_t, uint64_t);
    bool sortS(NodeType *, NodeType *);

    #if defined(parallel)
    vector<Signatures *> makeGroups(Signatures *, uInt, uInt, int);
    void parallelExtraction(Signatures*, uInt, uInt);
    void shingleParallel(Shingle*, Signatures*, uInt, uInt);
    #endif
};

template class BicliqueExtractor<GraphWeighted, NodeWeighted>;
template class BicliqueExtractor<Graph, Node>;

#endif