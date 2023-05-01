#include <iostream>
#include "BicliqueExtractor.hpp"
#include "Utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    auto arguments = parseArguments(argc, argv);
    cout << "file: " << arguments["file"] << endl;
    cout << "Num_signatures:  " << arguments["numSignatures"] << endl;
    cout << "Min_Cluster_Size:  " << arguments["minClusterSize"] << endl;
    cout << "Min_Adyacency_Nodes:  " << arguments["minAdyNodes"] << endl;
    cout << "Biclique_Size:  " << arguments["bicliqueSize"] << endl;
    cout << "Biclique_Size_Decrease:  " << arguments["bsDecrease"] << endl;
    cout << "Shingle Size:  " << arguments["shingleSize"] << endl;
    cout << "SelfLoop:  " << arguments["selfLoop"] << endl;
    cout << "Debug flag:  " << arguments["debug"] << endl;
    cout << "Threshold:: " << arguments["threshold"] << endl;

    BicliqueExtractor be(
        arguments["file"],        
        atoi(arguments["numSignatures"].c_str()), 
        atoi(arguments["minClusterSize"].c_str()), 
        atoi(arguments["bicliqueSize"].c_str()), 
        atoi(arguments["minAdyNodes"].c_str()), 
        atoi(arguments["bsDecrease"].c_str()), 
        atoi(arguments["shingleSize"].c_str()),
        atoi(arguments["selfLoop"].c_str()),
        atoi(arguments["threshold"].c_str())
        );
    be.extract();

    return 0;
}

// ./biclique_extractor ../data/dblp-2011.txt 2 10 2 500 400 1
//./biclique_extractor ../data/ej1.txt 2 1 2 2 400 1