#include "AdjacencyMatrix.hpp"

AdjacencyMatrix::AdjacencyMatrix(const string path, bool selfLoops) : path(path), selfLoops(selfLoops)
{
	build();
}

AdjacencyMatrix::AdjacencyMatrix()
{}

AdjacencyMatrix::~AdjacencyMatrix()
{
	for(auto i : matrix) delete i;
}

void AdjacencyMatrix::build()
{
	int count = 0;
	ifstream file; 
    file.open(path); 
    if (!file.is_open()){
        cout << "No se puede leer fichero" << endl;
        exit(0);
    }
    string line; 
    getline(file, line); //num nodes
    while (!file.eof()) {
        getline(file, line);
        auto adjacents = splitString(line, " ");
        if (adjacents.size() == 0) continue; 

        Node* tempNode = new Node(atoi(adjacents.at(0).c_str()), selfLoops);

        for (auto i : adjacents) {
			uint64_t adjId = atoi(i.c_str());
			if(adjId == tempNode->getId() and not selfLoops) continue; 
        	tempNode->addAdjacent(atoi(i.c_str())); 
        }

        matrix.push_back(tempNode);
		if(count++ == 100 and DEBUG_LEVEL > 3) break;
    }
    file.close();
}

uint64_t AdjacencyMatrix::size()
{
    return matrix.size();
}

uint64_t AdjacencyMatrix::all_edges_size()
{
	uint64_t size = 0;
	for (auto it : matrix) {
		size += it->getAdjacents().size();
	}
	return size; 
}

void AdjacencyMatrix::insert(Node* node)
{
    matrix.push_back(node);
}


void AdjacencyMatrix::print()
{
	for(auto i : matrix){
		cout << i->getId() << ": ";
		auto adj = i->getAdjacents();
		auto cache = i->getCache();
		for(auto j : adj) cout << j << " ";
		if(cache.size() > 0) cout << " || ";
		for(auto j : cache) cout << j << " ";
		
		cout << endl;
	}
}

void AdjacencyMatrix::writeAdjacencyList()
{
	
	cout << path + now_time() << endl;
	ofstream file;
	file.open(path + now_time() +".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero

	for(auto i : matrix){
		file << i->getId() << ": ";
		
		for(auto j : i->getAdjacents()) file << j << " ";
		file << endl;
	}
	file.close();
}

void AdjacencyMatrix::restoreNodes()
{
	for (auto i : matrix) {
		i->restore();
	}
	/*
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
	*/
}

AdjMatrixIterator AdjacencyMatrix::begin()
{
	return matrix.begin();
}

AdjMatrixIterator AdjacencyMatrix::end()
{
	return matrix.end();
}

AdjMatrixIterator AdjacencyMatrix::find(Node* a)
{
	return std::find(matrix.begin(), matrix.end(), a);
}