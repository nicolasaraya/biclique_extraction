#ifndef NODE_HPP
#define NODE_HPP

#include "Utils.hpp"

using namespace std;

typedef vector<uint64_t>::iterator AdjacentsIterator;

class Node
{
public:
	// PUBLIC METHODS
	Node(uint64_t, bool);
	~Node();

	bool hasSelfLoop();
	bool isModified();
	uint64_t getId();
	uint64_t edgesSize();
	void addAdjacent(uint64_t);
	void find_to_erase(vector<uint64_t> *);
	void setModified(bool);
	void sort();
	void sortByFrecuency(unordered_map<uint64_t, uint32_t> *);
	bool includes(vector<uint64_t> *);
	bool removeAdjacent(uint64_t);
	void moveToCache(unordered_map<uint64_t, uint32_t> *, uint16_t);
	// const vector<uint64_t> &getAdjacents() const;
	AdjacentsIterator adjacentsBegin();
	AdjacentsIterator adjacentsEnd();
	uint64_t getFrontAdjacent();
	bool restore();
	void print();

private:
	// PRIVATE VARIABLES
	uint64_t id;
	uint64_t adjacents_size;
	bool modified;
	bool selfLoop;
	vector<uint64_t> adjacentNodes;
	vector<uint64_t> cacheNodes;

	// PRIVATE METHODS
	bool sortFrecuencyComp(const uint64_t &a, const uint64_t &b, unordered_map<uint64_t, uint32_t> *mapFrecuency);
};

#endif