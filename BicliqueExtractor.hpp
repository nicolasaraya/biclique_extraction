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

    private:
        /*
            VARIABLES
        */
        bool adjMatrixLoaded;
        bool adjMatrixSorted; 
        uint16_t num_signatures;
        uint32_t biclique_size; 
        AdjencyMatrix* adjMatrix;
        Shingle* shingle; 
        string path; 
        /*
            METHODS
        */
        vector<uint64_t> splitString(string, string);
        
};

#endif