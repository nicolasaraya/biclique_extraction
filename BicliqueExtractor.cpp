#include "BicliqueExtractor.hpp"

////////////////////////////////////////////////////////////////PUBLIC METHODS //////////////////////////////////////////////////////////////////
BicliqueExtractor::BicliqueExtractor(const string path, uint16_t num_signatures, uint16_t minClusterSize , uint16_t minAdyNodes ,uint32_t biclique_size, uint16_t bs_decrease){
    this->path = path;
    this->name = path;
    while(name[name.length()-1]!= '.') name.pop_back();
	name.pop_back();
    this->num_signatures = num_signatures;
    this->biclique_size = biclique_size;

    this->minClusterSize = minClusterSize;
    this->minAdyNodes = minAdyNodes;
    this->bs_decrease = bs_decrease;

    iteration = 1;
    total_biclique = 0;
    biclique_s_size = 0;
    biclique_c_size = 0;
    biclique_sxc_size = 0;

    adjMatrix = new AdjacencyMatrix(name);
}

BicliqueExtractor::~BicliqueExtractor(){
    delete adjMatrix;
}

void BicliqueExtractor::extract(){
    if(!adjacencyMatrixLoaded) makeAdjacencyMatrix();
    uint64_t adjacencyMatrixOriginalSize = adjMatrix->size();
    uint64_t adjacencyMatrixOriginalEdgesSize = adjMatrix->all_edges_size();
    
    ofstream file;
    file.open("log.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero log
    file.close();

    file.open(name+"_bicliques.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero bicliques y se cierra
    file.close();

    TIMERSTART(extraction_biclique);
    while(1){
        cout << "Iteracion: " << iteration << endl;

        shingle = new Shingle(num_signatures, minAdyNodes);
        computeShingles();
        delete shingle;

        uint32_t n_clusters = computeClusters();

        clearSignatures();

        computeTree();

        uint32_t n_bicliques = extractBicliques();

        file.open("log.txt",fstream::app);
        file << "****************************************************************" << endl;
        file << "Adjacency Matrix Size: " << adjMatrix->size() << endl;
        file << "Iteracion: " << iteration << endl;
        file << "Min Bilcique Size: " << biclique_size << endl;
        file << "Clusters encontrados: " << n_clusters << endl;
        file << "Bilciques encontrados: " << n_bicliques << endl;
        file.close();

        adjMatrix->reWork();

        for(auto i : clusters) delete i;
        clusters.clear();

        if(n_bicliques < 1){
            biclique_size -= bs_decrease;
            if(biclique_size < 99) break;
        }
        iteration++;
        total_biclique += n_bicliques;

    }
    
    TIMERSTOP(extraction_biclique);

    file.open("log.txt",fstream::app);
    file << "****************************************************************" << endl;
    file << "Original Size AdjacencyMatrix: " << adjacencyMatrixOriginalSize << endl;
    file << "Current Size AdjacencyMatrix: " << adjMatrix->size() << endl << endl;
    file << "Original Edges Size AdjacencyMatrix: " << adjacencyMatrixOriginalEdgesSize << endl;
    file << "Current Edges Size AdjacencyMatrix: " << adjMatrix->all_edges_size() << endl << endl;
    file << "Number of Bicliques Extracted: " << total_biclique << endl;
    file << "Sum of S: " << biclique_s_size << endl;
    file << "Sum of C: " << biclique_c_size << endl;
    file << "Sum of S + C: " << biclique_s_size + biclique_c_size << endl;
    file << "Sum of Multiplication of S x C: " << biclique_sxc_size << endl;
    file.close();

    adjMatrix->makeAdjacencyList();


}

////////////////////////////////////////////////////////////////PRIVATE METHODS //////////////////////////////////////////////////////////////////


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

void BicliqueExtractor::makeAdjacencyMatrix(){
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
    adjacencyMatrixLoaded = true; 
    file.close();
}

uint32_t BicliqueExtractor::computeClusters(){
    int size_cluster = computeClusters2(&signatures,0);
    return size_cluster;
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
    if(size_cluster == 0 && sign_cluster->size() > minClusterSize){
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
    //int counter = 0;
    TIMERSTART(create_trie);

    //#pragma omp parallel for
    for(auto i : clusters){
        //counter++;
        //cout << counter << endl;
        i->computeTrie();
        
    }
    
    
    TIMERSTOP(create_trie);
    return;
}

void BicliqueExtractor::computeShingles(){
    Node* node_;
    for(uint64_t i = 0 ; i < adjMatrix->size() ; i++){
        node_ = adjMatrix->getNode(i);
        SignNode* sn = NULL; 
        if(node_ != NULL) sn = shingle->computeShingle(adjMatrix->getNode(i));
        if(sn != NULL) signatures.push_back(sn);
    }
}

uint32_t BicliqueExtractor::extractBicliques(){

    ofstream file;
    uint32_t n_bicliques = 0;
  	//file.open(name+"_bicliques-"+to_string(iteration)+".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    file.open(name+"_bicliques.txt", fstream::app);
    for(size_t i = 0; i < clusters.size(); i++){
        Biclique* btemp = clusters[i]->getBiclique();
        if(btemp == NULL) continue;

        vector<uint64_t*>* C = &btemp->second;
        vector<Node*>* S = btemp->first;

        if( S->size()*C->size() < biclique_size ){
            delete btemp;
            continue;
        }
        else n_bicliques+=1;

        biclique_s_size += S->size();
        biclique_c_size += C->size();
        biclique_sxc_size+= S->size() * C->size();

        sort(C->begin(), C->end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2 ));
        sort(S->begin(), S->end(), bind(&BicliqueExtractor::sortNodes, this, placeholders::_1, placeholders::_2));

        file << "S: ";
        for(size_t j = 0; j < S->size(); j++){
            file << S->at(j)->nodeID; 
            if(j != S->size()-1) file << " ";
        }
        file << endl << "C: ";

        for(size_t j = 0; j < C->size(); j++){
            file << *(C->at(j));
            if(j != C->size()-1) file << " ";
            for(size_t k = 0; k < S->size(); k++){
                //cout <<"C: " <<  *(C->at(j)) << endl;
                vector<uint64_t>* t_adyNodes = &(S->at(k)->adyNodes);
                auto temp = find(t_adyNodes->begin(), t_adyNodes->end(), *(C->at(j))); //buscamos el elemento de C en la lista del Nodo
                if(temp != S->at(k)->adyNodes.end()){ //si se encuentra se elimina
                    S->at(k)->adyNodes.erase(temp);
                }
            }
        }
        file << endl;
        delete btemp;
    }
    file.close();
    return n_bicliques;
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


/*
* 
* sort
*
*/
void BicliqueExtractor::sortSignatures(vector<SignNode*>* signs, int signature_index){
    sort(signs->begin(), signs->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}

bool BicliqueExtractor::sortC(uint64_t* a,uint64_t* b){
    return *a < *b;
}

bool BicliqueExtractor::sortNodes(Node* a, Node* b){
    return a->nodeID < b->nodeID;
}

bool BicliqueExtractor::compareMinHash(const SignNode* a, const SignNode* b, int signature_index){
    return (a->minHash.at(signature_index) < b->minHash.at(signature_index)); //signNode->vector->minHash
}
