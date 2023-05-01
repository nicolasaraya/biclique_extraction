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
    BicliqueExtractor(const string, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t);
    ~BicliqueExtractor();

    void extract();

private:
    // PRIVATE VARIABLES
    bool selfLoop = true;
    bool adjacencyMatrixLoaded = false;
    string path;
    uint16_t num_signatures;
    uint32_t minClusterSize;
    uint32_t biclique_size;
    uint32_t minAdyNodes;
    uint16_t bs_decrease;
    uint32_t shingleSize;
    uint32_t threshold; 
    uint16_t iteration = 1;
    uint64_t total_biclique = 0;
    uint64_t biclique_s_size = 0;
    uint64_t biclique_c_size = 0;
    uint64_t biclique_sxc_size = 0;
    AdjacencyMatrix *adjMatrix;
    vector<Cluster *> clusters;

    // PRIVATE METHODS
    bool compareMinHash(const SignNode *, const SignNode *, int);
    bool compareBicliqueRank(const Biclique *, const Biclique *);
    vector<Signatures *> makeGroups(Signatures *, int);
    // void computeClusters(vector<Signatures *> *, unsigned int);
    void computeClusters(Signatures *, unsigned int);
    void computeTree();
    Signatures *computeShingles();
    void computeShinglesInline();
    uint32_t extractBicliques();
    void printSignatures(Signatures *);
    void sortSignatures(Signatures *, int);
    void sortBicliques(vector<Biclique *> *);
    bool sortC(uint64_t, uint64_t);
    bool sortS(Node *, Node *);
};

#endif