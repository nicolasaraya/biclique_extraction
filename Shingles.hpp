#ifndef SHINGLES_HPP
#define SHINGLES_HPP

#include <iostream>
#include <vector> 
#include <string>
#include <cstdint>
#include <functional>

#include "AdjencyMatrix.hpp"

using namespace std;

class Shingle{
    public:
        Shingle(unsigned char, unsigned int, AdjencyMatrix*);
        ~Shingle(); 
        void computeShingles();
    private:
        /*
            Variables
        */
            unsigned char num_signatures;
            unsigned int biclSize;
            unsigned int shingle_size = 1; 
            AdjencyMatrix* adjMatrix; 
        /*
            Methods
        */

};

#endif