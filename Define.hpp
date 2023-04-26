#ifndef DEFINE_HPP
#define DEFINE_HPP

#include "Node.hpp"
#include "Utils.hpp"

using namespace std; 

struct SignNode{
    Node* ptrNode;
    vector<uint64_t> minHash; 
};

typedef pair<vector<Node*>*, vector<uint64_t>> Biclique;
typedef vector<SignNode*> Signatures; 

struct TrieNode{
    TrieNode *parent;
    uint64_t vertex;
    vector<Node*>* indices;
    vector<TrieNode*>* childrens;
    uint32_t depth;
};


#endif