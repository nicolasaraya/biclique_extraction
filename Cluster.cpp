#include "Cluster.hpp"

// PUBLIC METHODS
template <typename NodeType> 
Cluster<NodeType>::Cluster(vector<NodeType *> *entry)
{
    t = new Trie<NodeType>();
    nodes = entry;
}

template <typename NodeType> 
Cluster<NodeType>::~Cluster()
{
    mapFrecuency.clear();
    delete t;
    delete nodes;
}

template <typename NodeType>
void Cluster<NodeType>::printMap(){} 

template <> 
void Cluster<Node>::printMap()
{
    cout << "********MAP**********" << endl;
    for (auto i : mapFrecuency)
    {
        cout << i.first << ", freq: " << i.second << endl;
    }
}

template <> 
void Cluster<NodeWeighted>::printMap()
{
    cout << "********MAP**********" << endl;
    for (auto i : mapFrecuencyWeighted)
    {
        if(i.second > 2){
            cout << "(" << i.first << ")" << ", freq: " << i.second << endl;
        }
    }
}


template<typename NodeType>
void Cluster<NodeType>::computeHistogram()
{
    return;
}

template <typename NodeType> 
void Cluster<NodeType>::computeFrecuency()
{   
    //printMap();
}


template<>
void Cluster<Node>::computeFrecuency(){
    for (auto node : *nodes)
    {
        for (auto j = node->adjacentsBegin(); j != node->adjacentsEnd(); j++)
        {   
            auto aux = mapFrecuency.find(*j);
            if (aux != mapFrecuency.end())
            { // existe
                mapFrecuency[*j]++;
            }
            else
            {
                mapFrecuency[*j] = 1; // insert freq 1;
            }
        }
    }
}

template <>
void Cluster<NodeWeighted>::computeFrecuency(){
    for (auto node : *nodes)
    {
        for (auto j = node->adjacentsBegin(); j != node->adjacentsEnd(); j++)
        {   
            string node_id = to_string((*j).first) + "," + to_string((*j).second); 
            auto aux = mapFrecuencyWeighted.find((node_id));
            if (aux != mapFrecuencyWeighted.end())
            { // existe
                mapFrecuencyWeighted[node_id]++;
            }
            else
            {
                mapFrecuencyWeighted[node_id] = 1; // insert freq 1;
            }
        }
    }
    //printMap();

}


template <> 
void Cluster<Node>::computeHistogram()
{
    computeFrecuency();

    for (auto node : *nodes)
    {
        node->sortByFrecuency(&mapFrecuency);
        node->moveToCache(&mapFrecuency, minFreq);
    }
    sort(nodes->begin(), nodes->end(), bind(&Cluster::sortSizeComp, this, placeholders::_1, placeholders::_2));
}
template <> 
void Cluster<NodeWeighted>::computeHistogram()
{
    computeFrecuency();

    for (auto node : *nodes)
    {
        node->sortByFrecuency(&mapFrecuencyWeighted);
        node->moveToCache(&mapFrecuencyWeighted, minFreq);
    }
    sort(nodes->begin(), nodes->end(), bind(&Cluster::sortSizeComp, this, placeholders::_1, placeholders::_2));
}

template <typename NodeType> 
void Cluster<NodeType>::computeTrie()
{
    if(nodes != nullptr){
        computeHistogram();
        t->create(nodes);
    }
    mapFrecuency.clear();
    mapFrecuencyWeighted.clear();
    
}

template <typename NodeType> 
vector<Biclique<NodeType>*>* Cluster<NodeType>::getBicliques()
{
    return t->getBicliques();
}

template <typename NodeType> 
void Cluster<NodeType>::printCluster()
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

template <typename NodeType> 
bool Cluster<NodeType>::sortSizeComp(NodeType *a, NodeType *b)
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


