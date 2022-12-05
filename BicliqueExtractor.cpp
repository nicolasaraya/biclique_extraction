#include "BicliqueExtractor.hpp"

////////////////////////////////////////////////////////////////PUBLIC METHODS //////////////////////////////////////////////////////////////////
BicliqueExtractor::BicliqueExtractor(const string path, uint16_t num_signatures, uint32_t biclique_size){
    this->path = path;
    this->num_signatures = num_signatures;
    this->biclique_size = biclique_size;
    adjMatrix = new AdjencyMatrix(path);
    shingle = new Shingle(num_signatures);
    //vector<Cluster> clusters;
}

BicliqueExtractor::~BicliqueExtractor(){
    delete adjMatrix;
    clearSignatures();
    delete shingle;
    for(auto i : clusters) delete i;
}

void BicliqueExtractor::extract(){
    if(!adjencyMatrixLoaded) makeAdjencyMatrix();
    //cout << "Adjency Matrix Size: " << adjMatrix->size() << endl;
    //adjMatrix->print();
    computeShingles();

    //cout << "Compute clusters" << endl;
    computeClusters();
    clearSignatures();
    //cout << "Size of vector Clusters: " << clusters.size() << endl;
    //cout << "Compute Trie" << endl;
    computeTree();
    //cout << "Compute Bilciques" << endl;
    extractBicliques();
    adjMatrix->reWork();
    adjMatrix->print();
    adjMatrix->makeAdjencyList();
    
}

////////////////////////////////////////////////////////////////PRIVATE METHODS //////////////////////////////////////////////////////////////////

bool BicliqueExtractor::compareMinHash(const SignNode* a, const SignNode* b, int signature_index){
    return (a->minHash.at(signature_index) < b->minHash.at(signature_index)); //signNode->vector->minHash
}

void BicliqueExtractor::clearSignatures(){
    for(auto i : signatures){
        delete i;
    }
    signatures.clear();
}

vector<vector<SignNode*>*> BicliqueExtractor::makeGroups(vector<SignNode*>* sign_cluster, int column){
    vector<vector<SignNode*>*> groups;
    vector<SignNode*>* new_group = new vector<SignNode*>();

    uint64_t element = sign_cluster->at(0)->minHash.at(column);
    new_group->push_back(sign_cluster->at(0));

    for(size_t i = 1; i < sign_cluster->size(); i++){
        if(sign_cluster->at(i)->minHash.at(column) != element){
            element = sign_cluster->at(i)->minHash.at(column);
            groups.push_back(new_group);

            new_group = new vector<SignNode*>();
            new_group->push_back(sign_cluster->at(i));
        }
        else{
            new_group->push_back(sign_cluster->at(i));
        }
    }
    groups.push_back(new_group);

    return groups;
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
        Node* temp = new Node();
        getline(file, line);
        temp->adyNodes = splitString(line, " ");
        //cout << "i: " << countAux << " , size: " << nodes.size() << endl; 
        if(temp->adyNodes.size() == 0) {
            delete temp;
            continue;
        }
        temp->nodeID = temp->adyNodes[0]; 
        temp->adyNodes.erase(temp->adyNodes.begin()); //eliminar autociclo
        sort(temp->adyNodes.begin(), temp->adyNodes.end());
        auto aux = find(temp->adyNodes.begin(), temp->adyNodes.end(), temp->nodeID); //busca occurrencia de autociclo 
        if(withAutoCycle && aux != temp->adyNodes.end()){ //si se quiere un autociclo y no se encuentra se agrega
            temp->adyNodes.push_back(temp->nodeID);
            sort(temp->adyNodes.begin(), temp->adyNodes.end());
            temp->autoCycle = true;
        }
        if(temp->adyNodes.size() > 0){ //push Nodo y Nodos Adyacentes.
            adjMatrix->insert(temp); 
        } 
        else{
            delete temp;
        }
        //if(countAux == 100) break;
        countAux++;
        //if(countAux%1000 == 0) cout << countAux <<"Nodos leidos" << endl; //10 nodos
    }
    adjencyMatrixLoaded = true; 
}

void BicliqueExtractor::computeClusters(){
    //printSignatures();
    int size_cluster = computeClusters2(&signatures,0);
    cout << "Clusters encontrados: " << size_cluster << endl;
}


int BicliqueExtractor::computeClusters2(vector<SignNode*>* sign_cluster,int column){
    int size_cluster = 0;
    sortSignatures(sign_cluster,column);
    
    vector<vector<SignNode*>*> groups = makeGroups(sign_cluster,column);

    for(size_t i = 0 ; i < groups.size(); i++){
        uint32_t numberEntries = groups[i]->size();

        if( numberEntries > minClusterSize && column < num_signatures-1){
            size_cluster += computeClusters2(groups[i],column+1);
        }
        else if(numberEntries > minClusterSize ){
            size_cluster++;
            vector< Node* >* new_cluster = new vector<Node*>(); 
            for(size_t j = 0; j < groups[i]->size(); j++){
                new_cluster->push_back(groups[i]->at(j)->ptrNode);
            }

            Cluster *c = new Cluster(new_cluster);
            clusters.push_back(c);
        }
        delete groups[i];
    }
    if(size_cluster == 0){
        size_cluster++;
        vector< Node* >* new_cluster = new vector<Node*>(); 
        for(size_t j = 0; j < sign_cluster->size(); j++){
            new_cluster->push_back(sign_cluster->at(j)->ptrNode);
        }

        Cluster *c = new Cluster(new_cluster);
        clusters.push_back(c);
    }
    return size_cluster;
    
}

void BicliqueExtractor::computeTree(){
    //omp_set_num_threads(NUM_THREADS);
    int counter = 0;
    TIMERSTART(create_trie);

    //#pragma omp parallel for
    for(auto i : clusters){
        counter++;
        //cout << counter << endl;
        i->computeTrie();
        
    }
    
    
    TIMERSTOP(create_trie);
    return;
}

void BicliqueExtractor::computeShingles(){
    Node* node_;
    for(uint64_t i = 0 ; i < adjMatrix->size() ; i++){
        //cout <<endl << i ;
        node_ = adjMatrix->getNode(i);
        SignNode* sn; 
        if(node_ != NULL) sn = shingle->computeShingle(adjMatrix->getNode(i));
        if(sn != NULL) signatures.push_back(sn);
    }
    //printSignatures();

}

void BicliqueExtractor::extractBicliques(){
    vector<Biclique> bicliques;
    for(size_t i = 0; i < clusters.size(); i++){
        bicliques.push_back(clusters[i]->getBiclique());
        vector<uint64_t*>* C = &bicliques.at(i).second;
        vector<Node*>* S = bicliques.at(i).first;

        for(size_t j = 0; j < C->size(); j++){
            //cout << "C: " << *(C->at(j)) << endl;
            for(size_t k = 0; k < S->size(); k++){
                //cout <<"C: " <<  *(C->at(j)) << endl;
                vector<uint64_t>* t_adyNodes = &(S->at(k)->adyNodes);
                auto temp = find(t_adyNodes->begin(), t_adyNodes->end(), *(C->at(j))); //buscamos el elemento de C en la lista del Nodo
                if(temp != S->at(k)->adyNodes.end()){ //si se encuentra se elimina
                    S->at(k)->adyNodes.erase(temp);
                }
            }
        }
        delete clusters[i]; 
    }
    clusters.clear();
}


void BicliqueExtractor::printSignatures(){
    for(auto i : signatures){
        cout << i->ptrNode->nodeID ;
        for(size_t j = 0; j < i->minHash.size(); j++){
            cout << " | MH[" << j << "]: " << i->minHash.at(j); 
        }
        cout << endl;
    }
}

void BicliqueExtractor::printSignatures2(vector<SignNode*> signatures_){
    for(auto i : signatures_){
        cout << i->ptrNode->nodeID ;
        for(int j = 0; j < num_signatures; j++){
            cout << " | MH[" << j << "]: " << i->minHash.at(j);
        }
        cout << endl;
    }
}

void BicliqueExtractor::sortSignatures(vector<SignNode*>* signs, int signature_index){
    sort(signs->begin(), signs->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}
