#include "Shingles.hpp"

using namespace std;

Shingle::Shingle(unsigned char num_signatures, unsigned int biclSize, AdjencyMatrix* adjMatrix){
    this->adjMatrix = adjMatrix;
    this->num_signatures = num_signatures;
    this->biclSize = biclSize; 
}

Shingle::~Shingle(){
}

Shingle::computeShingles(){
    std::hash<string> string_hash //?? ??? ?? 


}