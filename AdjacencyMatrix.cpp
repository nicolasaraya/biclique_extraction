#include "AdjacencyMatrix.hpp"

// PUBLIC METHODS

AdjacencyMatrix::AdjacencyMatrix(const string path, bool selfLoops) : path(path), selfLoops(selfLoops)
{
	if(path.find(".txt" )!= std::string::npos) {
		buildTxt();
		format = "txt";
	}
	else if(path.find(".bin")!= std::string::npos) {
		buildBin();
		format = "bin";
	}
	//print();
}

AdjacencyMatrix::AdjacencyMatrix()
{
}

AdjacencyMatrix::~AdjacencyMatrix()
{
	for (auto i : matrix)
		delete i;
}
// g++ checker.cpp AdjacencyMatrix.cpp Node.cpp Utils.cpp -o checker
void AdjacencyMatrix::addBicliques(string path)
{
	cout << "Read File Bicliques" << endl;
	ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		cout << "No se puede leer fichero" << endl;
		cout << path << endl;
		exit(0);
	}
	vector<uInt> newNodesPos;
	if( size() != back()->getId())
		standardize(&newNodesPos);

	string S;
	string C;

	cout << "Add Bicliques" << endl;
	while (!file.eof())
	{
		getline(file, S);
		if (S == "")
			break;
		getline(file, C);

		std::vector<std::string> sources = splitString(S, " ");
		std::vector<std::string> centers = splitString(C, " ");
		sources.erase(sources.begin());
		centers.erase(centers.begin());

		for (auto node : sources)
		{
			uint64_t id = atoi(node.c_str());
			for (auto adjacent : centers)
			{
				uint64_t adjId = atoi(adjacent.c_str());
				matrix[id - 1]->addAdjacent(adjId);
			}
		}
	}
	cout << "Sorting" << endl;
	for (auto it = matrix.begin(); it != matrix.end(); it++)
		(*it)->sort();

	cout << "Delete aux Nodes" << endl;
	for (size_t i = 0; i < newNodesPos.size(); i++){	
		delete matrix[newNodesPos[i]];
		matrix[newNodesPos[i]] = nullptr;
	}
	newNodesPos.clear();
}

// funcion que agrega los nodos intermedios que no contienen ninguna arista
// para facilitar la busqueda de los nodos source
void AdjacencyMatrix::standardize(vector<uInt>* aux)
{
	cout << "Standarize" << endl;
	vector<Node*> new_matrix;
	auto it = matrix.rbegin();
	uint64_t last_node = back()->getId();
	for (uint64_t cont = last_node; cont > 0; it++, cont--)
	{
		if (it != matrix.rend() && cont == (*it)->getId())
		{
			//continue;
			new_matrix.push_back(*it);
			matrix.pop_back();
		}
		else
		{
			Node *tempNode = new Node(cont);
			new_matrix.push_back(tempNode);
			aux->push_back(cont-1);
			it--;
		}
	}

	for (size_t cont = last_node; cont > 0; cont--){
		matrix.push_back(new_matrix.back());
		new_matrix.pop_back();
	}


}

void AdjacencyMatrix::buildTxt()
{
	// int count = 0;
	ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		cout << "No se puede leer fichero" << endl;
		cout << path << endl;
		exit(0);
	}
	string line;
	uInt id;
	getline(file, line); // num nodes
	num_nodes = atoi(line.c_str());
	while (!file.eof())
	{
		getline(file, line);
		auto adjacents = splitString(line, " ");

		if (adjacents.empty())
			continue;
		
		id = atoi(adjacents.at(0).c_str());

		Node *tempNode = new Node(id);
		/*
		for (size_t i = 1; i < adjacents.size(); i++){
			uint64_t adjId = atoi(adjacents[i].c_str());
			tempNode->addAdjacent(adjId);
		}
		tempNode->sort();*/
		for (int i = 1; i < adjacents.size();i++)
		{
			uint64_t adjId = atoi(adjacents[i].c_str());
			//cout << adjId << " " << tempNode->getId() << endl;
			if ( (adjId == tempNode->getId()) && selfLoops){
				tempNode->setSelfLoop(true);
			}
			tempNode->addAdjacent(adjId);
		}
		if( !tempNode->hasSelfLoop() && selfLoops ){
			tempNode->addAdjacent(tempNode->getId());
		}
		tempNode->shrinkToFit();
		tempNode->sort();
		
	
		matrix.push_back(tempNode);
	}
	file.close();
	matrix.shrink_to_fit();
}

void AdjacencyMatrix::buildBin()
{
	ifstream binFile; 
    binFile.open(path, ios::in | ios::binary);    
	assert(binFile.is_open());

	BinVar* nodes = new BinVar(0); 
	binFile.read( (char*)nodes, sizeof(BinVar) );

	//std::cout << "cantidad de nodos: " << *nodes << endl;
	num_nodes = *nodes;
    BinVar* buffer = new BinVar(0); 
	Node* tempNode = nullptr; 
    while(binFile.read((char*)buffer, sizeof(BinVar))) {
		//cout << *buffer << endl;
        if((*buffer) < 0) {
			if(tempNode!=nullptr){
				if( !tempNode->hasSelfLoop() && selfLoops )
					tempNode->addAdjacent(tempNode->getId());
				tempNode->sort();
				tempNode->shrinkToFit();

			}
			uint64_t id = (*buffer) * -1;
			tempNode = new Node(id);
			matrix.push_back(tempNode);
        }
        else {
			if(tempNode == nullptr) 
				//cout << *buffer << " aristas" << endl;
				continue; //num de aristas

			tempNode->addAdjacent(*buffer);

			if ( (*buffer == tempNode->getId()) && selfLoops)
				tempNode->setSelfLoop(true);
        }
    }

	if( tempNode != nullptr  ){
		if( !tempNode->hasSelfLoop() && selfLoops )
			tempNode->addAdjacent(tempNode->getId());
		tempNode->sort();
		tempNode->shrinkToFit();
	}
	delete buffer;
	delete nodes;
    binFile.close();
	
}


uint64_t AdjacencyMatrix::size()
{
	return matrix.size();
}

Node* AdjacencyMatrix::back(){
	return matrix.back();
}

uint64_t AdjacencyMatrix::all_edges_size()
{
	uint64_t size = 0;
	for (auto it : matrix)
	{
		size += it->edgesSize();
	}
	return size;
}

void AdjacencyMatrix::insert(Node *node)
{
	matrix.push_back(node);
}

void AdjacencyMatrix::print()
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i]->print();
	}
}

void AdjacencyMatrix::writeAdjacencyList(string path_write_)
{
	if( size() == 0 ){
		cout << "Matrix Empty" << endl;
		return;
	}
	ofstream file;
	int count = 0;
	uint64_t matrix_size = size();

	string path_write = path_write_ + ".txt";
	cout << "Writing: " << path_write << endl;
	file.open(path_write, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero

	file << num_nodes << std::endl;
	/*
	for (uint64_t i = 0; i < original_edges.size() ; i++, it++)
	{
		if (it != matrix.end() && original_edges[i] == (*it)->getId())
		{
			if (i % 100000 == 0)
				cout << float(i) / float(original_edges[i]) * 100 << " %" << endl;

			file << (*it)->getId() << ":";
			if ( selfLoops && !(*it)->hasSelfLoop() ){
				(*it)->removeAdjacent( (*it)->getId() );
			}
			for (auto adj = (*it)->adjacentsBegin(); adj != (*it)->adjacentsEnd(); adj++)
				file << " " << *adj;
			count++;
		}
		else
		{
			file << original_edges[i] << ":";
			it--;
		}
		file << endl;
	}*/
	for (uint64_t i = 0; i < matrix_size ; i++)
	{
		if(matrix[i] == nullptr)
			continue;
		if (i % 100000 == 0)
			cout << float(i) / float(matrix_size) * 100 << " %" << endl;
		
		file << matrix[i]->getId() << ":";
		//cout << "id: " << matrix[i]->getId() << " / tamaño adjs: " << matrix[i]->edgesSize() << endl;
		if ( selfLoops && !matrix[i]->hasSelfLoop() ){
			matrix[i]->removeAdjacent( matrix[i]->getId() );
		}
		for (auto adj = matrix[i]->adjacentsBegin(); adj != matrix[i]->adjacentsEnd(); adj++)
			file << " " << *adj;
		file << endl;
	}

	file.close();
}

void AdjacencyMatrix::restoreNodes()
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i]->restore();
	}
}

AdjMatrixIterator AdjacencyMatrix::begin()
{
	return matrix.begin();
}

AdjMatrixIterator AdjacencyMatrix::end()
{
	return matrix.end();
}
