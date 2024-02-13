#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <type_traits>

#include "Graph/Graph.hpp"
#include "Graph/GraphWeighted.hpp"
#include "Cluster.hpp"
#include "Shingle.hpp"

using namespace std;

typedef vector<SignNode*> Signatures;
//typedef vector<pair<uInt, uInt>> C_values;

/*
struct CompactBicliqueWeighted {
    set<uInt> weights_values;
    // set<uInt> S_values; 
    vector<C_values> c_bicliques; 
    vector<pair<uInt, vector<uInt>>> linked_s; //S_value to C_values index   
};*/


/*
 * 2 : (1,e) - (3,a) - (6,b) - (7,c) - (8,f) - (10,d)
 * 3 : (3,a) - (6,b) - (7,c) - (10,d) 
 * 4 : (1,e) - (8,f)
 * 5 : (3,a) - (6,b) - (7,c) - (10, d) 
 * 
 *  weights_values : [a,b,c,d,e,f]
 *  c_bicliques[0] : [(1,3), (2,6), (3,7), (4,10)]  => (3,a), (6,b), (7,c), (10,d)
 *  c_bicliques[1] : [(5,1), (6,8)]  => (1,e), (8,f)
 * 
 *  linkedS[0]: <2, [0, 1]>
 *  linkedS[1]: <3, [0]>
 *  linkedS[2]: <4, [1]>
 *  linkedS[3]: <5, [0]>
*/


template <typename GraphType> 
class BicliqueExtractor {
    public:
        //BicliqueExtractor(const string, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, bool, uint32_t, uint16_t);
        BicliqueExtractor(uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint32_t, uint32_t, uint16_t);
        //path, num_signatures, minClusterSize, minAdyNodes, biclique_size, bs_decrease, shingleSize, threshold, iterations) 
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

        string path;
        uint16_t iteration;
        uint64_t total_biclique;
        uint64_t biclique_s_size;
        uint64_t biclique_c_size;
        uint64_t biclique_sxc_size;
        uint16_t min_sxc_size = 20;
        uint64_t cluster_size;
        uint64_t n_bicliques_iter;
        GraphType* graph = nullptr;
        
        string pathBicliques;
        string pathLog;
        
        //bool saveCompressed = false; 
        //CompactBicliqueWeighted* compBicl = nullptr; 

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
        void writeBicliques(vector<Biclique*>*);
        //void writeBicliques_alt(vector<Biclique*>*); 
        //void writeCompactStructure();
        //void writeCompactStructureBin();


};

template class BicliqueExtractor<Graph>;
template class BicliqueExtractor<GraphWeighted>;


#endif
