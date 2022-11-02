#include <iostream>
#include "BicliqueExtractor.hpp"

const string path = "./data/dblp-2011.txt";

using namespace std;

int main(int argc, char *argv[]){

    BicliqueExtractor be(path, 1 ,1);

    be.makeAdjencyMatrix();
    cout << be.getAdjencyMatrix()->size() << endl;

    return 0;
 }