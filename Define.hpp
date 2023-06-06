#ifndef DEFINE_HPP
#define DEFINE_HPP

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