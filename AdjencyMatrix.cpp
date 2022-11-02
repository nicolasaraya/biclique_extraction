#include "AdjencyMatrix.hpp"

AdjencyMatrix::AdjencyMatrix(){
    
}
AdjencyMatrix::~AdjencyMatrix(){
    matrix.clear();
}

uint64_t AdjencyMatrix::size(){
    return matrix.size();
}

void AdjencyMatrix::insert(Node node){
    matrix.push_back(node);
}

Node AdjencyMatrix::getNode(uint64_t i){
    return matrix.at(i);
}
