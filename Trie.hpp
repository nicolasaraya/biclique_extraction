#ifndef TRIE_HPP
#define TRIE_HPP

#include "Utils.hpp"

using namespace std; 

class Trie{
    public:
        Trie();
        ~Trie();

        void create(vector<Node*>*);
        void getBiclique();
        void printTrie();

    private:
        TrieNode *root;
        
        vector<biclique> bicliques;

        TrieNode* find(uint64_t,TrieNode*);
        void clear(TrieNode*);
        void getBiclique(TrieNode*,vector<uint64_t>*);
        void insert(Node*);
        void print(TrieNode*);

};

#endif