#include "AdjencyMatrix.hpp"

AdjencyMatrix::AdjencyMatrix(const string path){
	this->path = path;
}
AdjencyMatrix::~AdjencyMatrix(){
	for(unsigned int i=0; i < matrix.size(); i++){
		delete matrix[i];
	}	
	matrix.clear();
}

uint64_t AdjencyMatrix::size(){
    return matrix.size();
}

void AdjencyMatrix::insert(Node* node){
    matrix.push_back(node);
}

Node* AdjencyMatrix::getNode(uint64_t i){
    return matrix.at(i);
}

void AdjencyMatrix::print(){
	for(auto i : matrix){
		cout << i->nodeID << ": ";
		
		for(auto j : i->adyNodes) cout << j << " ";
		if(i->cacheAdyNodes.size() > 0) cout << " || ";
		for(auto j : i->cacheAdyNodes) cout << j << " ";
		
		cout << endl;
	}
}

void AdjencyMatrix::makeAdjencyList(){
	
	while(path[path.length()-1]!= '.') path.pop_back();
	path.pop_back();
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

void AdjencyMatrix::reWork(){
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