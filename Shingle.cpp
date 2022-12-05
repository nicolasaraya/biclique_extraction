#include "Shingle.hpp"

using namespace std; 

Shingle::Shingle(uint16_t num_signatures){
    srand(time(NULL));
    this->num_signatures = num_signatures;
    shingle_size = 1;
    prime = (1ULL << 61ULL) - 1ULL;
    cout << "Prime " << prime << endl;
    for (size_t i = 0; i < num_signatures; i++){
        A.push_back( rand() % prime + 1);
        B.push_back( rand() % prime + 1); 
        cout << "A: " << A[i] << ", B: " << B[i] << endl;
    }    
    
    // allocate the space for the comparison of shingles
    //signNode.reserve(numberSignatures * currentNumRows);
    //bcsize = bcs;
    
    
}
Shingle::~Shingle(){

}

/*
SignNode* Shingle::computeShingle(Node* _node){
    
    vector<uint64_t>* ady_nodes = &(_node->second);
    if (ady_nodes->size() == 0){
        return NULL; 
    }
    //cout << ady_nodes->size() << endl;
    //if(ady_nodes->size() < shingle_size ) return NULL;

    vector<uint64_t> MIN; 
    for (size_t i = 0; i < num_signatures; i++) MIN.push_back(prime);

    cout <<"Size: " << ady_nodes->size() << endl;
    for(size_t i = 0; i <= ady_nodes->size() - shingle_size ; i++){
        string shingle_ = ""; 
        for(size_t j = 0; j < shingle_size && i + j < ady_nodes->size(); j++){
            //cout << "    " <<  ady_nodes->at(i+j) ;
            if(ady_nodes->at(i+j) == 0){
                cout << " / numero hasheado: " << ady_nodes->at(i) <<"," << _node->first <<endl;
            }
            shingle_ += to_string( ady_nodes->at(i+j));
        }
        uint64_t shingleID = hash_nodes(shingle_);

        for(uint16_t k = 0; k < num_signatures; k++){
            uint64_t shingleHash = (A[k] * shingleID + B[k]) % prime;
            //cout << "shingle: " << shingleHash << " / numero hasheado: " << ady_nodes->at(i) <<"," << _node->first <<endl;
            if(shingleHash < MIN[k]){
                MIN[k] = shingleHash;
            }
        } 

    }
    //cout << MIN.size() << endl;
    SignNode* s = new SignNode(_node, MIN);
    return s;
}*/

SignNode* Shingle::computeShingle(Node* _node){

    if (_node->adyNodes.size() == 0){
        return NULL; 
    }
    /*
    for(auto i: _node->second){
        cout << i << " ";
    }*/
    //cout << endl;
    //cout << ady_nodes->size() << endl;
    //if(ady_nodes->size() < shingle_size ) return NULL;

    //vector<uint64_t> MIN; 
    SignNode* s = new SignNode();
    s->ptrNode = _node;


    for (size_t i = 0; i < num_signatures; i++) s->minHash.push_back(prime);

    //cout <<"Size: " << _node->second.size() << endl;
    for(size_t i = 0; i <= _node->adyNodes.size() - shingle_size ; i++){
        string shingle_ = ""; 
        /*
        if(_node->second[i] == 0){
            cout << " / numero hasheado: " << _node->second[i] <<"," << _node->first <<endl;
        }*/
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
