#ifndef TRIE_HPP
#define TRIE_HPP

#include <Utils/Utils.hpp>
#include <Graph/Node.hpp>
#include <Graph/GraphStd.hpp>
#include <Shingle.hpp>
#include <Graph/Biclique.hpp>


struct TrieNode 
{
  uInt treeIndex; 
  TrieNode* parent;
  uInt vertex;
  uInt weight = -1;
  std::vector<NodePtr> indices;
  std::vector<TrieNode*> childrens;
  uint32_t depth;
};

class Trie 
{
  public:
    Trie();
    ~Trie();
    void create(Signatures* nodes);
    void create(std::vector<NodePtr>*);
    std::vector<BicliquePtr>& getBicliques();
    void printForest();
    uint64_t size_bicliques = 0;
  private:
    TrieNode* find(uInt&, TrieNode*);
    TrieNode* find(Pair&, TrieNode*);
    void clear(TrieNode*);
    //void computeCandidatesBicliques(TrieNode*, std::vector<BicliquePtr>& );
    TrieNode* find_in_Forest(uInt);
    void insertInTree(TrieNode*, NodePtr);
    void insertNewTree(NodePtr);
    void printTrie();
    void printTree(TrieNode *);

    
    std::vector<TrieNode*> forest;
    std::vector<TrieNode*> candidates; 
    std::unordered_map<uInt, uInt> history; 
    std::vector<BicliquePtr> potential_bicliques;
};

#endif