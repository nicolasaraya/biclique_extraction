#ifndef TRIE_HPP
#define TRIE_HPP

#include "Utils/Utils.hpp"
#include "Graph/Node.hpp"
#include "Graph/NodeWeighted.hpp"

using namespace std;

template<typename NodeType>
struct TrieNode{
    TrieNode *parent;
    uInt vertex;
    uInt weight;
    vector<NodeType*>* indices;
    vector<TrieNode*>* childrens;
    uint32_t depth;
};

template struct TrieNode<Node>;
template struct TrieNode<NodeWeighted>;

template<typename NodeType>
struct Biclique{
    vector<NodeType*>* S;
    vector<uInt> C;
};

template struct Biclique<Node>;
template struct Biclique<NodeWeighted>;

template<typename NodeType> 
class Trie
{
public:
    // PUBLIC METHODS
    Trie();
    ~Trie();

    void create(vector<NodeType *> *);
    vector<Biclique<NodeType>*> *getBicliques();
    void printTrie();

    // PUBLIC VARIABLES
    uint64_t size_bicliques = 0;

private:
    // PRIVATE VARIABLES
    TrieNode<NodeType> *root;

    // PRIVATE METHODS
    TrieNode<NodeType> *find(uInt &, TrieNode<NodeType> *);
    void clear(TrieNode<NodeType> *);
    void computeCandidatesBicliques(TrieNode<NodeType> *, vector<Biclique<NodeType> *> *);
    void insert(NodeType*);
    void print(TrieNode<NodeType> *);
};


template class Trie<Node>;
template class Trie<NodeWeighted>;
#endif