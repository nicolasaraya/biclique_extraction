#ifndef DEFINE_HPP
#define DEFINE_HPP

#define BITS64

#if defined(BITS32)
    typedef int Int;
    typedef unsigned int uInt;
#elif defined(BITS64) 
    typedef long long int Int;
    typedef unsigned long long int uInt;
#endif

#include "Node.hpp"
#include "Utils.hpp"

using namespace std; 

struct SignNode{
    Node* ptrNode;
    vector<uInt> minHash; 
};

typedef pair<vector<Node*>*, vector<uInt>> Biclique;
typedef vector<SignNode*> Signatures; 
typedef vector<SignNode*>::iterator SignatureIterator;

struct TrieNode{
    TrieNode *parent;
    uInt vertex;
    vector<Node*>* indices;
    vector<TrieNode*>* childrens;
    uint32_t depth;
};


#endif