#include "Node.hpp"
using namespace std;

// PUBLIC METHODS

Node::Node(uint64_t id, bool selfLoop) : id(id), selfLoop(selfLoop)
{
	// adjacentNodes = new vector<uint64_t>();
	if (selfLoop)
	{
		adjacentNodes.push_back(id);
	}
}

Node::~Node()
{
	adjacentNodes.clear();
	cacheNodes.clear();
}

bool Node::hasSelfLoop()
{
	return selfLoop;
}

bool Node::isModified()
{
	return modified;
}

uint64_t Node::getId()
{
	return id;
}

uint64_t Node::edgesSize()
{
	return adjacentNodes.size();
}

void Node::addAdjacent(uint64_t id_adj)
{
	if (selfLoop and id_adj == id)
		return;
	else
		adjacentNodes.push_back(id_adj);
}

void Node::find_to_erase(vector<uint64_t> *C) // PODRIA SER BUSQUEDA BINARIA
{
	vector<uint64_t> new_adjacentNodes;
	vector<uint64_t>::iterator it = C->begin();
	vector<uint64_t>::iterator it_end = C->end();
	for (size_t i = 0; i < adjacentNodes.size(); i++)
	{
		if (it == it_end)
		{
			new_adjacentNodes.push_back(adjacentNodes.at(i));
			continue;
		}

		if ((*it) == adjacentNodes.at(i))
			it++;
		else
			new_adjacentNodes.push_back(adjacentNodes.at(i));
	}

	/*cout << "****************" << endl;
	for (size_t i = 0; i < adjacentNodes.size(); i++)
		cout << adjacentNodes[i] << " ";
	cout << endl
		 << "C: ";
	for (size_t i = 0; i < C->size(); i++)
		cout << C->at(i) << " ";
	cout << endl
		 << " new ";
	for (size_t i = 0; i < new_adjacentNodes.size(); i++)
		std::cout << new_adjacentNodes.at(i) << " ";
	std::cout << endl;

	cout << "****************" << endl;*/
	adjacentNodes.clear();
	adjacentNodes = new_adjacentNodes;
}

void Node::setModified(bool modified)
{
	this->modified = modified;
}

void Node::sort()
{
	std::sort(adjacentNodes.begin(), adjacentNodes.end());
}

void Node::sortByFrecuency(unordered_map<uint64_t, uint32_t> *mapFrecuency)
{
	std::sort(adjacentNodes.begin(), adjacentNodes.end(), bind(&Node::sortFrecuencyComp, this, placeholders::_1, placeholders::_2, mapFrecuency));
}

bool Node::includes(vector<uint64_t> *c)
{
	return std::includes(adjacentNodes.begin(), adjacentNodes.end(), c->begin(), c->end());
}

bool Node::removeAdjacent(uint64_t id_adj)
{
	auto element = std::find(adjacentNodes.begin(), adjacentNodes.end(), id_adj);
	if (element != adjacentNodes.end())
	{
		adjacentNodes.erase(element);
		return true;
	}
	else
	{
		return false;
	}
}

void Node::moveToCache(unordered_map<uint64_t, uint32_t> *mapFrecuency, uint16_t minFreq)
{
	for (auto it = (adjacentNodes.end() - 1); it != adjacentNodes.begin(); it--)
	{ // eliminar freq 1
		if (mapFrecuency->at(*it) <= minFreq)
		{
			adjacentNodes.erase(it);
			cacheNodes.push_back(*it);
		}
		else
			break;
	}
}

uint64_t Node::getFrontAdjacent()
{
	if (adjacentNodes.size() > 0)
		return adjacentNodes.at(0);
	return -1;
}

bool Node::restore()
{
	while (not cacheNodes.empty())
	{
		adjacentNodes.push_back(cacheNodes.back());
		cacheNodes.pop_back();
	}
	std::sort(adjacentNodes.begin(), adjacentNodes.end());
	return true;
}

void Node::print()
{
	cout << "Node " << id << ": ";
	for (size_t i = 0; i < adjacentNodes.size(); i++)
		cout << adjacentNodes.at(i) << " ";
	if (cacheNodes.size() > 0)
		cout << " || ";
	for (size_t j = 0; j < cacheNodes.size(); j++)
		cout << cacheNodes[j] << " ";
	cout << endl;
}

AdjacentsIterator Node::adjacentsBegin()
{
	return adjacentNodes.begin();
}

AdjacentsIterator Node::adjacentsEnd()
{
	return adjacentNodes.end();
}

// PRIVATE METHODS

bool Node::sortFrecuencyComp(const uint64_t &a, const uint64_t &b, unordered_map<uint64_t, uint32_t> *mapFrecuency)
{
	if (mapFrecuency->at(a) > mapFrecuency->at(b))
	{
		return true;
	}
	else if (mapFrecuency->at(a) == mapFrecuency->at(b))
	{
		return a < b;
	}
	else
	{
		return false;
	}
}
