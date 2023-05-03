#include "AdjacencyMatrix.hpp"

// PUBLIC METHODS

AdjacencyMatrix::AdjacencyMatrix(const string path, bool selfLoops) : path(path), selfLoops(selfLoops)
{
	build();
}

AdjacencyMatrix::AdjacencyMatrix()
{
}

AdjacencyMatrix::~AdjacencyMatrix()
{
	for (auto i : matrix)
		delete i;
}

void AdjacencyMatrix::build()
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
	getline(file, line); // num nodes
	while (!file.eof())
	{
		getline(file, line);
		auto adjacents = splitString(line, " ");
		if (adjacents.size() < 2)
			continue;

		Node *tempNode = new Node(atoi(adjacents.at(0).c_str()), selfLoops);

		for (auto i : adjacents)
		{
			uint64_t adjId = atoi(i.c_str());
			if (adjId == tempNode->getId() and not selfLoops)
				continue;
			tempNode->addAdjacent(atoi(i.c_str()));
		}
		matrix.push_back(tempNode);
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

void AdjacencyMatrix::writeAdjacencyList()
{
	auto p = path;
	p.pop_back();p.pop_back();p.pop_back();p.pop_back(); //pop ".txt"
	cout << "Writing: " << p + "_" + now_time()  << ".txt " << endl;
	ofstream file;
	file.open(p + "_" + now_time() + ".txt", std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero
	file << matrix.size() << std::endl;
	int count = 0; 
	for(auto node : matrix){
		if (count % 10000 == 0) cout << float(count)/float(matrix.size()) * 100 << " %" << endl; 
		file << node->getId() << ": ";
		for(auto adj = node->adjacentsBegin(); adj != node->adjacentsEnd(); adj++){
			file << *adj << " " ;
		}
		file << endl;
		count++; 
	}
	/*
	for (size_t i = 0; i < matrix.size(); i++)
	{
		file << matrix[i]->getId() << ": ";
		auto node_adjacents = matrix[i]->getAdjacents();
		for (size_t j = 0; j < node_adjacents.size(); j++)
			file << node_adjacents[j] << " ";
		file << endl;
		
	}
	*/
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

AdjMatrixIterator AdjacencyMatrix::find(Node *a)
{
	return std::find(matrix.begin(), matrix.end(), a);
}