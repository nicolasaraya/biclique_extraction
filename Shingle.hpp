#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include "Utils/Utils.hpp"
#include "Graph/Node.hpp"
#include "Graph/NodeWeighted.hpp"

using namespace std;

template <typename NodeType>
struct SignNode{
    NodeType* ptrNode = nullptr;
    vector<uInt> minHash; 
};

template<typename NodeType>
class Shingle
{
public:
    // PUBLIC METHODS
    typedef vector<SignNode<NodeType>*> Signatures; 
    typedef vector<SignNode<NodeType>*>::iterator SignatureIterator;
    Shingle(uint16_t, uint32_t, uint32_t);
    ~Shingle();
    SignNode<NodeType>* computeShingle(NodeType *);

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

template struct SignNode<NodeWeighted>;
template struct SignNode<Node>;
template class Shingle<NodeWeighted>;
template class Shingle<Node>;
#endif