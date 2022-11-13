#include "Cluster.hpp"


Cluster::Cluster(vector<Node*>* entry){
    nodes = entry;
}
Cluster::~Cluster(){

}

void Cluster::computeHistogram(){
    printCluster();
    map<uint64_t,vector<uint64_t>> mapVO;
    map<uint64_t,vector<uint64_t>> mapVOut;
    map<uint64_t, vector<uint64_t>>::iterator mit;
    uint64_t vertex;
    uint64_t edgeOfVertex;

    for(uint64_t i = 0; i < nodes->size();i++){
        vertex = nodes->at(i)->first;
        for(uint64_t j = 0; j < nodes->at(i)->second.size();j++){
            vector<uint64_t> vectorEdges;

            edgeOfVertex  = nodes->at(i)->second[j];
            mit = mapVO.find(edgeOfVertex);
            if(mit != mapVO.end()){
                    vectorEdges = mit->second;
            }
            vectorEdges.push_back(vertex);
            mapVO[edgeOfVertex] = vectorEdges;
        
        }
    }

    vector<Node> selectedEdges;
    for(mit=mapVO.begin(); mit != mapVO.end(); mit++){
        vector<uint64_t> vectorEdges = mit->second;
	    if(vectorEdges.size() > 1){
		    sort(vectorEdges.begin(),vectorEdges.end());
		    selectedEdges.push_back(make_pair(mit->first, vectorEdges));
	    }
    }
    cout << "selected edges size: " << selectedEdges.size() << endl;

    sort(selectedEdges.begin(), selectedEdges.end(), edgeCmp());

    for(uint64_t i = 0; i < selectedEdges.size();i++){

        vector<uint64_t> vectorVertex = selectedEdges[i].second;
        uint64_t EdgeVertex = selectedEdges[i].first;

        for(uint64_t j = 0; j < vectorVertex.size();j++){
            vector<uint64_t> vectorEdgeOfVertex;

            mit = mapVOut.find(vectorVertex[j]);
            if(mit != mapVOut.end()){
                vectorEdgeOfVertex = mit->second;
            }
            vectorEdgeOfVertex.push_back(EdgeVertex);
            mapVOut[vectorVertex[j]] = vectorEdgeOfVertex;
        }

    }

    for(mit= mapVOut.begin(); mit != mapVOut.end(); mit++){
        cout<<" vertex "<<mit->first<<": ";
        for(uint64_t j = 0; j < mit->second.size(); j++){
            cout << mit->second[j] << " ";
        }
        cout<<endl;
    }
}

void Cluster::printCluster(){
    for(uint64_t i = 0; i < nodes->size(); i++){
        cout << nodes->at(i)->first << ": ";
        for(uint64_t j = 0; j < nodes->at(i)->second.size(); j++){
            cout << nodes->at(i)->second[j] << " ";
        }
        cout << endl;
    }
}

