#include <iostream>
#include <algorithm>
#include "AdjacencyMatrix.hpp"
#include "Utils.hpp"

void compareAdjMatrix(AdjacencyMatrix *adjMatrix1, AdjacencyMatrix *adjMatrix2)
{
    cout << "Comparando Matrices" << endl;
    uint64_t size1 = adjMatrix1->size();
    uint64_t size2 = adjMatrix2->size();
    if (size1 != size2)
    {
        cout << "Matrices de distintos tamaño" << endl;
        cout << "Adj Matrix 1: " << size1 << " / Adj Matrix 2: " << size2 << endl;
        adjMatrix1->print();
        cout << endl
             << endl;
        adjMatrix2->print();
        return;
    }

    auto begin1 = adjMatrix1->begin();
    auto end1 = adjMatrix1->end();
    auto it2 = adjMatrix2->begin();

    for (auto it1 = begin1; it1 != end1; it1++, it2++)
    {
        if (!std::equal((*it1)->adjacentsBegin(), (*it1)->adjacentsBegin() + (*it1)->edgesSize(), (*it2)->adjacentsBegin()))
        {
            cout << "No son iguales" << endl;
            cout << "Nodo Adj Original:" << endl;
            (*it1)->print();
            cout << "Nodo Adj Restaurada:" << endl;
            (*it2)->print();
            return;
        }
    }
    cout << "Son identicas" << endl;
    return;
}

int main(int argc, char *argv[])
{
    std::unordered_map<std::string, std::string> input_arguments{
        {"original", ""},
        {"final", ""},
        {"biclique", ""}};

    auto arguments = parseArguments(argc, argv, &input_arguments);

    string original_file_path = arguments["original"];
    string final_file_path = arguments["final"];
    string bicliques_file_path = arguments["biclique"];

    cout << "original file: " << original_file_path << endl;
    cout << "final file:  " << final_file_path << endl;
    cout << "bicliques file:  " << bicliques_file_path << endl;

    AdjacencyMatrix *adjMatrix1 = new AdjacencyMatrix(original_file_path, true);
    AdjacencyMatrix *adjMatrix2 = new AdjacencyMatrix(final_file_path, true);
    adjMatrix2->addBicliques(bicliques_file_path);

    compareAdjMatrix(adjMatrix1, adjMatrix2);

    delete adjMatrix1;
    delete adjMatrix2;

    return 0;
}