#include <iostream>
#include <algorithm>
#include "AdjacencyMatrix.hpp"
#include "Utils.hpp"

int main(int argc, char *argv[])
{
    std::unordered_map<std::string, std::string> input_arguments{
        {"final", ""},
        {"biclique", ""}};

    auto arguments = parseArguments(argc, argv, &input_arguments);

    string final_file_path = arguments["final"];
    string bicliques_file_path = arguments["biclique"];

    cout << "final file:  " << final_file_path << endl;
    cout << "bicliques file:  " << bicliques_file_path << endl;

    AdjacencyMatrix *adjMatrix = new AdjacencyMatrix(final_file_path, false);
    cout << "alledgessize: " << adjMatrix->all_edges_size() << endl;    
    cout << "size: " << adjMatrix->size() << endl;
    adjMatrix->addBicliques(bicliques_file_path);
    adjMatrix->writeAdjacencyList(modify_path(arguments["final"],"rebuild"));
    //adjMatrix->print();

    delete adjMatrix;

    return 0;
}
// ./checker --final ../bin32/dblp-2011_compressed.txt --biclique ../bin32/dblp-2011_bicliques.txt 