#include <iostream>
#include "BicliqueExtractor.hpp"

const string path = "./data/";

using namespace std;

int main(int argc, char *argv[]){
    if(argc != 4){
        cout << "uso ./biclique_extractor filename num_sign biclique_size" << endl;
        return 0;
    }
    cout << "file: " << path+argv[1] << endl;
    cout << "Num_signatures " << atoi(argv[2]) << endl;
    BicliqueExtractor be(path+argv[1], atoi(argv[2]), atoi(argv[3]));

    be.makeAdjencyMatrix();
    cout << be.getAdjencyMatrix()->size() << endl;
    be.computeShingles();
    //be.printSignatures();
    be.computeClusters();
    cout <<"********************************" << endl;
    //be.saveCluster();
    //be.printSignatures();

    return 0;
 }