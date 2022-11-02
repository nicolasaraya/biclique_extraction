#include "BicliqueExtractor.hpp"

BicliqueExtractor::BicliqueExtractor(const string path, uint16_t num_signatures, uint32_t biclique_size){
    this->path = path;
    this->num_signatures = num_signatures;
    this->biclique_size = biclique_size;
    adjMatrixLoaded = false;
    adjMatrixSorted = false;
    adjMatrix = new AdjencyMatrix();
    shingle = new Shingle(num_signatures);
    //vector<Cluster> clusters;
}

BicliqueExtractor::~BicliqueExtractor(){
    delete(adjMatrix);
}

void BicliqueExtractor::makeAdjencyMatrix(){
    ifstream file; 
    file.open(path); 
    if (!file.is_open()){
        cout << "No se puede leer fichero" << endl;
        exit(0);
    }
    string line; 
    getline(file, line); //num nodes
    int countAux =0; 
    while(!file.eof()){
        getline(file, line);
        vector<uint64_t> nodes = splitString(line, " ");
        //cout << "i: " << countAux << " , size: " << nodes.size() << endl; 
        uint64_t nodeID = nodes[0]; 
        sort(nodes.begin(), nodes.end());
        if(nodes.size() > 0) adjMatrix->insert(make_pair(nodeID, nodes)); //push Nodo y Nodos Adyacentes.
        if(countAux++ == 10) break;
    }
    adjMatrixLoaded = true; 
}



void BicliqueExtractor::computeShingles(){

    Node node_;
    for(uint64_t i = 0 ; i < adjMatrix->size() ; i++){
        node_ = adjMatrix->getNode(i);
        shingle->computeShingle(node_.first, node_.second);
    }

}

void BicliqueExtractor::computeShinglesInline(){
    if(adjMatrixLoaded) return; 

}

AdjencyMatrix* BicliqueExtractor::getAdjencyMatrix(){
    return adjMatrix;
}


vector<uint64_t> BicliqueExtractor::splitString(string line, string delims){
    string::size_type bi, ei;
    vector<uint64_t> nodes;
    bi = line.find_first_not_of(delims);     
    while(bi != string::npos) {
  	    ei = line.find_first_of(delims, bi);
    	if(ei == string::npos) ei = line.length();
		string aux = line.substr(bi,ei-bi);
    	nodes.push_back(atoi(aux.c_str()));
    	bi = line.find_first_not_of(delims, ei);
  	}                 
    return nodes;
}