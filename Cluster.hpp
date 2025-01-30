#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>

#include <Graph/Node.hpp>
#include <Graph/GraphStd.hpp>
#include <Trie.hpp>
#include <Shingle.hpp>
#include <Graph/BicliqueExtracted.hpp>

class Trie;

class Cluster 
{
  public:
    Cluster(std::vector<NodePtr>* entry);
    Cluster(Signatures* entry);
    ~Cluster() = default;
    void computeTrie();
    std::vector<BicliquePtr>& getBicliques();
    void printCluster();
    void printMap();
      
  private:
    std::vector<NodePtr>* nodes = nullptr;
    Signatures* signatures = nullptr;
    std::unordered_map<uInt, uint32_t> mapFrecuency; // Valor Nodo, Frecuencia
    std::unordered_map<std::string, uint32_t> mapFrecuencyWeighted; // Valor Nodo, Frecuencia
    uint16_t minFreq = 1;
    std::unique_ptr<Trie> t;
    bool weighted = false;
    
    bool sortSizeCompSign(const SignNode& a, const SignNode& b);
    bool sortSizeComp(const NodePtr& a, const NodePtr& b);
    void computeFrecuency();
    void computeFrecuencyFromSignatures();
    
    void computeHistogram();
    void computeHistogramFromSignatures();

    void updateFrecuency(uInt& id);
    void updateFrecuency(std::string& id);
};


#endif