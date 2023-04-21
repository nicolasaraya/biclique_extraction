#include <iostream>
#include "BicliqueExtractor.hpp"

const string path = "../data/";

using namespace std;

int main(int argc, char *argv[]){
    if(argc != 7){
        cout << "uso ./biclique_extractor filename num_sign min_cluster_size min_adyacency_nodes biclique_size biclique_size_decrease" << endl;
        return 0;
    }
    cout << "file: " << path+argv[1] << endl;
    cout << "Num_signatures:  " << atoi(argv[2]) << endl;
    cout << "Min_Cluster_Size:  " << atoi(argv[3]) << endl;
    cout << "Min_Adyacency_Nodes:  " << atoi(argv[4]) << endl;
    cout << "Biclique_Size:  " << atoi(argv[5]) << endl;
    cout << "Biclique_Size_Decrease:  " << atoi(argv[6]) << endl;

    BicliqueExtractor be(path+argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

    be.extract();

    return 0;
 }