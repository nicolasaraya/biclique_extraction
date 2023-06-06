#include <iostream>
#include "BicliqueExtractor.hpp"
#include "Utils.hpp"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    
    std::unordered_map<std::string, std::string> input_arguments{
        {"file", "../data/dblp-2011.txt"},
        {"numSignatures", "2"},
        {"minClusterSize", "10"},
        {"bicliqueSize", "2"},
        {"minAdyNodes", "500"},
        {"bsDecrease", "500"},
        {"shingleSize", "1"},
        {"selfLoop", "0"},
        {"threshold", "100"},
        {"debug", "0"},
        {"iterations", "10"}};

    auto arguments = parseArguments(argc, argv, &input_arguments);

    #if defined(parallel)
        cout<<"Using: " << NUM_THREADS << " threads" << endl;
    #endif

    cout << "file: " << arguments["file"] << endl;
    cout << "Num_signatures:  " << arguments["numSignatures"] << endl;
    cout << "Min_Cluster_Size:  " << arguments["minClusterSize"] << endl;
    cout << "Min_Adyacency_Nodes:  " << arguments["minAdyNodes"] << endl;
    cout << "Biclique_Size:  " << arguments["bicliqueSize"] << endl;
    cout << "Biclique_Size_Decrease:  " << arguments["bsDecrease"] << endl;
    cout << "Shingle Size:  " << arguments["shingleSize"] << endl;
    cout << "SelfLoop:  " << arguments["selfLoop"] << endl;
    cout << "Debug flag:  " << arguments["debug"] << endl;
    cout << "Threshold: " << arguments["threshold"] << endl;
    cout << "Iterations: " << arguments["iterations"] << endl;
    cout << "Bits: " << sizeof(Int) * 8 << endl;
    
    
    
    BicliqueExtractor be(
        arguments["file"],
        atoi(arguments["numSignatures"].c_str()),
        atoi(arguments["minClusterSize"].c_str()),
        atoi(arguments["minAdyNodes"].c_str()),
        atoi(arguments["bicliqueSize"].c_str()),
        atoi(arguments["bsDecrease"].c_str()),
        atoi(arguments["shingleSize"].c_str()),
        atoi(arguments["selfLoop"].c_str()),
        atoi(arguments["threshold"].c_str()),
        atoi(arguments["iterations"].c_str())
        );
    be.extract();
    
    
    
    //AdjacencyMatrix* adjMatrix = new AdjacencyMatrix(arguments["file"], false);
    //adjMatrix->print();
    //cout << "adjmatrixsize " << adjMatrix->all_edges_size() << endl;
    //cout << "adjmatrixsizenode " << adjMatrix->size() << endl;
    //cout << "sleep" << endl;
    //sleep(30);
    //delete adjMatrix;
    
    

    return 0;
}

// ./biclique_extractor ../data/dblp-2011.txt 2 10 2 500 400 1
//./biclique_extractor ../data/ej1.txt 2 1 2 2 400 1
// pmap pid : memory usage