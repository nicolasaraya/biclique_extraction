#include "Trie.hpp"

// PUBLIC METHODS

Trie::Trie()
{
    root = NULL;
    candidateBiclique = NULL;
}

Trie::~Trie()
{
    if (root != NULL)
    {
        clear(root);
    }
    candidateBiclique = NULL;
}

void Trie::create(vector<Node *> *nodes)
{
    for (size_t i = 0; i < nodes->size(); i++)
    {
        if (root != NULL)
            if (nodes->at(i)->getFrontAdjacent() != root->vertex)
                continue;

        insert(nodes->at(i));
    }
}

Biclique *Trie::getBiclique()
{
    Biclique *b = new Biclique();
    computeCandidateBiclique(root);
    if (candidateBiclique != NULL)
    {
        computeBiclique(b, candidateBiclique);
    }
    else
    {
        delete b;
        return NULL;
    }
    return b;
}

void Trie::computeCandidateBiclique(TrieNode *node)
{
    if (node->childrens->size() > 0)
    {
        for (size_t i = 0; i < node->childrens->size(); i++)
        {
            computeCandidateBiclique(node->childrens->at(i));
        }
    }
    if (candidateBiclique == NULL)
    {
        if ((node->depth - 1) * (node->indices->size() - 1) > 0)
        {
            candidateBiclique = node;
        }
    }
    else if ((candidateBiclique->depth - 1) * (candidateBiclique->indices->size() - 1) < (node->depth - 1) * (node->indices->size() - 1))
    {
        candidateBiclique = node;
    }
}

void Trie::computeBiclique(Biclique *b, TrieNode *node)
{
    if (node == candidateBiclique)
    {
        b->first = node->indices;
    }
    b->second.push_back(node->vertex);

    if (node->parent == NULL)
    {
        return;
    }

    computeBiclique(b, node->parent);
}

void Trie::printTrie()
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

void Trie::computeCandidatesBicliques(TrieNode *node, map<vector<Node *> *, TrieNode *> *candidate_bicliques)
{
    if (node->childrens->size() > 0)
    {
        for (size_t i = 0; i < node->childrens->size(); i++)
        {
            computeCandidatesBicliques(node->childrens->at(i), candidate_bicliques);
        }
    }

    uint64_t index = (node->depth - 1) * (node->indices->size() - 1);
    if (index > 0)
    {
        // comprobar en el vector
        if (candidate_bicliques->find(node->indices) != candidate_bicliques->end())
        {
            uint64_t last_rank = candidate_bicliques->at(node->indices)->depth * node->indices->size();
            uint64_t rank = node->depth * node->indices->size();
            if (rank > last_rank)
            {
                candidate_bicliques->insert(make_pair(node->indices, node));
            }
        }
        else
        {
            candidate_bicliques->insert(make_pair(node->indices, node));
        }
    }
}

TrieNode *Trie::find(uint64_t &vertex, TrieNode *ptr)
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

void Trie::clear(TrieNode *node)
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

void Trie::insert(Node *node)
{
    TrieNode *t_node;
    TrieNode *ptr = root;

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
            t_node = new TrieNode();
            t_node->indices = new vector<Node *>();
            t_node->childrens = new vector<TrieNode *>();
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

void Trie::print(TrieNode *node)
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
