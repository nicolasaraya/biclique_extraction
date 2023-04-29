#ifndef NODE_HPP
#define NODE_HPP

#include "Utils.hpp"

using namespace std;

class Node
{
public:
	Node(uint64_t, bool);
	~Node();
	bool hasSelfLoop();
	bool isModified();
	uint64_t getId();
	void addAdjacent(uint64_t);
	void setModified(bool);
	bool removeAdjacent(uint64_t);
	bool moveToCache(uint64_t);
	vector<uint64_t> &getAdjacents();
	vector<uint64_t> &getCache();
	bool restore();

private:
	uint64_t id;
	bool modified;
	bool selfLoop;
	vector<uint64_t> adjacentNodes;
	vector<uint64_t> cacheNodes;
};

#endif