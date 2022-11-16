#include "Trie.hpp"

using namespace std; 

Trie::Trie(){
    root = NULL;
}
Trie::~Trie(){
}

void Trie::create(vector<Node*>* nodes){
    cout << "Creating Trie" << endl;
    for(size_t i = 0; i < nodes->size();i++){
        if( root != NULL){
            if(nodes->size() > 0 && nodes->at(0)->first != root->vertex ){
                return;
            }
        }
        insert(nodes->at(i));
    }
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
