#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <iostream>
#include <string>
#include <algorithm>

#include "Cluster.hpp"
#include "Shingle.hpp"
#include "AdjencyMatrix.hpp"

using namespace std; 

class BicliqueExtractor{
    public:
        BicliqueExtractor(const string, uint16_t, uint32_t);
        ~BicliqueExtractor();
        void makeAdjencyMatrix();
        void computeShingles();
        void computeShinglesInline();
        AdjencyMatrix* getAdjencyMatrix();
        void printSignatures();
        void computeClusters();
        void saveCluster();

    private:
        /*
            VARIABLES
        */
        bool adjMatrixLoaded;
        bool adjMatrixSorted; 
        bool sortedHashes;
        int numb_clusters;
        uint16_t num_signatures;
        uint32_t biclique_size; 
        uint32_t minClusterSize = 1;
        string path; 
        AdjencyMatrix* adjMatrix;
        vector<SignNode*> signatures;
        vector< vector<SignNode*>* > Clusters; 
        //vector<Cluster*> clusters;
        Shingle* shingle;
        /*
            METHODS
        */
        vector<uint64_t> splitString(string, string);
        void sortSignatures(vector<SignNode*>*, int);
        bool compareMinHash(const SignNode*, const SignNode*, int);

        //nuevos metodos
        void computeClusters2(vector<SignNode*>,int);
        vector<uint64_t> make_positions_Groups(vector<SignNode*> ,int);
        vector<SignNode*> makeGroups(vector<SignNode*>, uint64_t , uint64_t );
        void printSignatures2(vector<SignNode*>);
	
        
};

#endif