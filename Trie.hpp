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
    Biclique *getBiclique();
    void printTrie();

    // PUBLIC VARIABLES
    uint64_t size_bicliques = 0;

private:
    // PRIVATE VARIABLES
    TrieNode *root;
    TrieNode *candidateBiclique;

    // PRIVATE METHODS
    TrieNode *find(uint64_t &, TrieNode *);
    void clear(TrieNode *);
    void computeBiclique(Biclique *, TrieNode *);
    void computeCandidatesBicliques(TrieNode *, map<vector<Node *> *, TrieNode *> *);
    void computeCandidateBiclique(TrieNode *);
    void insert(Node *);
    void print(TrieNode *);
};

#endif