#include "Shingle.hpp"

using namespace std; 

Shingle::Shingle(uint16_t num_signatures, uint32_t minAN){
    srand(time(NULL));
    this->num_signatures = num_signatures;
    shingle_size = 1;
    minAdyNodes = minAN;
    prime = (1ULL << 61ULL) - 1ULL;
    //cout << "Prime " << prime << endl;
    for (size_t i = 0; i < num_signatures; i++){
        A.push_back( rand() % prime + 1);
        B.push_back( rand() % prime + 1); 
    }    
}
Shingle::~Shingle(){
    A.clear();
    B.clear();
}

SignNode* Shingle::computeShingle(Node* _node){

    if (_node->adyNodes.size() == 0 || _node->adyNodes.size() < minAdyNodes){
        //cout << "entre al primer if / size: " <<  _node->adyNodes.size() << " / minAdyNodes: "  <<  minAdyNodes << endl;
        return NULL; 
    }

    SignNode* s = new SignNode();
    s->ptrNode = _node;


    for (size_t i = 0; i < num_signatures; i++) s->minHash.push_back(prime);

    //cout <<"Size: " << _node->second.size() << endl;
    for(size_t i = 0; i <= _node->adyNodes.size() - shingle_size ; i++){
        string shingle_ = ""; 
        
        for(size_t j = 0; j < shingle_size && i + j < _node->adyNodes.size(); j++){
            //cout << "    " <<  ady_nodes->at(i+j) ;
            shingle_ += to_string( _node->adyNodes.at(i+j));
        }
        uint64_t shingleID = hash_nodes(shingle_);

        for(uint16_t k = 0; k < num_signatures; k++){
            uint64_t shingleHash = (A[k] * shingleID + B[k]) % prime;
            //cout << "shingle: " << shingleHash << " / numero hasheado: " << ady_nodes->at(i) <<"," << _node->first <<endl;
            if(shingleHash < s->minHash[k]){
                s->minHash[k] = shingleHash;
            }
        } 

    }
    //cout << MIN.size() << endl;
    
    return s;
}
