#include "Trie.hpp"

// PUBLIC METHODS

Trie::Trie()
{
    root = NULL;
}

Trie::~Trie()
{
    if (root != NULL)
    {
        clear(root);
    }
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

vector<Biclique *> Trie::getBicliques()
{
    vector<Biclique *> potential_bicliques;

    if (root == NULL)
        return potential_bicliques;

    map<vector<Node *> *, TrieNode *> candidate_bicliques;

    computeCandidatesBicliques(root, &candidate_bicliques);

    map<vector<Node *> *, TrieNode *>::iterator it;

    for (it = candidate_bicliques.begin(); it != candidate_bicliques.end(); it++)
    {
        Biclique *b = new Biclique();
        b->first = it->second->indices;
        b->second.push_back(it->second->vertex);

        TrieNode *parent_node = it->second->parent;
        while (parent_node != NULL)
        {
            b->second.push_back(parent_node->vertex);
            parent_node = parent_node->parent;
        }
        for (auto it_node = b->first->begin(); it_node != b->first->end(); it_node++)
            (*it_node)->sort();
        sort(b->second.begin(), b->second.end());
        potential_bicliques.push_back(b);
    }

    candidate_bicliques.clear();

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

    //auto node_adjacents = node->getAdjacents(); // modificado
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
