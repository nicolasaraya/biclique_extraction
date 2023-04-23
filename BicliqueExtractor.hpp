#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <omp.h>
#include <math.h>

#include "AdjacencyMatrix.hpp"
#include "Cluster.hpp"
#include "Shingle.hpp"

using namespace std; 

class BicliqueExtractor{
    public:
        BicliqueExtractor(const string, uint16_t, uint16_t, uint16_t,  uint32_t, uint16_t);
        ~BicliqueExtractor();

        void extract();

    private:
        /*
            VARIABLES
        */
        bool withAutoCycle = false;
        bool adjacencyMatrixLoaded = false;
        uint16_t num_signatures;
        uint32_t biclique_size; 
        uint32_t minClusterSize;
        uint32_t minAdyNodes;
        uint16_t iteration;
        uint16_t bs_decrease;

        uint64_t total_biclique;
        uint64_t biclique_s_size;
        uint64_t biclique_c_size;
        uint64_t biclique_sxc_size;

        string path; 
        string name;

        AdjacencyMatrix* adjMatrix;
        vector<Cluster*> clusters;
        Shingle* shingle;
        vector<SignNode*> signatures;
        vector< vector< SignNode* >* > posClusters;
        /*
            METHODS
        */
        bool compareMinHash(const SignNode*, const SignNode*, int);
        bool compareBIndex(const Biclique*, const Biclique*);

        vector<vector<SignNode*>*> makeGroups(vector<SignNode*>*,int );
        vector<uint64_t> splitString(string, string);

        void makeAdjacencyMatrix();

        void computeClusters();
        void computeClusters2(vector<SignNode*>*,int);
        void computeTree();
        void computeShingles();
        void computeShinglesInline();

        uint32_t extractBicliques();

        void clearSignatures();
        void printSignatures();
        void printSignatures2(vector<SignNode*>);

        void sortSignatures(vector<SignNode*>*, int);
        void sortBicliques(vector<Biclique*>*);
        bool sortC(uint64_t*, uint64_t*);
        bool sortNodes(Node*,Node*);

        
};

#endif