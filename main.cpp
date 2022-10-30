#include <iostream>
#include <string>

#include "AdjencyMatrix.hpp"

const string path = "./data/dblp-2011.txt";

using namespace std;

int main(int argc, char *argv[]){

    AdjencyMatrix* am = new AdjencyMatrix();
    cout << "ok" << endl;
    am->loadFileTxt(path);
    return 0;
 }