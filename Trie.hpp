#ifndef TRIE_HPP
#define TRIE_HPP

#include "Utils.hpp"

using namespace std; 

class Trie{
    public:
        Trie();
        ~Trie();

        void create(vector<Node*>*);

        void print(TrieNode*);
        void printTrie();

    private:
        TrieNode *root;

        TrieNode* find(uint64_t,TrieNode*);
        void insert(Node*);

};

#endif