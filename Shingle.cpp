#include "Shingle.hpp"

using namespace std;

Shingle::Shingle(uint16_t num_signatures, uint32_t minAN, uint32_t shingle_size) : 
    num_signatures(num_signatures), 
    minAdyNodes(minAN),
    shingle_size(shingle_size)
{
    srand(time(NULL));
    if (DEBUG_LEVEL > 1) cout << "Prime " << prime << endl;
    for (size_t i = 0; i < num_signatures; i++)
    {
        A.push_back(rand() % prime + 1);
        B.push_back(rand() % prime + 1);

        if(DEBUG_LEVEL > 1) cout << A[i] << " " << B[i] << endl;
    }
}
Shingle::~Shingle()
{
    A.clear();
    B.clear();
}

SignNode *Shingle::computeShingle(Node* node)
{

    if (node->getAdjacents().size() == 0 || node->getAdjacents().size() < minAdyNodes){
        return nullptr;
    }

    SignNode *s = new SignNode();
    s->ptrNode = node;

    for (size_t i = 0; i < num_signatures; i++)
        s->minHash.push_back(prime);

    uint64_t shingleID;
    uint64_t shingleHash;

    for (auto i = node->getAdjacents().begin(); i != node->getAdjacents().end(); i++) {
        string shingle_ = to_string(*i);
        shingleID = hash_nodes(shingle_);

        for (uint16_t k = 0; k < num_signatures; k++){
            shingleHash = (A[k] * shingleID + B[k]) % prime;
            // cout << "shingle: " << shingleHash << " / numero hasheado: " << ady_nodes->at(i) <<"," << _node->first <<endl;
            if (shingleHash < s->minHash[k]) {
                s->minHash[k] = shingleHash;
            }
        }
    }
    // cout << MIN.size() << endl;

    return s;
}
