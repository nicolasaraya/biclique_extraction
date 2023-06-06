#include "BicliqueExtractor.hpp"

// PUBLIC METHODS

BicliqueExtractor::BicliqueExtractor(
    const string path, uint16_t num_signatures, uint16_t minClusterSize, uint16_t minAdyNodes, uint32_t biclique_size, uint16_t bs_decrease, uint32_t shingleSize, bool selfLoop, uint32_t threshold, uint16_t iterations) : path(path), num_signatures(num_signatures), minClusterSize(minClusterSize), biclique_size(biclique_size), minAdyNodes(minAdyNodes), bs_decrease(bs_decrease), shingleSize(shingleSize), selfLoop(selfLoop), threshold(threshold), iterations(iterations)
{

    adjMatrix = new AdjacencyMatrix(path, selfLoop);
    iteration = 1;
    biclique_s_size = 0;
    biclique_sxc_size = 0;
    biclique_c_size = 0;
    total_biclique = 0;
    cluster_size = 0;
    n_bicliques_iter = 0;
    if (DEBUG_LEVEL > 10)
        adjMatrix->print();
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
    file.open("log.txt", std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero log
    file.close();

    string new_path = modify_path(path, "bicliques.txt");
    file.open(new_path, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero bicliques y se cierra
    file.close();

    TIMERSTART(extraction_biclique);
    while (true)
    {
        n_bicliques_iter = 0;
        // adjMatrix->print();
        cout << "Iteration: " << iteration << endl;

        cout << "Compute Shingles" << endl;
        TIMERSTART(compute_shingles);
        auto signatures = computeShingles();
        TIMERSTOP(compute_shingles);

        assert(not signatures->empty());

        cout << "Compute Clusters and Extract Bicliques" << endl;
        TIMERSTART(compute_clusters_and_bicliques);
        // sortSignatures(signatures, 0);
        // computeClusters(signatures, 1);
        sortSignatures(signatures, 0);

        #if defined(parallel)
            uInt division = signatures->size() / NUM_THREADS;
            std::vector<std::thread> threads(NUM_THREADS);
            for(int i_thread = 0; i_thread < NUM_THREADS; i_thread++){
                uInt l = i_thread * division;
                uInt r = (i_thread + 1) * division;
                if (r > signatures->size() ) r = signatures->size() ;
                threads[i_thread] = std::thread(&BicliqueExtractor::parallelExtraction, this, signatures, l, r);
            }
            for(int i_thread = 0; i_thread < NUM_THREADS; i_thread++){
                threads[i_thread].join();
            }
            
        #else
            vector<Signatures *> group = makeGroups(signatures, 0);
            computeClusters(&group, 1);
        #endif

        TIMERSTOP(compute_clusters_and_bicliques);

        for (auto i : *signatures)
        {
            delete i;
        }
        delete (signatures);

        total_biclique += n_bicliques_iter;

        std::cout << "biclique iteracion: " << n_bicliques_iter << std::endl;
        std::cout << "total: " << total_biclique << std::endl;

        file.open("log.txt", fstream::app);
        file << "****************************************************************" << endl;
        file << "Iteracion: " << iteration << endl;
        file << "Adjacency Matrix Size: " << adjMatrix->size() << endl;
        file << "Current Edges Size AdjacencyMatrix: " << adjMatrix->all_edges_size() << endl;
        file << "Min Bilcique Size: " << biclique_size << endl;
        file << "Clusters encontrados: " << cluster_size << endl;
        file << "Bilciques encontrados: " << n_bicliques_iter << endl;
        file.close();

        cout << "Restaurando Nodos" << endl;

        TIMERSTART(restaure_nodes);
        adjMatrix->restoreNodes();
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
    file << "Current Size AdjacencyMatrix: " << adjMatrix->size() << endl
         << endl;
    file << "Original Edges Size AdjacencyMatrix: " << adjacencyMatrixOriginalEdgesSize << endl;
    file << "Current Edges Size AdjacencyMatrix: " << adjMatrix->all_edges_size() << endl
         << endl;
    file << "Number of Bicliques Extracted: " << total_biclique << endl;
    file << "Sum of S: " << biclique_s_size << endl;
    file << "Sum of C: " << biclique_c_size << endl;
    file << "Sum of S + C: " << biclique_s_size + biclique_c_size << endl;
    file << "Sum of Multiplication of S x C: " << biclique_sxc_size << endl;
    file.close();

    string path_write = modify_path(path, "compressed");
    adjMatrix->writeAdjacencyList(path_write);
}

// PRIVATE METHODS
#if defined(parallel)
void BicliqueExtractor::parallelExtraction(Signatures* signatures, uInt l, uInt r){
    //std::cout << l << ", " << r << endl;
    vector<Signatures *> group = makeGroups(signatures, l, r,  0);
    computeClusters(&group, 1);
}


vector<Signatures *> BicliqueExtractor::makeGroups(Signatures *signatures, uInt l, uInt r, int column)
{
    vector<Signatures *> group;
    Signatures *partition = new Signatures();
    group.push_back(partition);

    for(uInt i = l; i < r; i++){
        auto signNode = signatures->at(i); 
         if (partition->empty())
        {
            partition->push_back(signNode);
        }
        else if (partition->front()->minHash.at(column) == (signNode)->minHash.at(column))
        {
            partition->push_back(signNode);
        }
        else
        {
            partition = new Signatures();
            group.push_back(partition);
        }

    }
    return group;
}
#endif
vector<Signatures *> BicliqueExtractor::makeGroups(Signatures *signatures, int column)
{
    vector<Signatures *> group;
    Signatures *partition = new Signatures();
    group.push_back(partition);

    for (SignatureIterator signNode = signatures->begin(); signNode != signatures->end(); signNode++)
    {
        if (partition->empty())
        {
            partition->push_back(*signNode);
        }
        else if (partition->front()->minHash.at(column) == (*signNode)->minHash.at(column))
        {
            partition->push_back(*signNode);
        }
        else
        {
            partition = new Signatures();
            group.push_back(partition);
        }
    }
    return group;
}


/*
void BicliqueExtractor::computeClusters(Signatures *sign_cluster, unsigned int column)
{
    sortSignatures(sign_cluster, column);
    vector<Signatures *> candidates = makeGroups(sign_cluster, column);

    auto discarted = new vector<Node *>();

    for (auto cluster : candidates)
    {
        auto clusterSize = cluster->size();

        if (clusterSize > minClusterSize and (int) column < num_signatures - 1)
        {
            computeClusters(cluster, column + 1);
        }
        else if (clusterSize > 1)
        {
            auto newCluster = new vector<Node *>();

            for (auto node : *cluster)
            {
                newCluster->push_back(node->ptrNode);
            }
            extractBicliques(new Cluster(newCluster));
        }
        else if (clusterSize == 1)
        {
            discarted->push_back(cluster->front()->ptrNode);
        }
        delete cluster;
    }

    if (discarted->size() > 1)
    {
        extractBicliques(new Cluster(discarted));
    }
    else
    {
        delete discarted;
    }

    candidates.clear();
}*/

void BicliqueExtractor::computeClusters(vector<Signatures *> *groups, unsigned int column)
{
    for (size_t i = 0; i < groups->size(); i++)
    {
        uint64_t numberEntries = groups->at(i)->size();

        if ((num_signatures + (uint64_t)1) < numberEntries)
        {
            sortSignatures(groups->at(i), column);
            vector<Signatures *> new_groups = makeGroups(groups->at(i), column);

            vector<Node *> *new_cluster_single_elements = new vector<Node *>();

            for (size_t j = 0; j < new_groups.size(); j++)
            {
                uint64_t numberEntries_new_group = new_groups[j]->size();

                if (numberEntries_new_group > minClusterSize && column < num_signatures - (unsigned int)1)
                {
                    vector<Signatures *> groups_new_cluster;
                    groups_new_cluster.push_back(new_groups[j]);
                    computeClusters(&groups_new_cluster, column + 1);
                }
                else if (numberEntries_new_group > 1)
                {
                    vector<Node *> *new_cluster = new vector<Node *>();
                    for (size_t k = 0; k < new_groups[j]->size(); k++)
                        new_cluster->push_back(new_groups[j]->at(k)->ptrNode);

                    extractBicliques(new Cluster(new_cluster));
                }
                else if (numberEntries_new_group == 1)
                    new_cluster_single_elements->push_back(new_groups[j]->at(0)->ptrNode);

                delete new_groups[j];
            }
            if (new_cluster_single_elements->size() > 1)
                extractBicliques(new Cluster(new_cluster_single_elements));
            else
                delete new_cluster_single_elements;
        }
        delete groups->at(i);
    }
}

void BicliqueExtractor::extractBicliques(Cluster *c)
{
    cluster_size++;
    c->computeTrie();
    getBicliques(c);
    delete c;
}

Signatures *BicliqueExtractor::computeShingles()
{
    auto shingle = new Shingle(num_signatures, minAdyNodes, shingleSize);
    Signatures *sg = new Signatures();

    #if defined(parallel)
        uInt partitionSize = adjMatrix->size()/NUM_THREADS;
        vector<std::thread> threads(NUM_THREADS);
        for(Int i = 0; i < NUM_THREADS; i++){
            uInt l = i * partitionSize;
            uInt r = (i+1) * partitionSize; 
            if (r > adjMatrix->size()) r = adjMatrix->size();
            threads[i] = std::thread(&BicliqueExtractor::shingleParallel, this, shingle, sg, l, r);
        }
        for(Int i = 0; i < NUM_THREADS; i++){
            threads[i].join();
        }


    #else
        for (auto i = adjMatrix->begin(); i != adjMatrix->end(); i++)
        {
            SignNode *sn = shingle->computeShingle(*i);
            if (sn != nullptr){
                sg->push_back(sn);
            }
            else
                delete sn;
        }
    #endif

    delete shingle;
    return sg;
}

#if defined(parallel)
void BicliqueExtractor::shingleParallel(Shingle* shingle, Signatures* sg, uInt l, uInt r){
    for(uInt i = l; i < r; i++){
        SignNode *sn = shingle->computeShingle(adjMatrix->at(i));
         if (sn != nullptr){
            std::unique_lock<mutex> lock(mtxSignatures);
            sg->push_back(sn);
        }
        else
            delete sn;
    }

}
#endif

void BicliqueExtractor::getBicliques(Cluster *c)
{
    
    vector<Biclique *> *possible_bicliques = c->getBicliques();

    if (possible_bicliques->empty())
        return;

    for (size_t i = 0; i < possible_bicliques->size(); i++)
        sort(possible_bicliques->at(i)->second.begin(), possible_bicliques->at(i)->second.end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2));

    while (!possible_bicliques->empty())
    {
        // sort by size/rank
        sortBicliques(possible_bicliques);

        // se elije el biclique con mayor rank
        Biclique *best_biclique = possible_bicliques->at(possible_bicliques->size() - 1);

        vector<Node *> *S = best_biclique->first;
        vector<uInt> *C = &best_biclique->second;

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
        Biclique *best_biclique_to_erase;
        vector<Node *> *S_to_erase;
        vector<uInt> *C_to_erase;

        // cout << "Estoy eliminando aristas de los posibles bicliques " << endl;
        for (auto it = possible_bicliques->rbegin(); it != possible_bicliques->rend(); it++)
        {
            best_biclique_to_erase = *it;

            S_to_erase = best_biclique_to_erase->first;
            C_to_erase = &best_biclique_to_erase->second;

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
}

void BicliqueExtractor::writeBiclique(vector<Node*>* S, vector<uInt>* C){
    std::unique_lock<mutex> lock(mtxWriteBiclique);
    ofstream file;
    // file.open(name+"_bicliques-"+to_string(iteration)+".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    string new_path = modify_path(path, "bicliques.txt");
    file.open(new_path, fstream::app);
    file << "S: ";
    for (size_t j = 0; j < S->size(); j++)
    {
        file << S->at(j)->getId();
        if (j != S->size() - 1)
            file << " ";
        // cout << "Estoy eliminando S " << j << endl;
        S->at(j)->find_to_erase(C);
        S->at(j)->setModified(true);
    }
    file << endl
            << "C: ";

    for (size_t j = 0; j < C->size(); j++)
    {
        file << C->at(j);
        if (j != C->size() - 1)
            file << " ";
    }

    file << endl;
    
    file.close();

}

void BicliqueExtractor::printSignatures(Signatures *signatures)
{
    for (auto i : *signatures)
    {
        cout << i->ptrNode->getId();
        for (int j = 0; j < num_signatures; j++)
        {
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

void BicliqueExtractor::sortSignatures(Signatures *signs, int signature_index)
{
    sort(signs->begin(), signs->end(), bind(&BicliqueExtractor::compareMinHash, this, placeholders::_1, placeholders::_2, signature_index));
}

void BicliqueExtractor::sortBicliques(vector<Biclique *> *bicliques)
{
    sort(bicliques->begin(), bicliques->end(), bind(&BicliqueExtractor::compareBicliqueRank, this, placeholders::_1, placeholders::_2));
}

bool BicliqueExtractor::sortC(uint64_t a, uint64_t b)
{
    return a < b;
}

bool BicliqueExtractor::sortS(Node *a, Node *b)
{
    return a->getId() < b->getId();
}

bool BicliqueExtractor::compareMinHash(const SignNode *a, const SignNode *b, int signature_index)
{
    return (a->minHash.at(signature_index) < b->minHash.at(signature_index)); // signNode->vector->minHash
}

bool BicliqueExtractor::compareBicliqueRank(const Biclique *a, const Biclique *b)
{
    return a->first->size() * a->second.size() < b->first->size() * b->second.size();
}
