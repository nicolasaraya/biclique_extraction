#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <omp.h>

#include "AdjencyMatrix.hpp"
#include "Cluster.hpp"
#include "Shingle.hpp"

using namespace std; 

class BicliqueExtractor{
    public:
        BicliqueExtractor(const string, uint16_t, uint32_t);
        ~BicliqueExtractor();

        void extract();

    private:
        /*
            VARIABLES
        */
        bool adjMatrixLoaded;
        bool adjMatrixSorted; 
        bool sortedHashes;
        bool withAutoCycle = false;
        
        int numb_clusters = 0;
        uint16_t num_signatures;
        uint32_t biclique_size; 
        uint32_t minClusterSize = 10;

        string path; 

        AdjencyMatrix* adjMatrix;
        vector<Cluster*> clusters;
        Shingle* shingle;
        vector<SignNode*> signatures;
        vector< vector< SignNode* >* > posClusters;
        /*
            METHODS
        */
        AdjencyMatrix* getAdjencyMatrix();

        bool compareMinHash(const SignNode*, const SignNode*, int);

        vector<vector<SignNode*>*> makeGroups(vector<SignNode*>*,int );
        vector<uint64_t> splitString(string, string);

        void makeAdjencyMatrix();

        void computeClusters();
        void computeClusters2(vector<SignNode*>*,int);
        void computeHistograms();
        void computeShingles();
        void computeShinglesInline();

        void printSignatures();
        void printSignatures2(vector<SignNode*>);

        //void saveCluster();
        void sortSignatures(vector<SignNode*>*, int);
        
};

#endif