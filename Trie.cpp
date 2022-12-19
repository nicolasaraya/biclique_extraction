#include "Trie.hpp"

////////////////////////////////////////////////////////////////PUBLIC METHODS //////////////////////////////////////////////////////////////////

Trie::Trie(){
    root = NULL;
    candidateBiclique = NULL;
    file.open("trie-error.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    
}

Trie::~Trie(){
    if(root != NULL){
        clear(root);
    }
}

void Trie::create(vector<Node*>* nodes){
    for(size_t i = 0; i < nodes->size();i++){
        if( root != NULL){
            if(nodes->at(i)->adyNodes[0] != root->vertex ){
                continue;
            }
        }
        insert(nodes->at(i));
    }
}
Biclique* Trie::getBiclique(){
    //printTrie();
    Biclique* b = new Biclique();
    computeCandidateBiclique(root);
    if(candidateBiclique != NULL){
        computeBiclique(b, candidateBiclique);
        /*
        cout << "Biclique: ";
        cout << "S: ";
        for(uint64_t i = 0; i < b->first->size(); i++){
            cout << b->first->at(i)->nodeID << " ";
        }
        cout << endl;
        cout << "C: ";
        for(uint64_t i = 0; i < b->second.size(); i++){
            cout << *(b->second[i]) << " ";
        }
        cout << endl;
        cout << "coeficiente con -1: " << (b->first->size()-1) * (b->second.size()-1) << endl;
        cout << "coeficiente: " << (b->first->size())*(b->second.size()) << endl;*/
        
        
    }
    else{
        return NULL;
        //cout << "el candidato es nulo" << endl;
        //printTrie();
    }
    return b;
    
}

void Trie::computeCandidateBiclique(TrieNode* node){
    if(node->childrens->size() > 0){
        for(size_t i = 0; i < node->childrens->size(); i++){
            computeCandidateBiclique(node->childrens->at(i));
        }
    }
    if( candidateBiclique == NULL ){
        if( (node->depth - 1)*(node->indices->size() - 1) > 0 ){
            candidateBiclique = node;
        }
    }
    else if( (candidateBiclique->depth - 1)*(candidateBiclique->indices->size() - 1) < (node->depth - 1)*(node->indices->size() - 1) ){
        candidateBiclique = node;
    }

}

void Trie::computeBiclique(Biclique* b, TrieNode* node){
    if(node == candidateBiclique){
        b->first = node->indices;
    }
    b->second.push_back(&node->vertex);

    if(node->parent == NULL){
        return;
    }

    computeBiclique(b, node->parent);

}

void Trie::printTrie(){
    if(root == NULL){
        file << "root is NULL" << endl;
    }
    else{
        file << "print trie" << endl;
        print(root);
        file << "finish print" << endl;
    }
}

////////////////////////////////////////////////////////////////PRIVATE METHODS //////////////////////////////////////////////////////////////////

TrieNode* Trie::find(uint64_t& vertex, TrieNode* ptr){
    if(ptr == NULL){
        return NULL;
    }
    if(ptr->vertex == vertex){
        return ptr;
    }
    for(size_t i = 0; i < ptr->childrens->size();i++){
        if(ptr->childrens->at(i)->vertex == vertex){
            return ptr->childrens->at(i);
        }
    }
    return NULL;
}

void Trie::clear(TrieNode* node){
    
    if(node->childrens->size() > 0){
        for(size_t i = 0; i < node->childrens->size();i++){
            clear(node->childrens->at(i));
        }
    }
    delete node->indices;
    delete node->childrens;
    delete node;
    
}


void Trie::insert(Node* node){
    TrieNode* t_node;
    TrieNode* ptr = root;

    for(size_t i = 0; i < node->adyNodes.size();i++){
        t_node = find(node->adyNodes[i], ptr);

        if(t_node != NULL){  // Si ya existe
            t_node->indices->push_back(node);
        }
        else{  //No existe -> crear
            t_node = new TrieNode();
            t_node->indices = new vector<Node*>();
            t_node->childrens = new vector<TrieNode*>();
            t_node->vertex = node->adyNodes[i];
            t_node->indices->push_back(node);
            t_node->depth = i+1;

            if(i == 0){
                root = t_node;
                t_node->parent = NULL;
            }
            else{
                t_node->parent = ptr;
                ptr->childrens->push_back(t_node);
            }
        }
        ptr = t_node;
    }
}

void Trie::print(TrieNode* node){
    file << "Nodo: " << node->vertex << endl;
    file << "Indices: ";
    for(size_t i = 0; i < node->indices->size();i++){
        file << node->indices->at(i)->nodeID << " ";
    }
    file <<endl;
    for(size_t i = 0; i < node->childrens->size();i++){
        print(node->childrens->at(i));
    }

}
