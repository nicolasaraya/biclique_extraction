#ifndef NODEW_HPP
#define NODEW_HPP

#include "NodeADT.hpp"

using namespace std;

class NodeWeighted : public NodeADT
{
public:
	// PUBLIC METHODS
	NodeWeighted(uInt);
	~NodeWeighted();
	uint64_t edgesSize();
	void addAdjacent(uInt, uInt);
	//void find_to_erase(vector<uInt> *);
	
	vector<uInt> find_to_erase(vector<uInt> *);
	bool findAdjacent(uInt);
	void shrinkToFit();
	void sort();
	void sortByFrecuency(unordered_map<string, uint32_t> *);
	bool includes(vector<uInt> *);
	bool removeAdjacent(uInt);
	void moveToCache(unordered_map<string, uint32_t> *, uint16_t);

	WeightedIterator adjacentsBegin();
	WeightedIterator adjacentsEnd();
	uInt getFrontAdjacent();
	pair<uInt, uInt> getFrontWeighted();
	bool restore();
	void print();

private:
	// PRIVATE VARIABLES
	bool weighted = true;
	vector<pair<uInt, uInt>> adjacentNodes;
	vector<pair<uInt, uInt>> cacheNodes;

	// PRIVATE METHODS
	bool sortFrecuencyComp(const pair<uInt, uInt> &a, const pair<uInt, uInt> &b, unordered_map<string, uint32_t> *mapFrecuency);
	bool binarySearch(uInt, uInt, uInt);
	bool sortWeighted(const pair<uInt, uInt>&a, const pair<uInt, uInt>&b);
};

struct compareIncludes {
    bool operator()(const std::pair<int, int>& p, int value) const {
        return p.first < value;
    }

    bool operator()(int value, const std::pair<int, int>& p) const {
        return value < p.first;
    }
};

struct CompareFind {
    bool operator()(const std::pair<int, int>& p, int value) const {
        return p.first == value;
    }
};


#endif