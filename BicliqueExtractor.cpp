#include "BicliqueExtractor.hpp"

// PUBLIC METHODS

template <typename GraphType> 
BicliqueExtractor<GraphType>::BicliqueExtractor(
    uint16_t num_signatures, uint16_t minClusterSize, uint16_t minAdyNodes, uint32_t biclique_size, uint16_t bs_decrease, uint32_t shingleSize, uint32_t threshold, uint16_t iterations) : num_signatures(num_signatures), minClusterSize(minClusterSize), biclique_size(biclique_size), minAdyNodes(minAdyNodes), bs_decrease(bs_decrease), shingleSize(shingleSize), threshold(threshold), iterations(iterations)
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
            computeClusters(*cluster, column + 1);
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
    ofstream file;
    string new_path = modify_path(graph->getPath(), 4 , "bicliques.txt");
    file.open(new_path, fstream::app);
    assert(file.is_open());
    for (auto biclique = bicliques->begin(); biclique != bicliques->end(); biclique++) {
        vector<Node*>* S = (*biclique)->S;
        vector<uInt>* C = (*biclique)->C; 
        vector<pair<uInt, uInt>>* C_w = (*biclique)->C_w;

        uInt S_size = S->size();
        uInt C_size = 0; 
        if (C != nullptr) {
            C_size = C->size();
        } else {
            C_size = C_w->size(); 
        }

        if(S_size * C_size < biclique_size) {
            delete *biclique;
            continue; 
        }

        sort(S->begin(), S->end(), bind(&BicliqueExtractor::sortS, this, placeholders::_1, placeholders::_2));
        if(C != nullptr) sort(C->begin(), C->end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2));
        else sort(C_w->begin(), C_w->end(), bind(&BicliqueExtractor::sortC_w, this, placeholders::_1, placeholders::_2));

        file << "S: ";
        for(auto it = S->begin(); it != S->end(); it++){
            if(C != nullptr){
                (*it)->deleteExtracted(C);
            } else {
                (*it)->deleteExtracted(C_w);  
            }
            file << (*it)->getId() << " "; 
            //(*it)->setModified(true);
        }

        file << endl << "C: ";
        if (C != nullptr) {
            for(auto it = C->begin(); it != C->end(); it++){
                file << *it << " "; 
            }
        } else {
            for(auto it = C_w->begin(); it != C_w->end(); it++){
                file << "(" << (*it).first << "," << (*it).second << ") "; 
            }
        }
        file << endl;
        file << "SxC = " << C_size * S_size << endl;   
        n_bicliques_iter++;
        biclique_s_size += S_size;
        biclique_c_size += C_size;
        biclique_sxc_size += S_size * C_size;
        delete *biclique; 
    }
    file.close();
    delete bicliques;
    return; 
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
bool BicliqueExtractor<GraphType>::sortC_w(pair<uInt, uInt> a, pair<uInt, uInt> b)
{
    return a.first < b.first;
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
    if(a->C != nullptr and b->C != nullptr){
        return a->S->size() * a->C->size() < b->S->size() * b->C->size();
    } else {
        return a->S->size() * a->C_w->size() < b->S->size() * b->C_w->size();
    }
    return false; 
}
