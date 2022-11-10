#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Utils.hpp"

using namespace std; 

class Shingle{
    public:
        Shingle(uint16_t num_signatures);
        ~Shingle();
        SignNode* computeShingle(Node*);
    private:
        uint16_t num_signatures;
        uint32_t shingle_size;
        uint64_t prime;
        std::hash<string> hash_nodes;
        vector<uint64_t> A;
        vector<uint64_t> B;
};

#endif