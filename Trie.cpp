#include "Trie.hpp"

// PUBLIC METHODS
template<typename NodeType>
Trie<NodeType>::Trie()
{
    root = NULL;
}

template<typename NodeType>
Trie<NodeType>::~Trie()
{
    if (root != NULL)
        clear(root);
}

template<typename NodeType>
void Trie<NodeType>::create(vector<NodeType *> *nodes)
{
    for (size_t i = 0; i < nodes->size(); i++)
    {
        if (root != NULL){
            if (nodes->at(i)->getFrontAdjacent() != root->vertex){
                continue;
            }
        }
        insert(nodes->at(i));
    }
    //printTrie();
}

template<typename NodeType>
vector<Biclique<NodeType> *> *Trie<NodeType>::getBicliques(){
    vector<Biclique<NodeType>*> *potential_bicliques = new vector<Biclique<NodeType> *>();

    if (root == NULL)
        return potential_bicliques;

    Biclique<NodeType> *b = new Biclique<NodeType>();

    b->S = root->indices;
    for (auto it_node = b->S->begin(); it_node != b->S->end(); it_node++)
        (*it_node)->sort();

    b->C.push_back(root->vertex);

    potential_bicliques->push_back(b);

    computeCandidatesBicliques(root, potential_bicliques);

    if (potential_bicliques->at(0)->C.size() == 1)
        potential_bicliques->erase(potential_bicliques->begin());

    return potential_bicliques;
}
template<typename NodeType>
void Trie<NodeType>::printTrie()
{
    if (root == NULL)
    {
        cout << "root is NULL" << endl;
    }
    else
    {
        cout << "print trie" << endl;
        print(root);
        cout << "finish print" << endl;
    }
}

// PRIVATE METHODS

template<typename NodeType>
void Trie<NodeType>::computeCandidatesBicliques(TrieNode<NodeType> *node, vector<Biclique<NodeType> *> *candidate_bicliques)
{
    uint64_t index = (node->depth - 1) * (node->indices->size() - 1);
    if (index > 0)
    {
        // comprobar en el vector
        if (node->indices->size() != node->parent->indices->size())
        {
            Biclique<NodeType> *b = new Biclique<NodeType>();

            b->S = node->indices;
            for (auto it_node = b->S->begin(); it_node != b->S->end(); it_node++)
                (*it_node)->sort();

            b->C.push_back(node->vertex);
            TrieNode<NodeType> *parent_node = node->parent;
            while (parent_node != NULL)
            {
                b->C.push_back(parent_node->vertex);
                parent_node = parent_node->parent;
            }
            candidate_bicliques->push_back(b);
        }
        else
        {
            Biclique<NodeType> *b = candidate_bicliques->at(candidate_bicliques->size() - 1);
            b->C.push_back(node->vertex);
        }
    }

    if (node->childrens->size() > 0)
    {
        for (size_t i = 0; i < node->childrens->size(); i++)
            computeCandidatesBicliques(node->childrens->at(i), candidate_bicliques);
    }
}
template<typename NodeType>
TrieNode<NodeType> *Trie<NodeType>::find(uInt &vertex, TrieNode<NodeType> *ptr)
{
    if (ptr == NULL)
    {
        return NULL;
    }
    if (ptr->vertex == vertex)
    {
        return ptr;
    }
    for (size_t i = 0; i < ptr->childrens->size(); i++)
    {
        if (ptr->childrens->at(i)->vertex == vertex)
        {
            return ptr->childrens->at(i);
        }
    }
    return NULL;
}


template<typename NodeType>
TrieNode<NodeType> *Trie<NodeType>::find(pair<uInt, uInt> node, TrieNode<NodeType> *ptr)
{
    if (ptr == NULL)
    {
        return NULL;
    }
    if (ptr->vertex == node.first and ptr->weight == node.second)
    {
        return ptr;
    }
    for (size_t i = 0; i < ptr->childrens->size(); i++)
    {
        if (ptr->childrens->at(i)->vertex == node.first and ptr->childrens->at(i)->weight == node.second)
        {
            return ptr->childrens->at(i);
        }
    }
    return NULL;
}

template<typename NodeType>
void Trie<NodeType>::clear(TrieNode<NodeType> *node)
{

    if (node->childrens->size() > 0)
    {
        for (size_t i = 0; i < node->childrens->size(); i++)
        {
            clear(node->childrens->at(i));
        }
    }
    delete node->indices;
    delete node->childrens;
    delete node;
}

template<typename NodeType>
void Trie<NodeType>::insert(NodeType* node) {}

template<>
void Trie<Node>::insert(Node *node)
{
    TrieNode<Node> *t_node;
    TrieNode<Node> *ptr = root;

    // auto node_adjacents = node->getAdjacents(); // modificado
    int i = 0;
    for (auto adj = node->adjacentsBegin(); adj != node->adjacentsEnd(); adj++, i++)
    {
        t_node = find(*adj, ptr);

        if (t_node != NULL)
        { // Si ya existe
            t_node->indices->push_back(node);
        }
        else
        { // No existe -> crear
            t_node = new TrieNode<Node>();
            t_node->indices = new vector<Node *>();
            t_node->childrens = new vector<TrieNode<Node> *>();
            t_node->vertex = *adj;
            t_node->indices->push_back(node);
            t_node->depth = i + 1;

            if (i == 0)
            {
                root = t_node;
                t_node->parent = NULL;
            }
            else
            {
                t_node->parent = ptr;
                ptr->childrens->push_back(t_node);
            }
        }
        ptr = t_node;
    }
}
template<>
void Trie<NodeWeighted>::insert(NodeWeighted *node)
{
    TrieNode<NodeWeighted> *t_node;
    TrieNode<NodeWeighted> *ptr = root;

    // auto node_adjacents = node->getAdjacents(); // modificado
    int i = 0;
    for (auto adj = node->adjacentsBegin(); adj != node->adjacentsEnd(); adj++, i++)
    {
        t_node = find((*adj), ptr);

        if (t_node != NULL)
        { // Si ya existe
            t_node->indices->push_back(node);
        }
        else
        { // No existe -> crear
            t_node = new TrieNode<NodeWeighted>();
            t_node->indices = new vector<NodeWeighted *>();
            t_node->childrens = new vector<TrieNode<NodeWeighted> *>();
            t_node->vertex = (*adj).first;
            t_node->weight = (*adj).second;
            t_node->indices->push_back(node);
            t_node->depth = i + 1;

            if (i == 0)
            {
                root = t_node;
                t_node->parent = NULL;
            }
            else
            {
                t_node->parent = ptr;
                ptr->childrens->push_back(t_node);
            }
        }
        ptr = t_node;
    }
}

template<typename NodeType>
void Trie<NodeType>::print(TrieNode<NodeType> *node)
{
    cout << "Nodo: " << node->vertex << endl;
    cout << "Indices: ";
    for (size_t i = 0; i < node->indices->size(); i++)
    {
        cout << node->indices->at(i)->getId() << " ";
    }
    cout << endl;
    cout << "Hijos de " << node->vertex << endl;
    for (size_t i = 0; i < node->childrens->size(); i++)
    {
        print(node->childrens->at(i));
    }
}

template<>
void Trie<NodeWeighted>::print(TrieNode<NodeWeighted> *node)
{
    cout << "Nodo: " << "(" <<node->vertex << "," << node->weight << ")" << endl;
    cout << "Indices: ";
    for (size_t i = 0; i < node->indices->size(); i++)
    {
        cout << node->indices->at(i)->getId() << " ";
    }
    cout << endl;
    cout << "Hijos de " << node->vertex << endl;
    for (size_t i = 0; i < node->childrens->size(); i++)
    {
        print(node->childrens->at(i));
    }
}
