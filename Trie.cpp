#include "Trie.hpp"

// PUBLIC METHODS
Trie::Trie() {}

Trie::~Trie()
{
    for(auto i : forest){
        clear(i);
    }
}

void Trie::create(vector<Node*>* nodes)
{
    //cout << "size: " << nodes->size() << endl;
    for (size_t i = 0; i < nodes->size(); i++)
    {
        auto root = find_in_Forest(nodes->at(i)->getFrontAdjacent());
        if(root != nullptr){
            insertInTree(root, nodes->at(i));
        }
        else {
            insertNewTree(nodes->at(i));
        }
    }
    //printTrie();
}

vector<Biclique*>* Trie::getBicliques(){

    vector<Biclique*> *potential_bicliques = new vector<Biclique*>();

    if (forest.empty()) return potential_bicliques;

    for (auto i : candidates){
        if(i->depth < 2) continue;
        if(i->parent == nullptr) continue;
        if(i->indices->size() <= 1 ) continue;

        auto tempBiclique = new Biclique();
        tempBiclique->S = i->indices; 

        if(i->weight != -1) tempBiclique->C_w = new vector<pair<uInt, uInt>>();
        else tempBiclique->C = new vector<uInt>();

        TrieNode* ptr = i;
        do {
            if (ptr->weight != -1) {
                tempBiclique->C_w->push_back(make_pair(ptr->vertex, ptr->weight));
            } else {
                tempBiclique->C->push_back(ptr->vertex);
            }
            ptr = ptr->parent; 
        } while (ptr != nullptr);
        potential_bicliques->push_back(tempBiclique);
    }
    return potential_bicliques;

}

void Trie::printTrie()
{
    if (forest.empty()) {
        cout << "no forest" << endl;
        return;
    } else {
        cout << "Print Forest" << endl;
        printForest();
        cout << "finish print" << endl;
    }
}

/*
void Trie::computeCandidatesBicliques(TrieNode* node, vector<Biclique*>* candidate_bicliques)
{
    uint64_t index = (node->depth - 1) * (node->indices->size() - 1);
    if (index > 0)
    {
        // comprobar en el vector
        if (node->indices->size() != node->parent->indices->size())
        {
            Biclique* b = new Biclique();

            b->S = node->indices;
            for (auto it_node = b->S->begin(); it_node != b->S->end(); it_node++)
                (*it_node)->sort();

            b->C.push_back(node->vertex);
            TrieNode* parent_node = node->parent;
            while (parent_node != nullptr)
            {
                b->C.push_back(parent_node->vertex);
                parent_node = parent_node->parent;
            }
            candidate_bicliques->push_back(b);
        }
        else
        {
            Biclique<NodeType> *b = candidate_bicliques->at(candidate_bicliques->size() - 1);
            b->C.push_back(node->vertex);
        }
    }

    if (node->childrens->size() > 0)
    {
        for (size_t i = 0; i < node->childrens->size(); i++)
            computeCandidatesBicliques(node->childrens->at(i), candidate_bicliques);
    }
}*/

TrieNode* Trie::find(uInt &vertex, TrieNode* ptr)
{
    if (ptr == nullptr) {
        return nullptr;
    }
    if (ptr->vertex == vertex) {
        return ptr;
    }
    for (size_t i = 0; i < ptr->childrens->size(); i++) {
        if (ptr->childrens->at(i)->vertex == vertex) {
            return ptr->childrens->at(i);
        }
    }
    return nullptr;
}

TrieNode* Trie::find(pair<uInt, uInt> node, TrieNode* ptr)
{
    if (ptr == nullptr) return nullptr;
    
    if (ptr->vertex == node.first and ptr->weight == node.second) return ptr;
    
    for(auto i = ptr->childrens->begin(); i != ptr->childrens->end(); i++){
        if((*i)->vertex == node.first and (*i)->weight == node.second){
            return *i; 
        }
    }
    return nullptr;
}

void Trie::clear(TrieNode* node)
{
    if (node->childrens->size() > 0) {
        for (size_t i = 0; i < node->childrens->size(); i++) {
            clear(node->childrens->at(i));
        }
    }
    delete node->indices;
    delete node->childrens;
    delete node;
}

void Trie::printForest() 
{
    for (size_t i = 0; i < forest.size(); i++) {
        cout << "**** trie: " << i << " *****" <<endl;
        printTree(forest[i]);
        cout << "******************" << endl;
    }

    uInt x = 0;
    cout << "Best candidate: " ;
    for(auto i : candidates){
        cout << x++ << " "<< i->vertex << endl;
    }
}

void Trie::printTree(TrieNode* node)
{
    for(size_t esp = 0; esp < (node->depth-1) * 3; esp++)cout << " ";
    
    cout << "Nodo: " << "(" << node->vertex << ",";
    if(node->weight != -1) cout << node->weight; 
    cout << ")" << endl;
    
    for(size_t esp = 0; esp < (node->depth-1) * 3; esp++)cout << " ";
    cout << "Indices: ";
    for (size_t i = 0; i < node->indices->size(); i++) cout << node->indices->at(i)->getId() << " ";
    cout << endl << endl;
    for(size_t esp = 0; esp < (node->depth-1) * 3; esp++)cout << " ";
    cout << "Hijos de " << node->vertex << endl;
    for (size_t i = 0; i < node->childrens->size(); i++) printTree(node->childrens->at(i));
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

void Trie::insertInTree(TrieNode* root, Node* node)
{
    if(node->isWeighted()) {
        auto ptr = root; 
        for(auto adj = node->wAdjacentsBegin(); adj != node->wAdjacentsEnd(); adj++){
            auto t_node = find((*adj), ptr);
            if(t_node != nullptr){ 
                t_node->indices->push_back(node);
            } else {  //crea una ramificacion
                t_node = new TrieNode();
                t_node->indices = new vector<Node*>();
                t_node->childrens = new vector<TrieNode*>();
                t_node->vertex = (*adj).first;
                t_node->weight = (*adj).second;
                t_node->indices->push_back(node);
                t_node->depth = ptr->depth + 1;
                t_node->parent = ptr;
                t_node->treeIndex = root->treeIndex;
                ptr->childrens->push_back(t_node);
            }
            
            //check SxC and S + C;
            uInt S_prev = candidates[root->treeIndex]->indices->size() ;
            uInt C_prev = candidates[root->treeIndex]->depth;
            uInt SxC_prev = S_prev * C_prev;
            float coef_prev  = (float)SxC_prev / (float)(S_prev + C_prev); 

            uInt S_new = t_node->indices->size();
            uInt C_new = t_node->depth;
            uInt SxC_new = S_new * C_new; 
            float coef_new = (float)SxC_new / (float)(S_new + C_new);
            
            
            if(t_node->indices->size() > 1){ //evita nodos hoja de uno solo
                /*if ( (coef_prev) < (coef)){
                    candidates[root->treeIndex] = t_node;
                } else {
                    if ((SxC_prev < SxC_new)){
                        candidates[root->treeIndex] = t_node;
                    }
                }*/
                if (SxC_prev <= SxC_new and coef_prev < coef_new){
                    candidates[root->treeIndex] = t_node;
                }
                
                //cout << node->getId() << ", " << t_node->vertex << "," << t_node->weight  << "," << t_node->depth << "," << "|| " ;
                //cout << "coef_prev : " << coef_prev << endl;
                //cout << "coef new: " << coef << endl;
                
                    
            }
            ptr = t_node;
        }
    } else {
        auto ptr = root; 
        for(auto adj = node->adjacentsBegin(); adj != node->adjacentsEnd(); adj++){
            auto t_node = find((*adj), ptr);
            if(t_node != nullptr){ 
                t_node->indices->push_back(node);
            } else {  //crea una ramificacion
                t_node = new TrieNode();
                t_node->indices = new vector<Node*>();
                t_node->childrens = new vector<TrieNode*>();
                t_node->vertex = (*adj);
                t_node->indices->push_back(node);
                t_node->depth = ptr->depth + 1;
                t_node->parent = ptr;
                t_node->treeIndex = root->treeIndex;
                ptr->childrens->push_back(t_node);
            }
            
            //check SxC;
            uInt S_prev = candidates[root->treeIndex]->indices->size() ;
            uInt C_prev = candidates[root->treeIndex]->depth;
            uInt S_new = t_node->indices->size();
            uInt C_new = t_node->depth;
            
            if(t_node->indices->size() > 1){ //evita nodos hoja de uno solo
                if ( (S_prev * C_prev) < (S_new * C_new)){
                    candidates[root->treeIndex] = t_node;
                }
            }
            ptr = t_node;
        }

    }    
}

void Trie::insertNewTree(Node* node)
{
    auto root = new TrieNode();
    forest.push_back(root);
    candidates.push_back(root);
    root->indices = new vector<Node*>();
    root->childrens = new vector<TrieNode*>();
    root->vertex = node->getFrontAdjacent();
    root->parent = nullptr; 
    root->depth = 1; 
    if(node->isWeighted()) root->weight = node->getFrontWeighted().second;
    root->treeIndex = forest.size()-1;
    insertInTree(root, node);
}
