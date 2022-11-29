#include "Trie.hpp"

////////////////////////////////////////////////////////////////PUBLIC METHODS //////////////////////////////////////////////////////////////////

Trie::Trie(){
    root = NULL;
}

Trie::~Trie(){
    if(root != NULL){
        clear(root);
    }
}

void Trie::create(vector<Node*>* nodes){
    for(size_t i = 0; i < nodes->size();i++){
        if( root != NULL){
            if(nodes->at(i)->second.size() > 0 && nodes->at(i)->first != root->vertex ){
                return;
            }
        }
        insert(nodes->at(i));
    }
}
void Trie::getBiclique(){
    vector<uint64_t>* c = new vector<uint64_t>();
    getBiclique(root,c);
    
    for(size_t i = 0; i < bicliques.size(); i++){
        if(bicliques[i].first.size()>1){

        cout << "biclique " << i+1 << endl;

        cout << "S: ";
        for(size_t j = 0; j < bicliques[i].first.size(); j++){
            cout << bicliques[i].first[j] << " ";
        }
        cout << endl;

        cout << "C: ";
        for(size_t j = 0; j < bicliques[i].second.size(); j++){
            cout << bicliques[i].second[j] << " ";
        }
        cout << endl;
        }
    }
}

void Trie::getBiclique(TrieNode* node,vector<uint64_t>* c){
    c->push_back(node->vertex);
    if(node->childrens.size() > 0){
        for(size_t i = 0; i < node->childrens.size(); i++){
            getBiclique(node->childrens[i],c);
        }
    }
    else{
        vector<uint64_t> c_local = *c;
        bicliques.push_back(make_pair(node->indices,c_local));
    }
    c->pop_back();

}

void Trie::printTrie(){
    if(root == NULL){
        cout << "root is NULL" << endl;
    }
    else{
        cout << "print trie" << endl;
        print(root);
        cout << "finish print" << endl;
    }
}

////////////////////////////////////////////////////////////////PRIVATE METHODS //////////////////////////////////////////////////////////////////

TrieNode* Trie::find(uint64_t vertex,TrieNode* ptr){
    if(ptr == NULL){
        return NULL;
    }
    if(ptr->vertex == vertex){
        return ptr;
    }
    for(size_t i = 0; i < ptr->childrens.size();i++){
        if(ptr->childrens[i]->vertex == vertex){
            return ptr->childrens[i];
        }
    }
    return NULL;
}

void Trie::clear(TrieNode* node){
    if(node->childrens.size() > 0){
        for(size_t i = 0; i < node->childrens.size();i++){
            clear(node->childrens[i]);
        }
    }
    delete node;
}


void Trie::insert(Node* node){
    TrieNode* t_node;
    TrieNode* ptr = root;

    for(size_t i = 0; i < node->second.size();i++){
        t_node = find(node->second[i],ptr);
        if(i == 0){  // i = 0 / root
            if(t_node != NULL){  // Si ya existe
                t_node->indices.push_back(node->first);
            }
            else{  //No existe -> crear
                t_node = new TrieNode();
                t_node->vertex = node->second[i];
                t_node->indices.push_back(node->first);

                root = t_node;
            }
        }
        else{  // i > 0
            if(t_node != NULL){  // Si ya existe
                t_node->indices.push_back(node->first);
            }
            else{  //No existe -> crear
                t_node = new TrieNode();
                t_node->vertex = node->second[i];
                t_node->indices.push_back(node->first);

                t_node->parent = ptr;
                ptr->childrens.push_back(t_node);
                
            }
        }
        ptr = t_node;
    }
}

void Trie::print(TrieNode* node){
    cout << "Nodo: " << node->vertex << endl;
    cout << "Indices: ";
    for(size_t i = 0; i < node->indices.size();i++){
        cout << node->indices[i] << " ";
    }
    cout <<endl;
    for(size_t i = 0; i < node->childrens.size();i++){
        print(node->childrens[i]);
    }

}
