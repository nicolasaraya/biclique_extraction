#include "Trie.hpp"

// PUBLIC METHODS

Trie::Trie()
{
    root = NULL;
}

Trie::~Trie()
{
    if (root != NULL)
        clear(root);
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

vector<Biclique *> *Trie::getBicliques()
{
    vector<Biclique *> *potential_bicliques = new vector<Biclique *>();

    if (root == NULL)
        return potential_bicliques;

    Biclique *b = new Biclique();

    b->first = root->indices;
    for (auto it_node = b->first->begin(); it_node != b->first->end(); it_node++)
        (*it_node)->sort();

    b->second.push_back(root->vertex);

    potential_bicliques->push_back(b);

    computeCandidatesBicliques(root, potential_bicliques);

    if (potential_bicliques->at(0)->second.size() == 1)
        potential_bicliques->erase(potential_bicliques->begin());

    return potential_bicliques;
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

void Trie::computeCandidatesBicliques(TrieNode *node, vector<Biclique *> *candidate_bicliques)
{
    uint64_t index = (node->depth - 1) * (node->indices->size() - 1);
    if (index > 0)
    {
        // comprobar en el vector
        if (node->indices->size() != node->parent->indices->size())
        {
            Biclique *b = new Biclique();

            b->first = node->indices;
            for (auto it_node = b->first->begin(); it_node != b->first->end(); it_node++)
                (*it_node)->sort();

            b->second.push_back(node->vertex);
            TrieNode *parent_node = node->parent;
            while (parent_node != NULL)
            {
                b->second.push_back(parent_node->vertex);
                parent_node = parent_node->parent;
            }
            candidate_bicliques->push_back(b);
        }
        else
        {
            Biclique *b = candidate_bicliques->at(candidate_bicliques->size() - 1);
            b->second.push_back(node->vertex);
        }
    }

    if (node->childrens->size() > 0)
    {
        for (size_t i = 0; i < node->childrens->size(); i++)
            computeCandidatesBicliques(node->childrens->at(i), candidate_bicliques);
    }
}

TrieNode *Trie::find(uInt &vertex, TrieNode *ptr)
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
