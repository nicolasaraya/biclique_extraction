#include "Shingle.hpp"
using namespace std;

// PUBLIC METHODS
template<typename NodeType>
Shingle<NodeType>::Shingle(uint16_t num_signatures, uint32_t minAN, uint32_t shingle_size) : num_signatures(num_signatures),
                                                                                   minAdyNodes(minAN),
                                                                                   shingle_size(shingle_size)
{
    srand(time(NULL));
    if (DEBUG_LEVEL > 5)
        cout << "Prime " << prime << endl;
    for (size_t i = 0; i < num_signatures; i++)
    {
        A.push_back(rand() % prime + 1);
        B.push_back(rand() % prime + 1);

        if (DEBUG_LEVEL > 5)
            cout << "A: " << A[i] << " B: " << B[i] << endl;
    }
}
template<typename NodeType>
Shingle<NodeType>::~Shingle()
{
    A.clear();
    B.clear();
}

template<typename NodeType>
SignNode<NodeType>* Shingle<NodeType>::computeShingle(NodeType* node){}

template<>
SignNode<Node>* Shingle<Node>::computeShingle(Node *node)
{

    if (node->edgesSize() == 0 || node->edgesSize() < minAdyNodes)
    {
        return nullptr;
    }
    node->setModified(false);

    SignNode<Node> *s = new SignNode<Node>();
    s->ptrNode = node;

    for (size_t i = 0; i < num_signatures; i++)
        s->minHash.push_back(prime);

    uint64_t shingleID;
    uint64_t shingleHash;

    for (auto adjacent = node->adjacentsBegin(); adjacent != node->adjacentsEnd(); adjacent++)
    {
        string shingle_ = to_string(*adjacent);
        shingleID = hash_nodes(shingle_);

        for (uint16_t k = 0; k < num_signatures; k++)
        {
            shingleHash = (A[k] * shingleID + B[k]) % prime;
            // cout << "shingle: " << shingleHash << " / numero hasheado: " << ady_nodes->at(i) <<"," << _node->first <<endl;
            if (shingleHash < s->minHash[k])
            {
                s->minHash[k] = shingleHash;
            }
        }
    }
    // cout << MIN.size() << endl;

    return s;
}

template<>
SignNode<NodeWeighted>* Shingle<NodeWeighted>::computeShingle(NodeWeighted *node)
{

    if (node->edgesSize() == 0 || node->edgesSize() < minAdyNodes)
    {
        return nullptr;
    }
    node->setModified(false);

    SignNode<NodeWeighted> *s = new SignNode<NodeWeighted>();
    s->ptrNode = node;

    for (size_t i = 0; i < num_signatures; i++)
        s->minHash.push_back(prime);

    uint64_t shingleID;
    uint64_t shingleHash;

    for (auto adjacent = node->adjacentsBegin(); adjacent != node->adjacentsEnd(); adjacent++)
    {
        string shingle_ = to_string((*adjacent).first) + " " + to_string((*adjacent).second);
        //cout << shingle_ << endl;
        shingleID = hash_nodes(shingle_);

        for (uint16_t k = 0; k < num_signatures; k++)
        {
            shingleHash = (A[k] * shingleID + B[k]) % prime;
            // cout << "shingle: " << shingleHash << " / numero hasheado: " << ady_nodes->at(i) <<"," << _node->first <<endl;
            if (shingleHash < s->minHash[k])
            {
                s->minHash[k] = shingleHash;
            }
        }
    }
    // cout << MIN.size() << endl;

    return s;
}

