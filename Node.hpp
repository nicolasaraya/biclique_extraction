#ifndef NODE_HPP
#define NODE_HPP

#include "Utils.hpp"

using namespace std;

typedef vector<uInt>::iterator AdjacentsIterator;

class Node
{
public:
	// PUBLIC METHODS
	Node(uInt);
	//Node(uint64_t, bool);
	~Node();

	bool hasSelfLoop();
	bool isModified();
	uint64_t getId();
	uint64_t edgesSize();
	void addAdjacent(uInt);
	void find_to_erase(vector<uInt> *);
	bool findAdjacent(uInt);
	void setModified(bool);
	void setSelfLoop(bool);
	void shrinkToFit();
	void sort();
	void sortByFrecuency(unordered_map<uInt, uint32_t> *);
	bool includes(vector<uInt> *);
	bool removeAdjacent(uInt);
	void moveToCache(unordered_map<uInt, uint32_t> *, uint16_t);

	AdjacentsIterator adjacentsBegin();
	AdjacentsIterator adjacentsEnd();
	uint64_t getFrontAdjacent();
	bool restore();
	void print();

private:
	// PRIVATE VARIABLES
	uint64_t id;
	bool modified;
	bool selfLoop;
	vector<uInt> adjacentNodes;
	vector<uInt> cacheNodes;

	// PRIVATE METHODS
	bool sortFrecuencyComp(const uInt &a, const uInt &b, unordered_map<uInt, uint32_t> *mapFrecuency);
	bool binarySearch(uInt, uInt, uInt);
};

#endif