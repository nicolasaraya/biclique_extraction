#ifndef TRIE_HPP
#define TRIE_HPP

#include <fstream>
#include <map>
#include "Define.hpp"

using namespace std;

class Trie
{
public:
    Trie();
    ~Trie();

    uint64_t size_bicliques = 0;

    void create(vector<Node *> *);
    vector<Biclique *> getBicliques();
    void printTrie();

private:
    TrieNode *root;

    TrieNode *find(uint64_t &, TrieNode *);
    void clear(TrieNode *);
    void computeCandidatesBicliques(TrieNode *, map<vector<Node *> *, TrieNode *> *);
    void insert(Node *);
    void print(TrieNode *);
};

#endif