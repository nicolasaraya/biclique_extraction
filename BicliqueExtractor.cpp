#include "BicliqueExtractor.hpp"

BicliqueExtractor::BicliqueExtractor(const string path, uint16_t num_signatures, uint16_t minClusterSize , uint16_t minAdyNodes ,uint32_t biclique_size, uint16_t bs_decrease, uint32_t shingleSize) :
    path(path),
    num_signatures(num_signatures),
    minClusterSize(minClusterSize),
    biclique_size(biclique_size),
    minAdyNodes(minAdyNodes),
    bs_decrease(bs_decrease), 
    shingleSize(shingleSize)

{
    adjMatrix = new AdjacencyMatrix(path, selfLoop);
    if (DEBUG_LEVEL > 2) adjMatrix->print();
}

BicliqueExtractor::~BicliqueExtractor()
{
    delete adjMatrix;
}

void BicliqueExtractor::extract()
{
    assert(adjMatrix); 

    uint64_t adjacencyMatrixOriginalSize = adjMatrix->size();
    uint64_t adjacencyMatrixOriginalEdgesSize = adjMatrix->all_edges_size();
    
    ofstream file;
    file.open("log.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero log
    file.close();
    file.open(path + "_bicliques.txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero bicliques y se cierra
    file.close();

    TIMERSTART(extraction_biclique);
    while (iteration++) {
        cout << "Iteracion: " << iteration << endl;

        auto signatures = computeShingles();

        if(signatures->empty()){
            break;
        }

        computeClusters(signatures, 0);

        delete(signatures);

        computeTree();

        uint32_t n_bicliques = extractBicliques();
        total_biclique += n_bicliques;

        file.open("log.txt",fstream::app);
        file << "****************************************************************" << endl;
        file << "Adjacency Matrix Size: " << adjMatrix->size() << endl;
        file << "Iteracion: " << iteration << endl;
        file << "Min Bilcique Size: " << biclique_size << endl;
        file << "Clusters encontrados: " << clusters.size() << endl;
        file << "Bilciques encontrados: " << n_bicliques << endl;
        file.close();

        adjMatrix->restoreNodes();

        for(auto i : clusters) delete i;
        clusters.clear();

        if(n_bicliques < biclique_size){
            biclique_size -= bs_decrease;
            if(biclique_size < 99) break;
        }

        if(DEBUG_LEVEL > 3) break;
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

    adjMatrix->writeAdjacencyList();


}


vector<Signatures*> BicliqueExtractor::makeGroups(Signatures* sign_cluster, int column)
{
    vector<Signatures*> groups;
    Signatures* new_group = new Signatures();

    auto signNode = sign_cluster->at(0);
    auto minHash = signNode->minHash;
    uint64_t element = minHash.at(column);
    new_group->push_back(sign_cluster->at(0));

    for(size_t i = 1; i < sign_cluster->size(); i++){
        auto signNode_i = sign_cluster->at(i);
        auto minHash_i = signNode_i->minHash;
        auto element_i = minHash.at(column);
        if(element_i != element){
            element = element_i;
            groups.push_back(new_group);

            new_group = new Signatures();
            new_group->push_back(signNode_i);
        }
        else{
            new_group->push_back(signNode_i);
        }
    }
    groups.push_back(new_group);

    return groups;
}


void BicliqueExtractor::computeClusters(Signatures* sign_cluster, unsigned int column)
{
    sortSignatures(sign_cluster,column);
    vector<Signatures*> groups = makeGroups(sign_cluster,column);

    auto new_cluster_single_elements = new vector<Node*>(); 

    for(size_t i = 0 ; i < groups.size(); i++){
        uint32_t numberEntries = groups[i]->size();

        if( numberEntries > minClusterSize && (int)column < num_signatures-1){
            computeClusters(groups[i], column+1);
        } else if (numberEntries > 1 ) {
            auto new_cluster = new vector<Node*>(); 
            
            for (size_t j = 0; j < groups[i]->size(); j++) {
                new_cluster->push_back(groups[i]->at(j)->ptrNode);
            }

            auto c = new Cluster(new_cluster);
            clusters.push_back(c);
        } else if(numberEntries == 1) {
            new_cluster_single_elements->push_back(groups[i]->at(0)->ptrNode);
        }
        delete groups[i];
    }
    
    if (new_cluster_single_elements->size() > 1) {
        Cluster *c = new Cluster(new_cluster_single_elements);
        clusters.push_back(c);
    } else {
        delete new_cluster_single_elements;
    }
}

void BicliqueExtractor::computeTree()
{
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

Signatures* BicliqueExtractor::computeShingles()
{
    auto shingle = new Shingle(num_signatures, minAdyNodes, shingleSize);

    Signatures* sg = new Signatures();

    for(auto i = adjMatrix->begin() ; i != adjMatrix->end() ; i++){
        SignNode* sn = shingle->computeShingle(*i);
        if(sn != nullptr) sg->push_back(sn);
        else delete sn;
    }

    if(DEBUG_LEVEL > 2){
        printSignatures(sg);
    }
    delete shingle;
    return sg;
}

uint32_t BicliqueExtractor::extractBicliques()
{
    ofstream file;
    uint32_t n_bicliques = 0;
  	//file.open(name+"_bicliques-"+to_string(iteration)+".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    file.open(path + "_bicliques.txt", fstream::app);
    for(size_t i = 0; i < clusters.size(); i++){
        vector<Biclique*> vector_btemp = clusters[i]->getBicliques();
        if(vector_btemp.empty()) continue;

        vector<uint64_t> modified_nodes;
        vector<uint64_t>::iterator m_n_iterator;

        while(!vector_btemp.empty()){
            //sort by size/rank
            sortBicliques(&vector_btemp);

            // se elije el con mayor rank
            Biclique* btemp = vector_btemp.at(0);

            vector<Node*>* S = btemp->first;
            vector<uint64_t>* C = &btemp->second;

            // si el biclique no cumple con el tamaño deseado o es 2x2, significa que no hay mas bicliques que cumplan la condicion
            if( (S->size()*C->size() < biclique_size ) || ( (S->size()-1) * (C->size()-1) == 1 )){
                for(size_t j = 0; j < vector_btemp.size(); j++){
                    btemp = vector_btemp.at(j);
                    delete btemp;
                }
                vector_btemp.clear();
                continue;
            }
            else n_bicliques+=1;

            // se comienza a extraer el biclique 
            biclique_s_size += S->size();
            biclique_c_size += C->size();
            biclique_sxc_size+= S->size() * C->size();

            sort(C->begin(), C->end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2 ));
            sort(S->begin(), S->end(), bind(&BicliqueExtractor::sortNodes, this, placeholders::_1, placeholders::_2));

            file << "S: ";
            for(size_t j = 0; j < S->size(); j++){
                file << S->at(j)->getId(); 
                if(j != S->size()-1) file << " ";

                modified_nodes.push_back(S->at(j)->getId());
            }
            file << endl << "C: ";

            for(size_t j = 0; j < C->size(); j++){
                file << C->at(j);
                if(j != C->size()-1) file << " ";
                for(size_t k = 0; k < S->size(); k++){
                    //cout <<"C: " <<  *(C->at(j)) << endl;
                    auto t_AdjNodes = S->at(k)->getAdjacents();
                    //vector<uint64_t>* t_adyNodes = &(S->at(k)->adyNodes);
                    auto temp = std::find(t_AdjNodes.begin(), t_AdjNodes.end(),C->at(j)); //buscamos el elemento de C en la lista del Nodo
                    if(temp != t_AdjNodes.end()){ //si se encuentra se elimina
                        t_AdjNodes.erase(temp);
                    }
                }
            }
            file << endl;

            delete btemp;

            vector_btemp.erase(vector_btemp.begin());

            // se limpian el resto de bicliques
            Biclique* btemp_to_erase;
            vector<Node*>* S_to_erase;
            vector<uint64_t>* C_to_erase;

            for(size_t j = 0; j < vector_btemp.size(); j++){
                btemp_to_erase = vector_btemp.at(j);

                S_to_erase = btemp_to_erase->first;
                C_to_erase = &btemp_to_erase->second;

                for(size_t k = 0; k < S_to_erase->size(); k++){
                    m_n_iterator = find(modified_nodes.begin(), modified_nodes.end() ,S_to_erase->at(k)->getId());

                    if( m_n_iterator != modified_nodes.end() ){
                        if( !includes(S_to_erase->at(k)->getAdjacents().begin(),S_to_erase->at(k)->getAdjacents().end(), C_to_erase->begin(), C_to_erase->end() ) ){
                            S_to_erase->erase(S_to_erase->begin()+k);
                            k--;
                        }
                    }
                }
            }
        }
        modified_nodes.clear();
    }
    file.close();
    return n_bicliques;
}


void BicliqueExtractor::printSignatures(Signatures* signatures)
{
    for(auto i : *signatures){
        cout << i->ptrNode->getId() ;
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

void BicliqueExtractor::sortSignatures(Signatures* signs, int signature_index)
{
    sort(signs->begin(), signs->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}

void BicliqueExtractor::sortBicliques(vector<Biclique*>* bicliques)
{
    sort(bicliques->begin(), bicliques->end(), bind(&BicliqueExtractor::compareBicliqueRank, this, placeholders::_1, placeholders::_2));
}

bool BicliqueExtractor::sortC(uint64_t a,uint64_t b)
{
    return a < b;
}

bool BicliqueExtractor::sortNodes(Node* a, Node* b)
{
    return a->getId() < b->getId();
}

bool BicliqueExtractor::compareMinHash(const SignNode* a, const SignNode* b, int signature_index)
{
    return (a->minHash.at(signature_index) < b->minHash.at(signature_index)); //signNode->vector->minHash
}

bool BicliqueExtractor::compareBicliqueRank(const Biclique* a, const Biclique* b)
{
    return a->first->size()*a->second.size() > b->first->size()*b->second.size();
}
