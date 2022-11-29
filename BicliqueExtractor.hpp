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
        bool withAutoCycle = false;
        
        int numb_clusters = 0;
        uint16_t num_signatures;
        uint32_t biclique_size; 
        uint32_t minClusterSize = 10;
        uint32_t minAdyNodes = 3; // = num_signatures? 

        string path; 

        AdjencyMatrix* adjMatrix;
        vector<Cluster*> clusters;
        Shingle* shingle;

        vector<pair<vector<uint64_t>, vector<uint64_t>>> bicliques;
        vector<SignNode*> signatures;
        vector< vector< SignNode* >* > posClusters;
        /*
            METHODS
        */
        bool compareMinHash(const SignNode*, const SignNode*, int);

        vector<vector<SignNode*>*> makeGroups(vector<SignNode*>*,int );
        vector<uint64_t> splitString(string, string);

        void makeAdjencyMatrix();

        void computeClusters();
        int computeClusters2(vector<SignNode*>*,int);
        void computeTree();
        void computeShingles();
        void computeShinglesInline();
        void extractBicliques();

        void printSignatures();
        void printSignatures2(vector<SignNode*>);

        void sortSignatures(vector<SignNode*>*, int);
        
};

#endif