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
// g++ checker.cpp AdjacencyMatrix.cpp Node.cpp Utils.cpp -o checker
void AdjacencyMatrix::addBicliques(string path)
{
	cout << "addBicliques" << endl;
	ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		cout << "No se puede leer fichero" << endl;
		cout << path << endl;
		exit(0);
	}
	standardize();

	string S;
	string C;
	vector<Node *> newNodes;
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
	cout << "Fin addBicliques" << endl;
	cout << "Sorting and Deleting" << endl;
	for (auto it = matrix.begin(); it != matrix.end(); it++)
	{
		if ((*it)->edgesSize() < 1)
		{
			delete (*it);
			matrix.erase(it);
			it--;
		}
		else
			(*it)->sort();
	}
}

// funcion que agrega los nodos intermedios que no contienen ninguna arista
// para facilitar la busqueda de los nodos source
void AdjacencyMatrix::standardize()
{
	auto it = matrix.begin();
	for (uint64_t cont = 1; cont < last_node + 1; it++, cont++)
	{
		if (it != matrix.end() && cont == (*it)->getId())
		{
			continue;
		}
		else
		{
			Node *tempNode = new Node(cont, false);
			matrix.insert(it, tempNode);
			it = matrix.begin() + (cont - 1);
		}
	}
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
	uint64_t id;
	getline(file, line); // num nodes
	while (!file.eof())
	{
		getline(file, line);
		auto adjacents = splitString(line, " ");

		if (adjacents.size() > 0)
		{
			id = atoi(adjacents.at(0).c_str());
			last_node = id;
		}
		if (adjacents.size() < 2)
			continue;

		Node *tempNode = new Node(id, selfLoops);

		for (auto i : adjacents)
		{
			uint64_t adjId = atoi(i.c_str());
			if (adjId == tempNode->getId() and not selfLoops)
				continue;
			tempNode->addAdjacent(adjId);
		}
		tempNode->sort();
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
	p.pop_back();
	p.pop_back();
	p.pop_back();
	p.pop_back(); // pop ".txt"
	cout << "Writing: " << p + "_" + now_time() << ".txt " << endl;
	ofstream file;
	file.open(p + "_" + now_time() + ".txt", std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero
	file << last_node << std::endl;

	int count = 0;
	auto it = matrix.begin();
	for (uint64_t i = 1; i < last_node + 1; i++, it++)
	{
		if (it != matrix.end() && i == (*it)->getId())
		{
			if (i % 100000 == 0)
				cout << float(i) / float(last_node) * 100 << " %" << endl;

			file << (*it)->getId() << ":";
			for (auto adj = (*it)->adjacentsBegin(); adj != (*it)->adjacentsEnd(); adj++)
				file << " " << *adj;
			count++;
		}
		else
		{
			file << i << ":";
			it--;
		}
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