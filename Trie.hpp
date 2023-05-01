#ifndef TRIE_HPP
#define TRIE_HPP

#include <fstream>
#include <map>
#include "Define.hpp"

using namespace std;

class Trie
{
public:
    // PUBLIC METHODS
    Trie();
    ~Trie();

    void create(vector<Node *> *);
    vector<Biclique *> getBicliques();
    void printTrie();

    // PUBLIC VARIABLES
    uint64_t size_bicliques = 0;

private:
    // PRIVATE VARIABLES
    TrieNode *root;

    // PRIVATE METHODS
    TrieNode *find(uint64_t &, TrieNode *);
    void clear(TrieNode *);
    void computeCandidatesBicliques(TrieNode *, map<vector<Node *> *, TrieNode *> *);
    void insert(Node *);
    void print(TrieNode *);
};

#endif