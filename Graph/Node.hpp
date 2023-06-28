#ifndef NODE_HPP
#define NODE_HPP

#include "NodeADT.hpp"

using namespace std;

class Node : public NodeADT
{
public:
	// PUBLIC METHODS
	Node(uInt);
	~Node();
	uint64_t edgesSize();
	void addAdjacent(uInt);
	void find_to_erase(vector<uInt> *);
	bool findAdjacent(uInt);
	void shrinkToFit();
	void sort();
	void sortByFrecuency(unordered_map<uInt, uint32_t> *);
	bool includes(vector<uInt> *);
	bool removeAdjacent(uInt);
	void moveToCache(unordered_map<uInt, uint32_t> *, uint16_t);

	AdjacentsIterator adjacentsBegin();
	AdjacentsIterator adjacentsEnd();
	uInt getFrontAdjacent();
	bool restore();
	void print();

private:
	// PRIVATE VARIABLES
	vector<uInt> adjacentNodes;
	vector<uInt> cacheNodes;

	// PRIVATE METHODS
	bool sortFrecuencyComp(const uInt &a, const uInt &b, unordered_map<uInt, uint32_t> *mapFrecuency);
	bool binarySearch(uInt, uInt, uInt);
};

#endif