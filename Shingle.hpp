#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include "Define.hpp"

using namespace std; 

class Shingle{
    public:
        Shingle(uint16_t, uint32_t, uint32_t);
        ~Shingle();
        SignNode* computeShingle(Node*);
    private:
        uint16_t num_signatures;
        uint32_t minAdyNodes;
        uint32_t shingle_size = 1;
        uint64_t prime = (1ULL << 61ULL) - 1ULL;;

        std::hash<string> hash_nodes;

        vector<uint64_t> A;
        vector<uint64_t> B;
};

#endif