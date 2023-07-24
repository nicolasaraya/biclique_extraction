#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include "Utils/Utils.hpp"
#include "Graph/Node.hpp"

using namespace std;

struct SignNode{
    Node* ptrNode = nullptr;
    vector<uInt> minHash; 
};


class Shingle
{
    public:
        Shingle(uint16_t, uint32_t, uint32_t);
        ~Shingle();
        SignNode* computeShingle(Node *);

    private:
        uint16_t num_signatures;
        uint32_t minAdyNodes;
        uint32_t shingle_size = 1;
        #if defined(BITS32)
            uInt prime = (1ULL << 31ULL) - 1ULL;
        #else
            uInt prime = (1ULL << 61ULL) - 1ULL;
        #endif
        std::hash<string> hash_nodes;
        vector<uInt> A;
        vector<uInt> B;
};

#endif