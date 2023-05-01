#include <iostream>
#include "BicliqueExtractor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 8)
    {
        cout << "uso ./biclique_extractor filename num_sign min_cluster_size min_adyacency_nodes biclique_size biclique_size_decrease shingleSize" << endl;
        return 0;
    }
    cout << "file: " << argv[1] << endl;
    cout << "Num_signatures:  " << atoi(argv[2]) << endl;
    cout << "Min_Cluster_Size:  " << atoi(argv[3]) << endl;
    cout << "Min_Adyacency_Nodes:  " << atoi(argv[4]) << endl;
    cout << "Biclique_Size:  " << atoi(argv[5]) << endl;
    cout << "Biclique_Size_Decrease:  " << atoi(argv[6]) << endl;
    cout << "Shingle Size:  " << atoi(argv[7]) << endl;

    BicliqueExtractor be(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
    be.extract();

    return 0;
}

// ./biclique_extractor ../data/dblp-2011.txt 2 10 2 500 400 1
//./biclique_extractor ../data/ej1.txt 2 1 2 2 400 1