#include "Shingle.hpp"
using namespace std;

Shingle::Shingle(uint16_t num_signatures, uint32_t minAN, uint32_t shingle_size) : num_signatures(num_signatures),
                                                                                   minAdyNodes(minAN),
                                                                                   shingle_size(shingle_size)
{
    srand(time(NULL));
    for (size_t i = 0; i < num_signatures; i++) {
        A.push_back(rand() % prime + 1);
        B.push_back(rand() % prime + 1);
    }
}

Shingle::~Shingle()
{
    A.clear();
    B.clear();
}


SignNode* Shingle::computeShingle(Node* node)
{
    if (node->edgesSize() == 0 || node->edgesSize() < minAdyNodes) return nullptr;
    
    uInt shingleID;
    uInt shingleHash;
    SignNode* s = new SignNode();
    s->ptrNode = node;
    //node->setModified(false);

    for (size_t i = 0; i < num_signatures; i++){
        s->minHash.push_back(prime);
    }

    if(node->isWeighted()){
        for (auto adjacent = node->wAdjacentsBegin(); adjacent != node->wAdjacentsEnd(); adjacent++){
            string shingle_ = to_string((*adjacent).first) + "," + to_string((*adjacent).second);
            shingleID = hash_nodes(shingle_);
            for (uint16_t k = 0; k < num_signatures; k++){
                shingleHash = (A[k] * shingleID + B[k]) % prime;
                if (shingleHash < s->minHash[k]){
                    s->minHash[k] = shingleHash;
                }
            }
        }
    } else {
        for (auto adjacent = node->adjacentsBegin(); adjacent != node->adjacentsEnd(); adjacent++) {
            string shingle_ = to_string(*adjacent);
            shingleID = hash_nodes(shingle_);
            for (uint16_t k = 0; k < num_signatures; k++) {
                shingleHash = (A[k] * shingleID + B[k]) % prime;
                if (shingleHash < s->minHash[k]) {
                    s->minHash[k] = shingleHash;
                }
            }
        }
    }

    

    /*
    cout << "Shingle info: " << endl;
    cout << "Node: " << node->getId() << " Ady:" << node->edgesSize() << endl;
     for (size_t i = 0; i < num_signatures; i++){
        cout << "MH_ " << i << ":" << s->minHash[i] << endl;

    }
    */

    return s;
}

