#include <Graph/GraphStd.hpp>

#include <Cluster.hpp>
#include <Trie.hpp>
#include <Biclique.hpp>

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>



class Trie;

// PUBLIC METHODS 
// Cluster::Cluster(std::vector<NodePtr>& entry) : nodes(entry)
// {
//   t = std::make_unique<Trie>();
//   weighted = nodes.front()->isWeighted();
// }


Cluster::Cluster(std::vector<NodePtr>* entry) 
{
  nodes = entry;
  t = std::make_unique<Trie>();
  weighted = nodes->front()->isWeighted();
}

Cluster::Cluster(Signatures* entry) 
{
  signatures = entry;
  t = std::make_unique<Trie>();
  weighted = signatures->front()->ptrNode->isWeighted();
}

void Cluster::printMap()
{
  std::cout << "********MAP**********" << std::endl;
  if (weighted) {
    for (auto i : mapFrecuencyWeighted) {
      if (i.second > 2) {
        std::cout << "(" << i.first << ")" << ", freq: " << i.second << std::endl;
      }
    }
  } else {
    for (auto i : mapFrecuency) {
      std::cout << i.first << ", freq: " << i.second << std::endl;
    }
  }
}

void Cluster::updateFrecuency(uInt& id) 
{
  auto aux = mapFrecuency.find(id);
  if (aux != mapFrecuency.end()) { // existe
    mapFrecuency[id]++;
  } else {
    mapFrecuency[id] = 1; // insert freq 1;
  }
}

void Cluster::updateFrecuency(std::string& id)
{
  auto aux = mapFrecuencyWeighted.find((id));
  if (aux != mapFrecuencyWeighted.end()) { // existe
    mapFrecuencyWeighted[id]++;
  } else {
    mapFrecuencyWeighted[id] = 1; // insert freq 1;
  }
}
void Cluster::computeFrecuency()
{
  if(weighted){
    for (auto& node : *nodes) {
      for (auto j = node->wAdjacentsBegin(); j != node->wAdjacentsEnd(); j++) {   
        std::string node_id = std::to_string((*j).first) + "," + std::to_string((*j).second); 
        updateFrecuency(node_id);
      }
    }
  } else {
    for (auto& node : *nodes) {
      for (auto j = node->adjacentsBegin(); j != node->adjacentsEnd(); j++) {   
        updateFrecuency(*j);
      }
    }
  }   
}

void Cluster::computeFrecuencyFromSignatures()
{
  if(weighted){
    for (auto& signNode : *signatures) {
      auto& node = signNode->ptrNode;
      for (auto j = node->wAdjacentsBegin(); j != node->wAdjacentsEnd(); j++) {   
        std::string node_id = std::to_string((*j).first) + "," + std::to_string((*j).second); 
        updateFrecuency(node_id);
      }
    }
  } else {
    for (auto& signNode : *signatures) {
      auto& node = signNode->ptrNode;
      for (auto j = node->adjacentsBegin(); j != node->adjacentsEnd(); j++) {   
        updateFrecuency(*j);
      }
    }
  }   
}

void Cluster::computeHistogramFromSignatures()
{
  computeFrecuency();

  for (auto& signNode : *signatures) {
    auto& node = signNode->ptrNode;
    if (weighted) {
      node->sortByFrecuency(&mapFrecuencyWeighted);
      node->moveToCache(&mapFrecuencyWeighted, minFreq);
    } else {
      node->sortByFrecuency(&mapFrecuency);
      node->moveToCache(&mapFrecuency, minFreq);
    }
  }

  std::sort(signatures->begin(), signatures->end(), std::bind(&Cluster::sortSizeCompSign, this, std::placeholders::_1, std::placeholders::_2));
}


void Cluster::computeHistogram()
{
  computeFrecuency();

  for (auto& node : *nodes) {
    if (weighted) {
      node->sortByFrecuency(&mapFrecuencyWeighted);
      node->moveToCache(&mapFrecuencyWeighted, minFreq);
    } else {
      node->sortByFrecuency(&mapFrecuency);
      node->moveToCache(&mapFrecuency, minFreq);
    }
  }

  std::sort(nodes->begin(), nodes->end(), std::bind(&Cluster::sortSizeComp, this, std::placeholders::_1, std::placeholders::_2));
}

void Cluster::computeTrie()
{
  if (nodes) {
    computeHistogram();
    t->create(nodes);
  } else {
    computeHistogramFromSignatures();
    t->create(signatures);
  }
  
}

std::vector<BicliquePtr>& Cluster::getBicliques()
{
  return t->getBicliques(); 
}

void Cluster::printCluster()
{
  
  std::cout << std::endl << "***************" << std::endl;
  if (nodes) {
    for (size_t i = 0; i < nodes->size(); ++i) {
      nodes->at(i)->print();
    }
  } else {
    for (size_t i = 0; i < signatures->size(); ++i) {
      signatures->at(i)->ptrNode->print();
    }
  }
  std::cout << "***************" << std::endl << std::endl;
}
 
bool Cluster::sortSizeCompSign(const SignNode& a, const SignNode& b)
{
  if (a->ptrNode->edgesSize() > b->ptrNode->edgesSize()) {
    return true;
  } else if (a->ptrNode->edgesSize() == b->ptrNode->edgesSize()) {
    return (a->ptrNode->getId() < b->ptrNode->getId());
  } else {
    return false;
  }
}

bool Cluster::sortSizeComp(const NodePtr& a, const NodePtr& b)
{
  if (a->edgesSize() > b->edgesSize()) {
    return true;
  } else if (a->edgesSize() == b->edgesSize()) {
    return (a->getId() < b->getId());
  } else {
    return false;
  }
}


