#include "AdjencyMatrix.hpp"

using namespace std; 

AdjencyMatrix::AdjencyMatrix(){
    countNodes = 0; 
}
AdjencyMatrix::~AdjencyMatrix(){
}
void AdjencyMatrix::loadFileTxt(const string path){
    this->path = path; 
    ifstream file; 
    file.open(path); 
    if (!file.is_open()){
        cout << "No se puede leer fichero" << endl;
        exit(0);
    }

    string line; 
    getline(file, line); //num nodes
    countNodes = atoll(line.c_str());

    int countAux =0; 
    while(!file.eof()){
        getline(file, line);
        vector<uint64_t> nodes = splitString(line, " ");
        //cout << "i: " << countAux << " , size: " << nodes.size() << endl; 
        if(nodes.size() > 0) matrix.push_back(make_pair(nodes[0], nodes)); //push Nodo y Nodos Adyacentes.
        if(countAux++ == 10) break;
    }
}

vector<uint64_t> AdjencyMatrix::splitString(string line, string delims){
    string::size_type bi, ei;
    vector<uint64_t> nodes;
    bi = line.find_first_not_of(delims);     
    while(bi != string::npos) {
  	    ei = line.find_first_of(delims, bi);
    	if(ei == string::npos) ei = line.length();
		string aux = line.substr(bi,ei-bi);
    	nodes.push_back(atoi(aux.c_str()));
    	bi = line.find_first_not_of(delims, ei);
  	}                 
    return nodes;
}