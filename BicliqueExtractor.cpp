#include "BicliqueExtractor.hpp"

// PUBLIC METHODS

BicliqueExtractor::BicliqueExtractor(const string path, uint16_t num_signatures, uint16_t minClusterSize, uint16_t minAdyNodes, uint32_t biclique_size, uint16_t bs_decrease, uint32_t shingleSize, bool selfLoop, uint32_t threshold) : path(path),
                                                                                                                                                                                                                                         num_signatures(num_signatures),
                                                                                                                                                                                                                                         minClusterSize(minClusterSize),
                                                                                                                                                                                                                                         biclique_size(biclique_size),
                                                                                                                                                                                                                                         minAdyNodes(minAdyNodes),
                                                                                                                                                                                                                                         bs_decrease(bs_decrease),
                                                                                                                                                                                                                                         shingleSize(shingleSize),
                                                                                                                                                                                                                                         selfLoop(selfLoop),
                                                                                                                                                                                                                                         threshold(threshold)
{
    adjMatrix = new AdjacencyMatrix(path, selfLoop);
    iteration = 1;
    biclique_s_size = 0;
    biclique_sxc_size = 0;
    biclique_c_size = 0;
    total_biclique = 0;
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
    file.open(path + "_bicliques.txt", std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero bicliques y se cierra
    file.close();

    TIMERSTART(extraction_biclique);
    while (true)
    {
        // adjMatrix->print();
        cout << "Iteracion: " << iteration << endl;
        auto signatures = computeShingles();
        assert(not signatures->empty());
        sortSignatures(signatures, 0);

        /**/
        if (DEBUG_LEVEL > 4)
        {
            std::cout << "***signatures sorted***" << std::endl;
            // printSignatures(signatures);
            auto group = makeGroups(signatures, 0);
            int count = 0;
            for (auto g : group)
            {
                std::cout << "***group(" << count++ << ")***" << std::endl;
                printSignatures(g);
            }
            std::cout << "Groups: " << count << std::endl;
        }
        // vector<Signatures *> group = makeGroups(signatures, 0);
        // computeClusters(&group, 1);
        computeClusters(signatures, 0);

        for (auto i : *signatures)
        {
            delete i;
        }
        delete (signatures);

        computeTree();

        uint32_t n_bicliques = extractBicliques();
        total_biclique += n_bicliques;
        std::cout << "biclique iteracion: " << n_bicliques << std::endl;
        std::cout << "total: " << total_biclique << std::endl;

        file.open("log.txt", fstream::app);
        file << "****************************************************************" << endl;
        file << "Iteracion: " << iteration << endl;
        file << "Adjacency Matrix Size: " << adjMatrix->size() << endl;
        file << "Current Edges Size AdjacencyMatrix: " << adjMatrix->all_edges_size() << endl;
        file << "Min Bilcique Size: " << biclique_size << endl;
        file << "Clusters encontrados: " << clusters.size() << endl;
        file << "Bilciques encontrados: " << n_bicliques << endl;
        file.close();

        adjMatrix->restoreNodes();

        for (auto i : clusters)
            delete i;
        clusters.clear();

        if (iteration == 10)
            break;

        if (n_bicliques < threshold)
        {
            cout << n_bicliques << " " << threshold << endl;
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

    adjMatrix->writeAdjacencyList();
}

// PRIVATE METHODS

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
            clusters.push_back(new Cluster(newCluster));
        }
        else if (clusterSize == 1)
        {
            discarted->push_back(cluster->front()->ptrNode);
        }
        delete cluster;
    }

    if (discarted->size() > 1)
    {
        clusters.push_back(new Cluster(discarted));
    }
    else
    {
        delete discarted;
    }

    candidates.clear();
}

/*
void BicliqueExtractor::computeClusters(vector<Signatures *> *groups, unsigned int column)
{
    for (size_t i = 0; i < groups->size(); i++)
    {
        uint64_t numberEntries = groups->at(i)->size();
        // if (1 < numberEntries)
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
                    {
                        new_cluster->push_back(new_groups[j]->at(k)->ptrNode);
                    }

                    Cluster *c = new Cluster(new_cluster);
                    clusters.push_back(c);
                }
                else if (numberEntries_new_group == 1)
                    new_cluster_single_elements->push_back(new_groups[j]->at(0)->ptrNode);

                delete new_groups[j];
            }
            if (new_cluster_single_elements->size() > 1)
            {
                Cluster *c = new Cluster(new_cluster_single_elements);
                clusters.push_back(c);
            }
            else
            {
                delete new_cluster_single_elements;
            }
        }
        delete groups->at(i);
    }
}*/

void BicliqueExtractor::computeTree()
{
    // omp_set_num_threads(NUM_THREADS);
    TIMERSTART(create_trie);
    // #pragma omp parallel for
    for (auto i : clusters)
    {
        i->computeTrie();
    }
    TIMERSTOP(create_trie);
    return;
}

Signatures *BicliqueExtractor::computeShingles()
{
    auto shingle = new Shingle(num_signatures, minAdyNodes, shingleSize);
    Signatures *sg = new Signatures();
    for (auto i = adjMatrix->begin(); i != adjMatrix->end(); i++)
    {
        SignNode *sn = shingle->computeShingle(*i);
        if (sn != nullptr)
            sg->push_back(sn);
        else
            delete sn;
    }

    if (DEBUG_LEVEL > 3)
    {
        printSignatures(sg);
    }
    delete shingle;
    return sg;
}

uint32_t BicliqueExtractor::extractBicliques()
{
    ofstream file;
    uint32_t n_bicliques = 0;
    // file.open(name+"_bicliques-"+to_string(iteration)+".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    file.open(path + "_bicliques.txt", fstream::app);
    for (size_t i = 0; i < clusters.size(); i++)
    {
        Biclique *b = clusters[i]->getBiclique();
        if (b == nullptr)
            continue;

        vector<Node *> *S = b->first;
        vector<uint64_t> *C = &b->second;

        // si el biclique no cumple con el tamaño deseado o es 2x2, significa que no hay mas bicliques que cumplan la condicion
        if ((S->size() * C->size() < biclique_size) || ((S->size() - 1) * (C->size() - 1) == 1))
        {
            delete b;
            continue;
        }
        else
            n_bicliques += 1;

        // se comienza a extraer el biclique
        biclique_s_size += S->size();
        biclique_c_size += C->size();
        biclique_sxc_size += S->size() * C->size();

        sort(S->begin(), S->end(), bind(&BicliqueExtractor::sortS, this, placeholders::_1, placeholders::_2));
        // sort(C->begin(), C->end(), bind(&BicliqueExtractor::sortC, this, placeholders::_1, placeholders::_2));

        file << "S: ";
        for (size_t j = 0; j < S->size(); j++)
        {
            file << S->at(j)->getId();
            if (j != S->size() - 1)
                file << " ";

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

        delete b;
    }
    file.close();
    return n_bicliques;
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
