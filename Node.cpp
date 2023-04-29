#include "Node.hpp"
using namespace std;

Node::Node(uint64_t id, bool selfLoop) : id(id), selfLoop(selfLoop)
{
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

void Node::setModified(bool modified)
{
	this->modified = modified;
}

void Node::addAdjacent(uint64_t id_adj)
{
	if (selfLoop and id_adj == id)
		return;
	else
		adjacentNodes.push_back(id_adj);
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

bool Node::moveToCache(uint64_t id_adj)
{
	auto f = std::find(adjacentNodes.begin(), adjacentNodes.end(), id_adj);
	if (f == adjacentNodes.end())
		return false;
	else
	{
		adjacentNodes.erase(f);
		cacheNodes.push_back(id_adj);
		return true;
	}
}

vector<uint64_t> &Node::getAdjacents()
{
	return adjacentNodes;
}

vector<uint64_t> &Node::getCache()
{
	return cacheNodes;
}

bool Node::restore()
{
	while (not cacheNodes.empty())
	{
		adjacentNodes.push_back(cacheNodes.back());
		cacheNodes.pop_back();
	}
	sort(adjacentNodes.begin(), adjacentNodes.end());
	return true;
}
