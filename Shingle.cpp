#include "Shingle.hpp"

using namespace std; 

Shingle::Shingle(uint16_t num_signatures){
    this->num_signatures = num_signatures;
    shingle_size = 1;
    prime = (pow(2,61)-1);

    for (size_t i = 0; i < num_signatures; i++){
        A.push_back( rand() % prime + 1);
        B.push_back( rand() % prime + 1); 
    }    
    
    // allocate the space for the comparison of shingles
    //signNode.reserve(numberSignatures * currentNumRows);
    //bcsize = bcs;
    
    
}
Shingle::~Shingle(){

}


SignNode* Shingle::computeShingle(Node* _node){
    vector<uint64_t>* ady_nodes = &(_node->second);
    if (ady_nodes->size() == 0){
        return NULL; 
    }
    cout << ady_nodes->size() << endl;
    //if(ady_nodes->size() < shingle_size ) return NULL;

    vector<uint64_t> MIN; 
    for (size_t i = 0; i < num_signatures; i++) MIN.push_back(prime);


    for(size_t i = 0; i < ady_nodes->size() - shingle_size ; i++){
        string shingle_ = ""; 
        for(size_t j = 0; j < shingle_size && i + j < ady_nodes->size(); j++){
            //cout << "    " <<  ady_nodes->at(i+j) ;
            shingle_ += to_string( ady_nodes->at(i+j));
        }
        uint64_t shingleID = hash_nodes(shingle_);

        for(uint16_t k = 0; k < num_signatures; k++){
            uint64_t shingleHash = (A[k] * shingleID + B[k]) % prime;

            if(shingleHash < MIN[k]){
                MIN[k] = shingleHash;
            }
        } 

    }
    //cout << MIN.size() << endl;
    SignNode* s = new SignNode(_node, MIN);
    return s;

    /*
    vector<uint64_t> HashedNodes(shingle_size); 

    for(size_t i = 0; i < ady_nodes.size(); i++){ 
        HashedNodes[ i % shingleSize ] = hash_nodes(ady_nodes[i]);

        if (i+1 >= shingleSize){  // skip to next if not yet a shingle-word
       
	        uint64_t shingleID = 0;
	        for(auto h : HashedNodes) shingleID += h// each shingle
	
	        for (size_t j = 0; j < numberSignatures_; j++){
	 
	            shingleHash = (( A_[j] *  shingleID)  + B_[j]) % prime_;
	
	            if (shingleHash < MIN_[j]){
                    MIN_[j] = shingleHash;
	            }
	        } 
        } 

    } 
    */
    

   // each vertex 


}
