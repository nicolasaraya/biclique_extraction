#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std; 

class Shingle{
    public:
        typedef pair<uint64_t,vector<uint64_t>> sign_node; 

        Shingle(uint16_t num_signatures);
        ~Shingle();
        vector<uint64_t> computeShingle(uint64_t, vector<uint64_t>);

    private:
        uint16_t num_signatures;
        uint32_t shingle_size;
        uint64_t prime;

        std::hash<string> hash_nodes;

        vector<uint64_t> A;
        vector<uint64_t> B;
        vector<sign_node> sign_node_vec;
};

#endif