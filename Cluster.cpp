#include "Cluster.hpp"

// PUBLIC METHODS
Cluster::Cluster(vector<Node *> *entry)
{
    t = new Trie();
    nodes = entry;
}
Cluster::~Cluster()
{
    mapFrecuency.clear();
    delete t;
    delete nodes;
}

void Cluster::computeHistogram()
{
    computeFrecuency();

    for (auto node : *nodes)
    {
        auto node_getAdjacents = node->getAdjacents();

        node->sortByFrecuency(&mapFrecuency);

        node->moveToCache(&mapFrecuency, minFreq);
    }
    sort(nodes->begin(), nodes->end(), bind(&Cluster::sortSizeComp, this, placeholders::_1, placeholders::_2));
}

void Cluster::computeTrie()
{
    // printCluster();

    computeHistogram();
    mapFrecuency.clear();
    // printCluster();
    t->create(nodes);

    // t->printTrie();
}

vector<Biclique *> Cluster::getBicliques()
{
    return t->getBicliques();
}

void Cluster::printCluster()
{
    cout << endl
         << "***************" << endl;
    for (size_t i = 0; i < nodes->size(); i++)
    {
        nodes->at(i)->print();
    }
    cout << "***************" << endl
         << endl;
}

// PRIVATE METHODS

bool Cluster::sortSizeComp(Node *a, Node *b)
{
    if (a->edgesSize() > b->edgesSize()) // modificado
    {
        return true;
    }
    else if (a->edgesSize() == b->edgesSize()) // modificado
    {
        return (a->getId() < b->getId());
    }
    else
        return false;
}

void Cluster::computeFrecuency()
{
    for (auto node : *nodes)
    {
        for (auto j : node->getAdjacents())
        {
            auto aux = mapFrecuency.find(j);
            if (aux != mapFrecuency.end())
            { // existe
                mapFrecuency[j]++;
            }
            else
            {
                mapFrecuency[j] = 1; // insert freq 1;
            }
        }
    }
    // printMap();
}

void Cluster::printMap()
{
    cout << "********MAP**********" << endl;
    for (auto i : mapFrecuency)
    {
        cout << i.first << ", freq: " << i.second << endl;
    }
}
