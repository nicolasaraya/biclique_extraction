#include "BicliqueExtractor.hpp"

// PUBLIC METHODS

template <typename GraphType> 
BicliqueExtractor<GraphType>::BicliqueExtractor(
    uint16_t num_signatures, uint16_t minClusterSize, uint16_t minAdyNodes, uint32_t biclique_size, uint16_t bs_decrease, uint32_t shingleSize, bool selfLoop, uint32_t threshold, uint16_t iterations) : num_signatures(num_signatures), minClusterSize(minClusterSize), biclique_size(biclique_size), minAdyNodes(minAdyNodes), bs_decrease(bs_decrease), shingleSize(shingleSize), selfLoop(selfLoop), threshold(threshold), iterations(iterations)
{
    iteration = 1;
    biclique_s_size = 0;
    biclique_sxc_size = 0;
    biclique_c_size = 0;
    total_biclique = 0;
    cluster_size = 0;
    n_bicliques_iter = 0;
}

template <typename GraphType> 
BicliqueExtractor<GraphType>::~BicliqueExtractor() {}

template <typename GraphType> 
void BicliqueExtractor<GraphType>::setGraph(GraphType* g){
    graph = g;
} 

template <typename GraphType> 
void BicliqueExtractor<GraphType>::extract()
{
    assert(graph);

    uint64_t adjacencyMatrixOriginalSize = graph->size();
    uint64_t adjacencyMatrixOriginalEdgesSize = graph->all_edges_size();

    ofstream file;
    file.open("log.txt", std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero log
    file.close();
    string new_path = modify_path(graph->getPath(), 4 ,"bicliques.txt");
    file.open(new_path, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero bicliques y se cierra
    file.close();

    TIMERSTART(extraction_biclique);
    while (true) {
        n_bicliques_iter = 0;
        cout << "Iteration: " << iteration << endl;
        cout << "Compute Shingles" << endl;
        TIMERSTART(compute_shingles);
        auto signatures = computeShingles();
        TIMERSTOP(compute_shingles);
        assert(not signatures->empty());

        cout << "Compute Clusters and Extract Bicliques" << endl;
        TIMERSTART(compute_clusters_and_bicliques);
        sortBySignatures(signatures, 0);
        computeClusters(signatures, 0);
        TIMERSTOP(compute_clusters_and_bicliques);

        for (auto i : *signatures) delete i;
        delete (signatures);

        total_biclique += n_bicliques_iter;

        cout << "biclique iteracion: " << n_bicliques_iter << endl;
        cout << "total: " << total_biclique << endl;

        file.open("log.txt", fstream::app);
        file << "****************************************************************" << endl;
        file << "Iteracion: " << iteration << endl;
        file << "Adjacency Matrix Size: " << graph->size() << endl;
        file << "Current Edges Size AdjacencyMatrix: " << graph->all_edges_size() << endl;
        file << "Min Bilcique Size: " << biclique_size << endl;
        file << "Clusters encontrados: " << cluster_size << endl;
        file << "Bilciques encontrados: " << n_bicliques_iter << endl;
        file.close();

        TIMERSTART(restaure_nodes);
        cout << "Restaurando Nodos" << endl;
        graph->restoreNodes();
        TIMERSTOP(restaure_nodes);

        if (iteration == iterations)
            break;

        if (n_bicliques_iter < threshold)
        {
            cout << n_bicliques_iter << " " << threshold << endl;
            if (bs_decrease > biclique_size)
                break;

            biclique_size -= bs_decrease;
            if (biclique_size < 20)
                break;
        }
        iteration++;
    }

    TIMERSTOP(extraction_biclique);

    file.open("log.txt", fstream::app);
    file << "****************************************************************" << endl;
    file << "Original Size AdjacencyMatrix: " << adjacencyMatrixOriginalSize << endl;
    file << "Current Size AdjacencyMatrix: " << graph->size() << endl
         << endl;
    file << "Original Edges Size AdjacencyMatrix: " << adjacencyMatrixOriginalEdgesSize << endl;
    file << "Current Edges Size AdjacencyMatrix: " << graph->all_edges_size() << endl
         << endl;
    file << "Number of Bicliques Extracted: " << total_biclique << endl;
    file << "Sum of S: " << biclique_s_size << endl;
    file << "Sum of C: " << biclique_c_size << endl;
    file << "Sum of S + C: " << biclique_s_size + biclique_c_size << endl;
    file << "Sum of Multiplication of S x C: " << biclique_sxc_size << endl;
    file.close();

    string path_write = modify_path(graph->getPath(), 4 ,"compressed");
    graph->writeAdjacencyList(path_write);
}

template <typename GraphType>
vector<Signatures*>* BicliqueExtractor<GraphType>::makeGroups(Signatures* group, int column)
{
    vector<Signatures*>* groups = new vector<Signatures*>(); 
    Signatures* partition = new Signatures();
    groups->push_back(partition);

    for (auto signNode = group->begin(); signNode != group->end(); signNode++) {
        if (not partition->empty() and partition->front()->minHash.at(column) != (*signNode)->minHash.at(column)){
            partition = new Signatures();
            groups->push_back(partition);
        }
        partition->push_back(*signNode);
    }
    return groups;
}



template <typename GraphType> 
void BicliqueExtractor<GraphType>::computeClusters(Signatures* group, unsigned int column)
{       
    sortBySignatures(group, column);
    vector<Signatures*>* candidates = makeGroups(group, column);

    if(column == 0) cout << "num candidates: " << candidates->size() << endl;

    vector<Node*>* discarted = new vector<Node*>();

    for (auto cluster = candidates->begin(); cluster != candidates->end(); cluster++){
        //printSignatures(*cluster);
        uInt clusterSize = (*cluster)->size();

        if (clusterSize >= minClusterSize and (int)column < num_signatures - 1) {
            computeClusters(*cluster, column+1);
        } else if (clusterSize > 1) {   
            auto newCluster = new vector<Node*>();
            for (auto node = (*cluster)->begin(); node != (*cluster)->end(); node++) {
                newCluster->push_back((*node)->ptrNode);
            }        
            extractBicliques(new Cluster(newCluster)); 
        } else if (clusterSize == 1) {
            discarted->push_back((*cluster)->front()->ptrNode);
        }
        delete *cluster;
    }
    if (discarted->size() > 1) {
        extractBicliques(new Cluster(discarted));
    } else {
        delete discarted;
    }
    delete candidates;
}


template <typename GraphType> 
void BicliqueExtractor<GraphType>::extractBicliques(Cluster* c)
{
    cluster_size++;
    c->computeTrie();
    getBicliques(c);
    delete c;
}

template <typename GraphType> 
Signatures* BicliqueExtractor<GraphType>::computeShingles()
{
    auto shingle = new Shingle(num_signatures, minAdyNodes, shingleSize);
    Signatures* group = new Signatures(); 

    for (auto i = graph->begin(); i != graph->end(); i++) {
        SignNode* sn = shingle->computeShingle(*i);
        if (sn != nullptr) {
            group->push_back(sn);
        } else {
            delete sn;
        }
    }
    delete shingle;
    return group;
}


template <typename GraphType> 
void BicliqueExtractor<GraphType>::getBicliques(Cluster* c)
{   
    vector<Biclique*>* bicliques = c->getBicliques();

    if (bicliques->empty()) {
        delete bicliques;
        return; 
    }

    // cout << n_bicliques_iter << endl;

    for (auto biclique = bicliques->begin(); biclique != bicliques->end(); biclique++) {

        vector<Node*>* S = (*biclique)->S;
        vector<uInt>* C = &(*biclique)->C; 

        if(S->size() * C->size() < biclique_size) {
            delete *biclique;
            continue; 
        }

        n_bicliques_iter++;
        biclique_s_size += S->size();
        biclique_c_size += C->size();
        biclique_sxc_size += S->size() * C->size();

        sort(C->begin(), C->end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2));
        sort(S->begin(), S->end(), bind(&BicliqueExtractor::sortS, this, placeholders::_1, placeholders::_2));

        
        //cout << "S: ";
        for (auto i = S->begin(); i != S->end(); i++) {
            //cout << (*i)->getId() << " "; 
            (*i)->sort();
        }

        /*
        cout << endl << "C: " ; 
        for (auto i : *C) {
            cout << i << " "; 
        }
        cout << endl;

        */

        writeBiclique(S, C);

        for(auto it = S->begin(); it != S->end(); it++){
            (*it)->deleteExtracted(C);
        }

        delete *biclique; 
    }
    delete bicliques;
    return; 
    /*
    while (!possible_bicliques->empty()) {
        // sort by size/rank
        // sortBicliques(possible_bicliques);

        // se elije el biclique con mayor rank
        vector<NodeType*> *S = best_biclique->S;
        vector<uInt> *C = &best_biclique->C;

        // si el biclique no cumple con el tamaño deseado o es 2x2, significa que no hay mas bicliques que cumplan la condicion
        if ((S->size() * C->size() < biclique_size) || ((S->size() - 1) * (C->size() - 1) == 1))
        {

            // cout << "Estoy eliminando posibles blicliques" << endl;

            for (size_t j = 0; j < possible_bicliques->size(); j++)
            {
                best_biclique = possible_bicliques->at(j);
                delete best_biclique;
            }
            possible_bicliques->clear();
            // cout << "Termine de eliminar posibles blicliques" << endl;
            break;
        }
        else
            n_bicliques_iter += 1;

        // cout << "S: " << S->size() << " / C: " << C->size() << endl;

        // se comienza a extraer el biclique
        biclique_s_size += S->size();
        biclique_c_size += C->size();
        biclique_sxc_size += S->size() * C->size();

        sort(S->begin(), S->end(), bind(&BicliqueExtractor::sortS, this, placeholders::_1, placeholders::_2));
        // sort(C->begin(), C->end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2));

        writeBiclique(S, C);

        delete best_biclique;

        possible_bicliques->pop_back();

        // se limpian el resto de bicliques
        Biclique<NodeType> *best_biclique_to_erase;
        vector<NodeType*> *S_to_erase;
        vector<uInt> *C_to_erase;

        // cout << "Estoy eliminando aristas de los posibles bicliques " << endl;
        for (auto it = possible_bicliques->rbegin(); it != possible_bicliques->rend(); it++)
        {
            best_biclique_to_erase = *it;

            S_to_erase = best_biclique_to_erase->S;
            C_to_erase = &best_biclique_to_erase->C;

            for (size_t k = 0; k < S_to_erase->size(); k++)
            {
                if (S_to_erase->at(k)->isModified())
                {
                    if (!S_to_erase->at(k)->includes(C_to_erase)) // modificado
                    {
                        swap(S_to_erase->at(k), S_to_erase->back());
                        S_to_erase->pop_back();
                        // cout << "Erase" << endl;
                        // S_to_erase->erase(S_to_erase->begin() + k);
                        k--;
                    }
                }
            }
        }
    }
    */
}

/*
template<typename GraphType>
void BicliqueExtractor<GraphType>::writeBiclique(vector<Node*>*S, vector<uInt>* C){}
*/
template<typename GraphType>
void BicliqueExtractor<GraphType>::writeBiclique(vector<Node*>* S, vector<uInt>* C)
{
    std::unique_lock<mutex> lock(mtxWriteBiclique);
    ofstream file;
    // file.open(name+"_bicliques-"+to_string(iteration)+".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    string new_path = modify_path(graph->getPath(), 4 , "bicliques.txt");
    file.open(new_path, fstream::app);
    cout << "writing: " << new_path << endl; 
    assert(file.is_open());

    file << "S: ";
    vector<uInt> weights; 
    for (size_t j = 0; j < S->size(); j++)
    {
        file << S->at(j)->getId();
        if (j != S->size() - 1)
            file << " ";
        // cout << "Estoy eliminando S " << j << endl;
        weights = S->at(j)->findToErase(C);
        S->at(j)->setModified(true);
    }
    file << endl
            << "C: ";

    if (weights.size() != C->size()) {
        cout << weights.size() << endl; 
        cout << C->size() << endl;
    }

    assert(weights.size() == C->size()); 

    for (size_t j = 0; j < C->size(); j++) {
        file << "(" << C->at(j) << "," << weights.at(j) << ")";
        if (j != C->size() - 1) file << " ";
    }
    file << endl;
    file.close();

}

template <typename GraphType> 
void BicliqueExtractor<GraphType>::printSignatures(Signatures* group)
{
    for (auto i : *group) {
        cout << i->ptrNode->getId();
        for (int j = 0; j < num_signatures; j++) {
            cout << " | MH[" << j << "]: " << i->minHash.at(j);
        }
        cout << endl;
    }
}

template <typename GraphType> 
void BicliqueExtractor<GraphType>::sortBySignatures(Signatures* group, int signature_index)
{
    sort(group->begin(), group->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}

template <typename GraphType> 
void BicliqueExtractor<GraphType>::sortBicliques(vector<Biclique*>* bicliques)
{
    sort(bicliques->begin(), bicliques->end(), bind(&BicliqueExtractor::compareBicliqueRank, this, placeholders::_1, placeholders::_2));
}

template <typename GraphType> 
bool BicliqueExtractor<GraphType>::sortC(uint64_t a, uint64_t b)
{
    return a < b;
}

template <typename GraphType> 
bool BicliqueExtractor<GraphType>::sortS(Node* a, Node* b)
{
    return a->getId() < b->getId();
}

template <typename GraphType> 
bool BicliqueExtractor<GraphType>::compareMinHash(const SignNode* a, const SignNode* b, int signature_index)
{
    return (a->minHash.at(signature_index) < b->minHash.at(signature_index)); // signNode->vector->minHash
}

template <typename GraphType> 
bool BicliqueExtractor<GraphType>::compareBicliqueRank(const Biclique* a, const Biclique* b)
{
    return a->S->size() * a->C.size() < b->S->size() * b->C.size();
}
