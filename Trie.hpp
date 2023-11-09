#ifndef TRIE_HPP
#define TRIE_HPP

#include "Utils/Utils.hpp"
#include "Graph/Node.hpp"

using namespace std;

struct TrieNode{
    uInt treeIndex; 
    TrieNode *parent;
    uInt vertex;
    uInt weight = -1;
    vector<Node*>* indices;
    vector<TrieNode*>* childrens;
    uint32_t depth;
};

struct Biclique{
    vector<Node*>* S = nullptr;
    vector<uInt>* C = nullptr;
    vector<pair<uInt,uInt>>* C_w = nullptr; 
};

class Trie {
    public:
        Trie();
        ~Trie();
        void create(vector<Node*>*);
        vector<Biclique*> *getBicliques();
        void printForest();
        uint64_t size_bicliques = 0;

    private:
        vector<TrieNode*> forest;
        vector<TrieNode*> candidates; 
        TrieNode* find(uInt&, TrieNode*);
        TrieNode* find(Pair&, TrieNode*);
        void clear(TrieNode*);
        void computeCandidatesBicliques(TrieNode*, vector<Biclique*> *);
        TrieNode* find_in_Forest(uInt);
        void insertInTree(TrieNode*, Node*);
        void insertNewTree(Node*);
        void printTrie();
        void printTree(TrieNode *);
        unordered_map<uInt, uInt> history; 
};

#endif