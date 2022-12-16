#ifndef TRIE_HPP
#define TRIE_HPP

#include <fstream>
#include "Utils.hpp"

using namespace std; 

class Trie{
    public:
        Trie();
        ~Trie();
        ofstream file;
        void create(vector<Node*>*);
        Biclique* getBiclique();
        void printTrie();

        uint64_t size_bicliques = 0;

    private:
        TrieNode* root;

        TrieNode* candidateBiclique;

        TrieNode* find(uint64_t&, TrieNode*);
        void clear(TrieNode*);
        void computeCandidateBiclique(TrieNode*);
        void computeBiclique(Biclique*, TrieNode*);
        void insert(Node*);
        void print(TrieNode*);

};

#endif