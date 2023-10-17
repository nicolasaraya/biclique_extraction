#include "Cluster.hpp"

// PUBLIC METHODS 
Cluster::Cluster(vector<Node*>* entry)
{
    t = new Trie();
    nodes = entry;
    weighted = entry->front()->isWeighted();
}

Cluster::~Cluster()
{
    mapFrecuency.clear();
    delete t;
    delete nodes;
}

void Cluster::printMap()
{
    cout << "********MAP**********" << endl;
    if (weighted) {
        for (auto i : mapFrecuencyWeighted) {
            if (i.second > 2) {
                cout << "(" << i.first << ")" << ", freq: " << i.second << endl;
            }
        }
    } else {
        for (auto i : mapFrecuency) {
            cout << i.first << ", freq: " << i.second << endl;
        }
    }
    
}

void Cluster::computeFrecuency()
{
    if(weighted){
        for (auto node : *nodes) {
            for (auto j = node->wAdjacentsBegin(); j != node->wAdjacentsEnd(); j++) {   
                string node_id = to_string((*j)->first) + "," + to_string((*j)->second); 
                auto aux = mapFrecuencyWeighted.find((node_id));
                if (aux != mapFrecuencyWeighted.end()) { // existe
                    mapFrecuencyWeighted[node_id]++;
                } else {
                    mapFrecuencyWeighted[node_id] = 1; // insert freq 1;
                }
            }
        }
    } else {
        for (auto node : *nodes) {
            for (auto j = node->adjacentsBegin(); j != node->adjacentsEnd(); j++) {   
                auto aux = mapFrecuency.find(*j);
                if (aux != mapFrecuency.end()) { // existe
                    mapFrecuency[*j]++;
                } else {
                    mapFrecuency[*j] = 1; // insert freq 1;
                }
            }
        }
    }   
}


void Cluster::computeHistogram()
{
    computeFrecuency();

    for (auto *node : *nodes) {
        if (weighted) {
            //using std::placeholders::_1;
            //std::function<void()> sortNode = [node, this](){node->sortByFrecuency(&mapFrecuencyWeighted);};
            //std::function<void()> moveNode = [node, this](){node->moveToCache(&mapFrecuencyWeighted, minFreq);};
            //
            //TaskManager::get().submit(moveNode);    

            //auto sortNode = std::bind(static_cast<void(Node::*)(unordered_map<string, uint32_t> *)>(&Node::sortByFrecuency), &node, &mapFrecuencyWeighted);
            //std::function<void()> sortNode = [this, node](){node->sortByFrecuency(&mapFrecuencyWeighted);};
            //TaskManager::get().submit(sortNode);
            node->sortByFrecuency(&mapFrecuencyWeighted);
            node->moveToCache(&mapFrecuencyWeighted, minFreq);
        } else {
            node->sortByFrecuency(&mapFrecuency);
            node->moveToCache(&mapFrecuency, minFreq);
        }
    }
    //cout << "********+ finish push ***********+" << endl;
    std::sort(nodes->begin(), nodes->end(), bind(&Cluster::sortSizeComp, this, placeholders::_1, placeholders::_2));
}

void Cluster::computeTrie()
{
    if (nodes != nullptr) {
        computeHistogram();
        t->create(nodes);
    }
    //mapFrecuency.clear();
    //mapFrecuencyWeighted.clear();
    
}

vector<Biclique*>* Cluster::getBicliques()
{
    return t->getBicliques();
    
}

void Cluster::printCluster()
{
    cout << endl << "***************" << endl;
    for (size_t i = 0; i < nodes->size(); i++) {
        nodes->at(i)->print();
    }
    cout << "***************" << endl << endl;
}
 
bool Cluster::sortSizeComp(Node* a, Node* b)
{
    if (a->edgesSize() > b->edgesSize()) {
        return true;
    } else if (a->edgesSize() == b->edgesSize()) {
        return (a->getId() < b->getId());
    } else {
        return false;
    }
}


