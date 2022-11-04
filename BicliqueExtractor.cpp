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
        if(nodes.size() == 0) continue;
        uint64_t nodeID = nodes[0]; 
        //sort(nodes.begin(), nodes.end());
        if(nodes.size() > 0){
            Node* aux = new Node(nodeID, nodes);
            adjMatrix->insert(aux); 
        } //push Nodo y Nodos Adyacentes.
        if(countAux++ == 10) break;
        //countAux++;
        //if(countAux++%1000 == 0) cout << countAux <<"Nodos leidos" << endl; //10 nodos
    }
    cout << "Nodos Totales: " << countAux << endl;
    adjMatrixLoaded = true; 
}



void BicliqueExtractor::computeShingles(){
    Node* node_;
    for(uint64_t i = 0 ; i < adjMatrix->size() ; i++){
        //cout <<endl << i ;
        node_ = adjMatrix->getNode(i);
        SignNode* sn; 
        if(node_ != NULL) sn = shingle->computeShingle(node_);
        if(sn != NULL) signatures.push_back(sn);
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


bool BicliqueExtractor::compareMinHash(const SignNode* a, const SignNode* b, int signature_index){
    return (a->second.at(signature_index) < b->second.at(signature_index)); //signNode->vector->minHash
}

void BicliqueExtractor::sortSignatures(vector<SignNode*>* signs, int signature_index){
    sort(signs->begin(), signs->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}

void BicliqueExtractor::computeClusters(){

    Clusters.clear();
    /*sortSignatures(&signatures, 0);

    SignNode* init = signatures[0];
    vector<SignNode*>* miniCluster = new vector<SignNode*>();
    miniCluster->push_back(init);

    for(size_t j = 1; j < signatures.size(); j++){
        if(signatures[j]->second.at(0) == miniCluster->at(0)->second.at(0)){//si el minHash es igualk
            miniCluster->push_back(signatures[j]);
        }
        else{
            Clusters.push_back(miniCluster);
            miniCluster =  new vector<SignNode*>();
            miniCluster->push_back(signatures[j]);
        }
    }
    Clusters.push_back(miniCluster);*/
    Clusters.push_back(&signatures);
    vector< vector<SignNode*>* > ClustersChild; 
    
    for(size_t i = 0; i < num_signatures; i++){

        for(size_t j = 0; j < Clusters.size(); j++){
            vector<SignNode*>* subSignatures = Clusters[j];
            sortSignatures(subSignatures, i);

            SignNode* init = subSignatures->at(0); //primer SignNode
            vector<SignNode*>* miniCluster = new vector<SignNode*>();
            miniCluster->push_back(init);

            for(size_t k = 1; k < subSignatures->size(); k++){
                if(subSignatures->at(k)->second.at(i) == miniCluster->at(0)->second.at(i)){
                    miniCluster->push_back(subSignatures->at(k));
                }
                else{
                    if(miniCluster->size() > minClusterSize) ClustersChild.push_back(miniCluster); 
                    miniCluster =  new vector<SignNode*>();
                    miniCluster->push_back(subSignatures->at(k));
                                            //vector<signNode>*
                                                //SignNode*
                                                    //vector<uint64_t> MIN
                                                        //i esimo hash
                }
            }
            if(miniCluster->size() > minClusterSize) ClustersChild.push_back(miniCluster); 
        }
        Clusters = ClustersChild;
        ClustersChild.clear();
        
    }
    for(auto i : Clusters){
        //printSignatures2(*i);

    }
    cout << "Clusters encontrados: " << Clusters.size() << endl;
    
}

void BicliqueExtractor::printSignatures(){
    for(auto i : signatures){
        cout << i->first->first << " | MH[0]: " << i->second.at(0) << " | MH[1]: " << i->second.at(1) <<  " | MH[2]: " << i->second.at(2) << endl;
    }
}

/*
void BicliqueExtractor::computeClusters(){
    cout << "Computando clusters" << endl;
    computeClusters2(signatures,0);
    cout << "Numero de Clusters: " << numb_clusters << endl;
}
*/

void BicliqueExtractor::saveCluster(){
    ofstream file; 
    string pathOut = path + "_cluster";
    file.open(pathOut,  std::ofstream::out | std::ofstream::trunc);

    for(auto i : Clusters){
        for(size_t j = 0; j < i->size(); j++){
            SignNode* aux = i->at(j);
            Node* n = aux->first;

            file << n->first << ": " ;
            for(auto k : n->second){
                file << k << " ";
            } 
            file << endl;
        }
        file << "**************" << endl;
    }
    file.close();
    
}

void BicliqueExtractor::computeClusters2(vector<SignNode*> sign_cluster,int column){

    sortSignatures(&sign_cluster,column);

    vector<uint64_t> positions = make_positions_Groups(sign_cluster,column);
    for(uint64_t i = 0 ; i < positions.size() - 1; i++){
        vector<SignNode*> new_sign_cluster = makeGroups(sign_cluster, positions[i] , positions[i+1] );
        int numberEntries = new_sign_cluster.size() / (num_signatures+1);
        if( numberEntries > 10 && column < num_signatures){
            computeClusters2(new_sign_cluster,column+1);
        }
        else if(numberEntries > 1 ){
            numb_clusters++;
        }
        else if(numberEntries == 1){
            numb_clusters++;
        }
            
    }
}

vector<uint64_t> BicliqueExtractor::make_positions_Groups(vector<SignNode*> sign_cluster,int column){
    vector<uint64_t> positions;

    uint64_t element = sign_cluster[0]->second.at(column);
    positions.push_back(0);

    for(size_t i = 1; i < sign_cluster.size(); i++){
        if(sign_cluster[i]->second.at(column) != element){
            element = sign_cluster[i]->second.at(column);
            positions.push_back(i);
        }
    }
    positions.push_back(sign_cluster.size());
    return positions;
}

vector<SignNode*> BicliqueExtractor::makeGroups(vector<SignNode*> sign_cluster, uint64_t pos1 , uint64_t pos2 ){
    vector<SignNode*> group;

    for(uint64_t i = pos1 ;  i < pos2; i++){
        group.push_back( sign_cluster[i] );
    }

    return group;
}

void BicliqueExtractor::printSignatures2(vector<SignNode*> signatures_){
    for(auto i : signatures_){
        cout << i->first->first ;
        for(int j = 0; j < num_signatures; j++){
            cout << " | MH[" << j << "]: " << i->second.at(j);
        }
        cout << endl;
    }
}
