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
			Node *tempNode = find_value_recursive(atoi(node.c_str()), 0, matrix.size() - 1);
			// Node *tempNode = find_value(atoi(node.c_str()));

			if (tempNode == NULL)
			{
				tempNode = new Node(atoi(node.c_str()), selfLoops);
				matrix.push_back(tempNode);
				sort(matrix.begin(), matrix.end(), bind(&AdjacencyMatrix::compareNodes, this, placeholders::_1, placeholders::_2));
			}

			for (auto adjacent : centers)
			{
				uint64_t adjId = atoi(adjacent.c_str());
				tempNode->addAdjacent(adjId);
			}

			if (tempNode->edgesSize() > centers.size())
				tempNode->sort();
		}
	}
	cout << "Fin addBicliques" << endl;
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
	file << matrix.size() << std::endl;
	int count = 0;
	for (uint64_t i = 1; i < last_node + 1; i++)
	{
		if (count < matrix.size() && i == matrix[count]->getId())
		{
			if (i % 100000 == 0)
				cout << float(i) / float(matrix.size()) * 100 << " %" << endl;

			file << matrix[count]->getId() << ":";
			for (auto adj = matrix[count]->adjacentsBegin(); adj != matrix[count]->adjacentsEnd(); adj++)
				file << " " << *adj;
			count++;
		}
		else
			file << i << ":";
		file << endl;
		count++;
	}
	/*for (auto node : matrix)
	{
		if (count % 100000 == 0)
			cout << float(count) / float(matrix.size()) * 100 << " %" << endl;
		file << node->getId() << ": ";
		for (auto adj = node->adjacentsBegin(); adj != node->adjacentsEnd(); adj++)
		{
			file << *adj << " ";
		}
		file << endl;
		count++;
	}*/
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
Node *AdjacencyMatrix::find_value(uint64_t id)
{
	uint64_t mid = matrix.size() / 2;

	if (id == matrix.at(mid)->getId())
	{
		return matrix.at(mid);
	}
	else if (id > matrix.at(mid)->getId())
	{
		return find_value_recursive(id, mid + 1, matrix.size());
	}
	else if (id < matrix.at(mid)->getId())
	{
		return find_value_recursive(id, 0, mid);
	}
	return nullptr;
}

Node *AdjacencyMatrix::find_value_recursive(uint64_t id, uint64_t l, uint64_t r)
{
	if (l > r)
		return nullptr;

	uint64_t mid = (l + r) / 2;

	uint64_t node_id = matrix.at(mid)->getId();

	if (id == node_id)
		return matrix.at(mid);

	if (id < node_id)
		r = mid - 1;
	else
		l = mid + 1;

	return find_value_recursive(id, l, r);
	/*
	uint64_t mid = ((r - l) / 2) + l;

	if (l > r)
		return nullptr;

	if (id == matrix.at(mid)->getId())
	{
		return matrix.at(mid);
	}
	else if (id > matrix.at(mid)->getId())
	{
		return find_value_recursive(id, mid + 1, r);
	}
	else if (id < matrix.at(mid)->getId())
	{
		return find_value_recursive(id, l, mid - 1);
	}
	return nullptr;*/
}

bool AdjacencyMatrix::compareNodes(Node *a, Node *b)
{
	return (a->getId() < b->getId());
}