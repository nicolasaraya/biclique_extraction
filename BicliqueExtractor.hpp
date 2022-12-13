#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <omp.h>
#include <math.h>

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
        bool adjencyMatrixLoaded = false;
        int numb_clusters = 0;
        uint16_t num_signatures;
        uint32_t biclique_size; 
        uint32_t minClusterSize = 50;
        uint32_t minAdyNodes = 3; // = num_signatures? 
        uint16_t iteration = 1;
        uint16_t decrem = 10;

        string path; 
        string name;

        AdjencyMatrix* adjMatrix;
        vector<Cluster*> clusters;
        Shingle* shingle;
        vector<SignNode*> signatures;
        vector< vector< SignNode* >* > posClusters;
        /*
            METHODS
        */
        bool compareMinHash(const SignNode*, const SignNode*, int);

        vector<vector<SignNode*>*> makeGroups(vector<SignNode*>*,int );
        vector<uint64_t> splitString(string, string);

        void makeAdjencyMatrix();

        uint32_t computeClusters();
        int computeClusters2(vector<SignNode*>*,int);
        void computeTree();
        void computeShingles();
        void computeShinglesInline();
        void extractBicliques();
        void clearSignatures();
        void printSignatures();
        void printSignatures2(vector<SignNode*>);

        void sortSignatures(vector<SignNode*>*, int);
        bool sortC(uint64_t*, uint64_t*);
        bool sortNodes(Node*,Node*);
        
};

#endif