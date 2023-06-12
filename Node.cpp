#include "Node.hpp"
using namespace std;

// PUBLIC METHODS

Node::Node(uInt id) : id(id)
{
	// withSelfLoop = false;
	selfLoop = false;
}

/*Node::Node(uint64_t id, bool selfLoop) : id(id), withSelfLoop(selfLoop)
{
	if(withSelfLoop)
	{
		adjacentNodes.push_back(id);
	}
}*/

Node::~Node()
{
	adjacentNodes.clear();
	cacheNodes.clear();
}

bool Node::hasSelfLoop()
{
	// return natureSelfLoop;
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

void Node::addAdjacent(uInt id_adj)
{
	/*if(id_adj == id) {
		natureSelfLoop = true;
		if(withSelfLoop){ return; }
	}*/
	adjacentNodes.push_back(id_adj);
}

void Node::shrinkToFit()
{
	adjacentNodes.shrink_to_fit();
}

void Node::find_to_erase(vector<uInt> *C)
{
	vector<uInt> new_adjacentNodes;
	vector<uInt>::iterator it = C->begin();
	vector<uInt>::iterator it_end = C->end();
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
	/*
	cout << "****************" << endl;
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
	// adjacentNodes.clear();
	// adjacentNodes = new_adjacentNodes;
	adjacentNodes.swap(new_adjacentNodes);
	new_adjacentNodes.clear();
}

bool Node::findAdjacent(uInt num){
	if( adjacentNodes.empty() )
		return false;
	return binarySearch(0, edgesSize()-1, num);
}

bool Node::binarySearch(uInt l, uInt r, uInt num){
	//cout << "l: " << l << "  r: " << r << endl;
    if (r >= l) {
        uInt mid = l + (r - l) / 2;

        if (adjacentNodes[mid] == num)
            return true;
 
        if (adjacentNodes[mid] > num){
			if(mid == 0)
				return false;
            return binarySearch(l, mid - 1, num);
		}
 
        return binarySearch(mid + 1, r, num);
    }
    return false;
}

void Node::setModified(bool modified)
{
	this->modified = modified;
}

void Node::setSelfLoop(bool selfloop)
{
	selfLoop = selfloop;
}

void Node::sort()
{
	if (edgesSize() == 0)
		return;
	std::sort(adjacentNodes.begin(), adjacentNodes.end());
}

void Node::sortByFrecuency(unordered_map<uInt, uint32_t> *mapFrecuency)
{
	std::sort(adjacentNodes.begin(), adjacentNodes.end(), bind(&Node::sortFrecuencyComp, this, placeholders::_1, placeholders::_2, mapFrecuency));
}

bool Node::includes(vector<uInt> *c)
{
	return std::includes(adjacentNodes.begin(), adjacentNodes.end(), c->begin(), c->end());
}

bool Node::removeAdjacent(uInt id_adj)
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

void Node::moveToCache(unordered_map<uInt, uint32_t> *mapFrecuency, uint16_t minFreq)
{
	for (auto it = (adjacentNodes.end() - 1); it != adjacentNodes.begin(); it--)
	{ // eliminar freq 1
		if (mapFrecuency->at(*it) <= minFreq)
		{
			cacheNodes.push_back(*it);
			adjacentNodes.erase(it);
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

bool Node::sortFrecuencyComp(const uInt &a, const uInt &b, unordered_map<uInt, uint32_t> *mapFrecuency)
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
