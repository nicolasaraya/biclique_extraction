#include <Trie.hpp>
#include <GraphStd.hpp>

// PUBLIC METHODS
Trie::Trie() {}

Trie::~Trie()
{
  for(auto i : forest){
    clear(i);
  }
}

void Trie::create(Signatures* nodes)
{
  //cout << "size: " << nodes->size() << endl;
  for (auto& i : *nodes) {
    auto& node = i;
    auto root = find_in_Forest(node->ptrNode->getFrontAdjacent());
    if (root != nullptr) {
      insertInTree(root, node->ptrNode);
    } else {
      insertNewTree(node->ptrNode);
    }
  }
  //printTrie();
}

void Trie::create(std::vector<NodePtr>* nodes)
{
  //cout << "size: " << nodes->size() << endl;
  for (auto& i : *nodes) {
    auto root = find_in_Forest(i->getFrontAdjacent());
    if (root != nullptr) {
      insertInTree(root, i);
    } else {
      insertNewTree(i);
    }
  }
  //printTrie();
}

std::vector<BicliquePtr>& Trie::getBicliques()
{
  if (forest.empty()) return potential_bicliques;

  for (auto ptr : candidates) {
    if (ptr->depth < 2 or not ptr->parent or ptr->indices.size() <= 1) {
      continue;
    }

    auto tempBiclique = std::make_unique<Biclique>();
    tempBiclique->S = ptr->indices; 

    //TrieNode* ptr = i;
    do {
      if (ptr->weight != -1) {
        tempBiclique->C_w.push_back(Pair(ptr->vertex, ptr->weight));
      } else {
        tempBiclique->C.push_back(ptr->vertex);
      }
      ptr = ptr->parent; 
    } while (ptr != nullptr);
    potential_bicliques.push_back(std::move(tempBiclique));
  }
  return potential_bicliques;
}

void Trie::printTrie()
{
  if (forest.empty()) {
    std::cout << "no forest" << std::endl;
    return;
  } else {
    std::cout << "Print Forest" << std::endl;
    printForest();
    std::cout << "finish print" << std::endl;
  }
}


TrieNode* Trie::find(uInt &vertex, TrieNode* ptr)
{
  if (ptr == nullptr) {
    return nullptr;
  }
  if (ptr->vertex == vertex) {
    return ptr;
  }
  for (size_t i = 0; i < ptr->childrens.size(); i++) {
    if (ptr->childrens.at(i)->vertex == vertex) {
      return ptr->childrens.at(i);
    }
  }
  return nullptr;
}

TrieNode* Trie::find(Pair& node, TrieNode* ptr)
{
  if (ptr == nullptr) return nullptr;
  
  if (ptr->vertex == node.first and ptr->weight == node.second) return ptr;
  
  for(auto i = ptr->childrens.begin(); i != ptr->childrens.end(); i++){
    if((*i)->vertex == node.first and (*i)->weight == node.second){
      return *i; 
    }
  }
  return nullptr;
}

void Trie::clear(TrieNode* node)
{
  if (node->childrens.size() > 0) {
    for (size_t i = 0; i < node->childrens.size(); ++i) {
      clear(node->childrens.at(i));
    }
  }
  delete node;
}

void Trie::printForest() 
{
  for (size_t i = 0; i < forest.size(); ++i) {
    std::cout << "**** trie: " << i << " *****" << std::endl;
    printTree(forest.at(i));
    std::cout << "******************" << std::endl;
  }

  uInt x = 0;
  std::cout << "Best candidate: " ;
  for (auto i : candidates) {
    std::cout << x++ << " "<< i->vertex << std::endl;
  }
}

void Trie::printTree(TrieNode* node)
{
  for (size_t esp = 0; esp < (node->depth-1) * 3; ++esp) {
    std::cout << " ";
  }
  
  std::cout << "Nodo: " << "(" << node->vertex << ",";
  if (node->weight != -1) {
    std::cout << node->weight;
  }
  std::cout << ")" << std::endl;
  
  for (size_t esp = 0; esp < (node->depth-1) * 3; ++esp) {
    std::cout << " ";
  } 

  std::cout << "Indices: ";
  for (size_t i = 0; i < node->indices.size(); ++i) {
    std::cout << node->indices.at(i)->getId() << " ";
  } 
  std::cout << std::endl << std::endl;
  for(size_t esp = 0; esp < (node->depth-1) * 3; ++esp) {
     std::cout << " ";
  }

  std:: cout << "Hijos de " << node->vertex << std::endl;
  for (size_t i = 0; i < node->childrens.size(); ++i) {
    printTree(node->childrens.at(i));
  }
}

TrieNode* Trie::find_in_Forest(uInt id)
{
  for(int i = 0; i < forest.size(); i++){
    if(forest[i]->vertex == id){
      return forest[i];
    }
  }
  return nullptr;
}

void Trie::insertInTree(TrieNode* root, NodePtr node)
{
  if (node->isWeighted()) {
    auto ptr = root; 
    for(auto adj = node->wAdjacentsBegin(); adj != node->wAdjacentsEnd(); ++adj) {
      auto t_node = find((*adj), ptr);
      if (t_node != nullptr) { 
        t_node->indices.push_back(node);
      } else {  //crea una ramificacion
        t_node = new TrieNode();
        //t_node->indices = new vector<NodePtr>();
        //t_node->childrens = new vector<TrieNode*>();
        t_node->vertex = (*adj).first;
        t_node->weight = (*adj).second;
        t_node->indices.push_back(node);
        t_node->depth = ptr->depth + 1;
        t_node->parent = ptr;
        t_node->treeIndex = root->treeIndex;
        ptr->childrens.push_back(t_node);
      }
      
      //check SxC and S + C;
      uInt S_prev = candidates[root->treeIndex]->indices.size() ;
      uInt C_prev = candidates[root->treeIndex]->depth;
      uInt SxC_prev = S_prev * C_prev;
      float coef_prev  = (float)SxC_prev / (float)(S_prev + C_prev); 

      uInt S_new = t_node->indices.size();
      uInt C_new = t_node->depth;
      uInt SxC_new = S_new * C_new; 
      float coef_new = (float)SxC_new / (float)(S_new + C_new);
      
      if (t_node->indices.size() > 1) { //evita nodos hoja de uno solo
        if (SxC_prev <= SxC_new and coef_prev < coef_new){
            candidates[root->treeIndex] = t_node;
        }
      }
      ptr = t_node;
    }
  } else {
    auto ptr = root; 
    for (auto adj = node->adjacentsBegin(); adj != node->adjacentsEnd(); ++adj) {
      auto t_node = find((*adj), ptr);
      if (t_node != nullptr) { 
        t_node->indices.push_back(node);
      } else {  //crea una ramificacion
        t_node = new TrieNode();
        //t_node->indices = new vector<NodePtr>();
        //t_node->childrens = new vector<TrieNode*>();
        t_node->vertex = (*adj);
        t_node->indices.push_back(node);
        t_node->depth = ptr->depth + 1;
        t_node->parent = ptr;
        t_node->treeIndex = root->treeIndex;
        ptr->childrens.push_back(t_node);
      }
      
      //check SxC;
      uInt S_prev = candidates[root->treeIndex]->indices.size() ;
      uInt C_prev = candidates[root->treeIndex]->depth;
      uInt SxC_prev = S_prev * C_prev;
      float coef_prev  = (float)SxC_prev / (float)(S_prev + C_prev); 

      uInt S_new = t_node->indices.size();
      uInt C_new = t_node->depth;
      uInt SxC_new = S_new * C_new; 
      float coef_new = (float)SxC_new / (float)(S_new + C_new);

      if (t_node->indices.size() > 1) { //evita nodos hoja de uno solo
        if (SxC_prev <= SxC_new and coef_prev < coef_new) {
          candidates[root->treeIndex] = t_node;
        }
      }
      ptr = t_node;
    }
  }
}

void Trie::insertNewTree(NodePtr node)
{
  auto root = new TrieNode();
  forest.push_back(root);
  candidates.push_back(root);
  //root->indices = new vector<Node*>();
  //root->childrens = new vector<TrieNode*>();
  root->vertex = node->getFrontAdjacent();
  root->parent = nullptr; 
  root->depth = 1; 
  if (node->isWeighted()) {
    root->weight = node->getFrontWeighted()->second;
  }
  root->treeIndex = forest.size()-1;
  insertInTree(root, node);
}
