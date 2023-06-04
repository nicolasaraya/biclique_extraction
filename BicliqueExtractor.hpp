#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include "AdjacencyMatrix.hpp"
#include "Cluster.hpp"
#include "Shingle.hpp"
#include "Define.hpp"

using namespace std;

class BicliqueExtractor
{
public:
    // PUBLIC METHODS
    BicliqueExtractor(const string, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t, uint16_t);
    ~BicliqueExtractor();

    void extract();

private:
    // PRIVATE VARIABLES
    // PARAMETERS
    uint16_t num_signatures;
    uint32_t minClusterSize;
    uint32_t biclique_size;
    uint32_t minAdyNodes;
    uint16_t bs_decrease;
    uint32_t shingleSize;
    uint32_t threshold;
    uint16_t iterations;

    bool selfLoop;
    string path;
    uint16_t iteration;
    uint64_t total_biclique;
    uint64_t biclique_s_size;
    uint64_t biclique_c_size;
    uint64_t biclique_sxc_size;
    uint64_t cluster_size;
    uint64_t n_bicliques_iter;
    AdjacencyMatrix *adjMatrix;

    // PRIVATE METHODS
    void extractBicliques(Cluster *);
    bool compareMinHash(const SignNode *, const SignNode *, int);
    bool compareBicliqueRank(const Biclique *, const Biclique *);
    vector<Signatures *> makeGroups(Signatures *, int);
    // void computeClusters(vector<Signatures *> *, unsigned int);
    void computeClusters(Signatures *, unsigned int);
    Signatures *computeShingles();
    void getBicliques(Cluster *);
    void printSignatures(Signatures *);
    void sortSignatures(Signatures *, int);
    void sortBicliques(vector<Biclique *> *);
    bool sortC(uint64_t, uint64_t);
    bool sortS(Node *, Node *);
};

#endif