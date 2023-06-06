#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include "Define.hpp"

using namespace std;

class Shingle
{
public:
    // PUBLIC METHODS
    Shingle(uint16_t, uint32_t, uint32_t);
    ~Shingle();
    SignNode *computeShingle(Node *);

private:
    // PRIVATE VARIABLES
    uint16_t num_signatures;
    uint32_t minAdyNodes;
    uint32_t shingle_size = 1;
    uint64_t prime = (1ULL << 31ULL) - 1ULL;

    std::hash<string> hash_nodes;

    vector<uInt> A;
    vector<uInt> B;
};

#endif