#include "BicliqueExtractor.hpp"

////////////////////////////////////////////////////////////////PUBLIC METHODS //////////////////////////////////////////////////////////////////
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
    delete adjMatrix;
    for(auto i : signatures) delete i;
    for(auto i : clusters) delete i;
    delete shingle;
}

void BicliqueExtractor::extract(){
    makeAdjencyMatrix();
    cout << getAdjencyMatrix()->size() << endl;
    computeShingles();
    //adjMatrix->print();
    //printSignatures();
    computeClusters();
    //printSignatures();
    computeTree();
    cout <<"********************************" << endl;
    //saveCluster();
    //printSignatures();
}

////////////////////////////////////////////////////////////////PRIVATE METHODS //////////////////////////////////////////////////////////////////

AdjencyMatrix* BicliqueExtractor::getAdjencyMatrix(){
    return adjMatrix;
}

bool BicliqueExtractor::compareMinHash(const SignNode* a, const SignNode* b, int signature_index){
    return (a->second.at(signature_index) < b->second.at(signature_index)); //signNode->vector->minHash
}

vector<vector<SignNode*>*> BicliqueExtractor::makeGroups(vector<SignNode*>* sign_cluster,int column){
    vector< vector<SignNode*> *> groups;
    vector<SignNode*>* new_group = new vector<SignNode*>();

    uint64_t element = sign_cluster->at(0)->second.at(column);
    new_group->push_back(sign_cluster->at(0));

    for(size_t i = 1; i < sign_cluster->size(); i++){
        if(sign_cluster->at(i)->second.at(column) != element){
            element = sign_cluster->at(i)->second.at(column);
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
        getline(file, line);
        vector<uint64_t> nodes = splitString(line, " ");
        //cout << "i: " << countAux << " , size: " << nodes.size() << endl; 
        if(nodes.size() == 0) continue;
        uint64_t nodeID = nodes[0]; 
        if(!withAutoCycle) nodes.erase(nodes.begin()); //eliminar autociclo
        sort(nodes.begin(), nodes.end());
        if(nodes.size() > 0){
            Node* aux = new Node(nodeID, nodes);
            adjMatrix->insert(aux); 
        } //push Nodo y Nodos Adyacentes.
        //if(countAux == 10) break;
        countAux++;
        //if(countAux%1000 == 0) cout << countAux <<"Nodos leidos" << endl; //10 nodos
    }
    adjMatrixLoaded = true; 
}


void BicliqueExtractor::computeClusters(){
    posClusters.clear();
    posClusters.push_back(&signatures);
    vector< vector<SignNode*>* > ClustersChild; 
    
    int countA = 0;
    for(size_t i = 0; i < num_signatures; i++){
        for(size_t j = 0; j < posClusters.size(); j++){
            vector<SignNode*>* subSignatures = posClusters[j];

            if(subSignatures->size() > minClusterSize){
                countA++;
            }   
                

            sortSignatures(subSignatures, i);

            SignNode* init = subSignatures->at(0); //primer SignNode
            vector<SignNode*>* miniCluster = new vector<SignNode*>();
            miniCluster->push_back(init);
             
            for(size_t k = 1; k < subSignatures->size(); k++){
                //signNode -> Node -> vector<uint64>.size
                if(subSignatures->at(k)->first->second.size() < minAdyNodes){ //minima cant de nodos ady.
                    continue;
                }

                if(subSignatures->at(k)->second.at(i) == miniCluster->at(0)->second.at(i)){
                    miniCluster->push_back(subSignatures->at(k));
                }
                else{
                    if(miniCluster->size() > minClusterSize) {
                        if( (int)i != num_signatures-1) ClustersChild.push_back(miniCluster); 
                        else{
                            vector< Node* >* new_cluster = new vector<Node*>(); 
                            for(size_t l = 0; l < miniCluster->size(); l++){
                                new_cluster->push_back(miniCluster->at(l)->first);
                            }
                            Cluster *c = new Cluster(new_cluster);
                            clusters.push_back(c);
                        }
                        //miniCluster = NULL;
                    }
                    else delete miniCluster;
                    miniCluster =  new vector<SignNode*>();
                    miniCluster->push_back(subSignatures->at(k));
                                            //vector<signNode>*
                                                //SignNode*
                                                    //vector<uint64_t> MIN
                                                        //i esimo hash
                }
            }
            if(miniCluster->size() > minClusterSize) {
                if( (int)i != num_signatures-1) ClustersChild.push_back(miniCluster); 
                else{
                    vector< Node* >* new_cluster = new vector<Node*>(); 
                    for(size_t l = 0; l < miniCluster->size(); l++){
                        new_cluster->push_back(miniCluster->at(l)->first);
                    }
                    Cluster *c = new Cluster(new_cluster);
                    clusters.push_back(c);
                }
            }
            else delete miniCluster;

            if(i > 0) delete posClusters[j];
        }
        posClusters = ClustersChild;
        ClustersChild.clear();
    }
    cout << "Clusters encontrados: " << clusters.size() << " + " << countA << endl;
    
}
/*
void BicliqueExtractor::computeClusters(){
    //printSignatures();
    cout << "Computando clusters" << endl;
    computeClusters2(&signatures,0);
    cout << "Size of vector Clusters: " << clusters.size() << endl;
}
*/

void BicliqueExtractor::computeClusters2(vector<SignNode*>* sign_cluster,int column){

    sortSignatures(sign_cluster,column);
    vector<vector<SignNode*>*> groups = makeGroups(sign_cluster,column);

    for(size_t i = 0 ; i < groups.size(); i++){
        uint32_t numberEntries = groups[i]->size();

        if( numberEntries > minClusterSize && column < num_signatures-1){
            computeClusters2(groups[i],column+1);
        }
        else if(numberEntries > minClusterSize ){
            vector< Node* >* new_cluster = new vector<Node*>(); 
            for(size_t j = 0; j < groups[i]->size(); j++){
                new_cluster->push_back(groups[i]->at(j)->first);
            }

            Cluster *c = new Cluster(new_cluster);
            clusters.push_back(c);
        }
        /*
        else if(numberEntries == 1){
            vector< Node* >* new_cluster = new vector<Node*>(); 
            for(uint64_t j = 0; j < groups[i]->size(); j++){
                new_cluster->push_back(groups[i]->at(j)->first);
            }

            Cluster *c = new Cluster(new_cluster);
            clusters.push_back(c);
        }*/

        delete groups[i];
    }
}

void BicliqueExtractor::computeTree(){
    //vector<Cluster*> clusters;
    //for(auto i: clusters){
    //    i->computeHistogram();
    //}
    //clusters[0]->computeHistogram(); 
    //clusters[0]->printCluster();
    omp_set_num_threads(NUM_THREADS);
    int counter = 0;
    TIMERSTART(create_tree);
    /*
    vector<Node*> *aux = new vector<Node*>();
    Node* n1 = new Node;
    vector<uint64_t> arista1{1,2,3,7,8};
    n1->second = arista1; 
    n1->first = 1;

    Node* n2 = new Node;
    vector<uint64_t> arista2{1,2,3,7,8};
    n2->second = arista2;
    n2->first = 2; 

    Node* n3 = new Node;
    vector<uint64_t> arista3{1,2,3,7,8};
    n3->second = arista3;
    n3->first = 3; 

    Node* n5 = new Node;
    vector<uint64_t> arista5{1,2,3,5,7,8};
    n5->second = arista5;
    n5->first = 5; 

    Node* n6 = new Node;
    vector<uint64_t> arista6{1,2,3,6,7,8};
    n6->second = arista6;
    n6->first = 6; 

    Node* n7 = new Node;
    vector<uint64_t> arista7{1,4,7,8};
    n7->second = arista7;
    n7->first = 7; 

    Node* n8 = new Node;
    vector<uint64_t> arista8{1,4,7,8};
    n8->second = arista8;
    n8->first = 8; 

    aux->push_back(n1);
    aux->push_back(n2);
    aux->push_back(n3);
    aux->push_back(n5);
    aux->push_back(n6);
    aux->push_back(n7);
    aux->push_back(n8);
    
    Cluster* c = new Cluster(aux);
    c->computeTree();
    */

    #pragma omp parallel for
    for(auto i : clusters){
        counter++;
        //cout << counter << endl;
        i->computeTree();
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
        if(node_ != NULL) sn = shingle->computeShingle(node_);
        if(sn != NULL) signatures.push_back(sn);
    }

}

void BicliqueExtractor::computeShinglesInline(){
    if(adjMatrixLoaded) return; 
}

void BicliqueExtractor::printSignatures(){
    for(auto i : signatures){
        cout << i->first->first << " | MH[0]: " << i->second.at(0) << " | MH[1]: " << i->second.at(1) <<  " | MH[2]: " << i->second.at(2) << endl;
    }
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

/*
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
}*/

void BicliqueExtractor::sortSignatures(vector<SignNode*>* signs, int signature_index){
    sort(signs->begin(), signs->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}
