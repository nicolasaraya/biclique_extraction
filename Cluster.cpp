#include "Cluster.hpp"

////////////////////////////////////////////////////////////////PUBLIC METHODS //////////////////////////////////////////////////////////////////
Cluster::Cluster(vector<Node*>* entry){
    t = new Trie();
    nodes = entry;
}
Cluster::~Cluster(){
    mapFrecuency.clear();
    delete t;
    delete nodes;
}

void Cluster::computeHistogram(){
    computeFrecuency();

    for(auto i : *nodes){
        //vector<uint64_t>* adyNodes = &i->second;

        sort(i->getAdjacents().begin(), i->getAdjacents().end(), bind(&Cluster::sortFrecuencyComp, this, placeholders::_1, placeholders::_2));
        

        for(auto j = i->getAdjacents().end() ; j != i->getAdjacents().begin(); j--){ //eliminar freq 1
            if(mapFrecuency[ *j ] <= minFreq){
                i->moveToCache(*j);
                /*
                i->cacheAdyNodes.push_back(i->adyNodes.at(j)); //backup de los nodos descartados.
                i->adyNodes.pop_back();
                //if(i->adyNodes.size() == 0) break;
                */
            }
            else break;
        }
    }
    sort(nodes->begin(), nodes->end(), bind(&Cluster::sortSizeComp, this, placeholders::_1, placeholders::_2));
}

void Cluster::computeTrie(){
    //printCluster();

    computeHistogram();
    mapFrecuency.clear();
    //printCluster();
    t->create(nodes);
    
    //t->printTrie();
}

vector<Biclique*> Cluster::getBicliques(){
    return t->getBicliques();
}


void Cluster::printCluster(){
    cout << endl << "***************" << endl;
    for(size_t i = 0; i < nodes->size(); i++){
        cout << nodes->at(i)->getId() << ": ";
        for(auto j = nodes->at(i)->getAdjacents().begin(); j != nodes->at(i)->getAdjacents().end(); j++){
            cout << *j /*<< "(" << mapFrecuency[nodes->at(i)->second[j] ] << ")"*/<< " ";
        }
        cout << endl;
    }
    cout << "***************" << endl << endl;
}

////////////////////////////////////////////////////////////////PRIVATE METHODS //////////////////////////////////////////////////////////////////
bool Cluster::sortFrecuencyComp(const uint64_t& a, const uint64_t& b ){
    if(mapFrecuency[a] > mapFrecuency[b]){
        return true; 
    }
    else if(mapFrecuency[a] == mapFrecuency[b]){
        return a < b;
    }
    else{
        return false;
    }
}

bool Cluster::sortSizeComp(Node* a, Node* b){
    if(a->getAdjacents().size() > b->getAdjacents().size()){
        return true; 
    }
    else if(a->getAdjacents().size() == b->getAdjacents().size() ){
        return (a->getId() < b->getId());
    }
    else return false;
}

void Cluster::computeFrecuency(){
    for(auto i : *nodes){
        //vector<uint64_t>* adyNodes = &i->second;

        for(auto j : i->getAdjacents()){
            auto aux = mapFrecuency.find(j);
            if(aux != mapFrecuency.end()){ //existe
                mapFrecuency[j]++;
            } 
            else{
                mapFrecuency[j] = 1; //insert freq 1;
            }
        }

    }
    /*
    cout << "********MAP**********" << endl;
    for(auto i : mapFrecuency){
        cout << i.first << ", freq: " << i.second << endl;
    }
    */
}
