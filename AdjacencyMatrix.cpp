#include "AdjacencyMatrix.hpp"

AdjacencyMatrix::AdjacencyMatrix(const string path){
	this->path = path;
}
AdjacencyMatrix::~AdjacencyMatrix(){
	for(unsigned int i=0; i < matrix.size(); i++){
		delete matrix[i];
	}	
	matrix.clear();
}

uint64_t AdjacencyMatrix::size(){
    return matrix.size();
}

uint64_t AdjacencyMatrix::all_edges_size(){
	uint64_t size = 0;
    for(uint64_t i = 0; i < matrix.size(); i++){
		size+= matrix[i]->adyNodes.size();
	}
	return size; 
}

void AdjacencyMatrix::insert(Node* node){
    matrix.push_back(node);
}

Node* AdjacencyMatrix::getNode(uint64_t i){
    return matrix.at(i);
}

void AdjacencyMatrix::print(){
	for(auto i : matrix){
		cout << i->nodeID << ": ";
		
		for(auto j : i->adyNodes) cout << j << " ";
		if(i->cacheAdyNodes.size() > 0) cout << " || ";
		for(auto j : i->cacheAdyNodes) cout << j << " ";
		
		cout << endl;
	}
}

void AdjacencyMatrix::makeAdjacencyList(){
	std::time_t t = std::time(0);   // get time now
    std::tm* t_now = std::localtime(&t);
	string now =  to_string(t_now->tm_year + 1900) + '-' + to_string(t_now->tm_mon + 1) + '-' + to_string(t_now->tm_mday) + "-" + to_string(t_now->tm_hour) +to_string(t_now->tm_min) +to_string(t_now->tm_sec)  ;
	cout << path+now << endl;


	ofstream file;
	file.open(path+now+".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero

	for(auto i : matrix){
		file << i->nodeID << ": ";
		
		for(auto j : i->adyNodes) file << j << " ";
		file << endl;
	}
	file.close();
	//file 

}

void AdjacencyMatrix::reWork(){
	for(vector<Node*>::iterator i = matrix.begin(); i != matrix.end(); i++){
		Node* aux = *i;
		
		if(aux->adyNodes.size() + aux->cacheAdyNodes.size() < 1) {
			matrix.erase(i);
			i--;
			delete aux;
			continue; 
		}
		for(auto j : aux->cacheAdyNodes){
			aux->adyNodes.push_back(j);
		}
		sort(aux->adyNodes.begin(), aux->adyNodes.end());
		aux->cacheAdyNodes.clear();
	}
}