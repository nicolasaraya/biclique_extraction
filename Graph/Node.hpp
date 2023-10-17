#ifndef NODE_HPP
#define NODE_HPP

#include <unistd.h>

#include "../Utils/Utils.hpp"

using namespace std;

typedef vector<pair<uInt, uInt>*>::iterator WeightedIt;
typedef vector<uInt>::iterator AdjacentsIt; 

class Node {
	public:
		Node(uInt);
		Node(uInt, bool);
		~Node();
		uInt getId();
		//void setModified(bool);
		//bool isModified();
		bool hasSelfLoop();
		void setSelfLoop(bool);
		bool isWeighted();
		uint64_t edgesSize();
		void addAdjacent(uInt);
		void addAdjacent(uInt, uInt);
		bool deleteAdjacent(uInt);
		bool deleteAdjacent(uInt, uInt);
		vector<uInt> findToErase(vector<uInt>*);
		void deleteExtracted(vector<uInt>*);
		void deleteExtracted(vector<pair<uInt, uInt>>*);
		bool findAdjacent(uInt);
		bool findAdjacent(uInt, uInt);
		uInt findAdjacentWeighted(uInt);
		bool increaseWeight(uInt, uInt);
		void shrinkToFit();
		void sort();
		void sortByFrecuency(unordered_map<uInt, uint32_t>*);
		void sortByFrecuency(unordered_map<string, uint32_t>*);
		bool includes(vector<uInt>*);
		bool removeAdjacent(uInt);
		void moveToCache(unordered_map<uInt, uint32_t> *, uint16_t);
		void moveToCache(unordered_map<string, uint32_t> *, uint16_t);
		AdjacentsIt adjacentsBegin();
		AdjacentsIt adjacentsEnd();
		WeightedIt wAdjacentsBegin();
		WeightedIt wAdjacentsEnd();
		uInt getFrontAdjacent();
		uInt getBackAdjacent();
		pair<uInt, uInt>* getFrontWeighted();
		pair<uInt, uInt>* getBackWeighted();
		bool restore();
		void print();
		void printBinary();

	private:
		uInt id;
		bool weighted = false; 
		vector<uInt> adjacentNodes;
		vector<pair<uInt, uInt>*> wAdjacentNodes; 
		vector<pair<uInt, uInt>*> wCacheNodes;
		vector<uInt> cacheNodes;
		//bool modified = false;
		bool naturalSelfLoop = false; 
		bool selfLoop = false;
		bool sorted = false;

		bool sortFrecuencyComp(const uInt &a, const uInt &b, unordered_map<uInt, uint32_t> *mapFrecuency);
		bool sortFrecuencyCompWeighted(pair<uInt, uInt> *a, pair<uInt, uInt> *b, unordered_map<string, uint32_t> *mapFrecuency);
		bool binarySearch(uint64_t, uint64_t, uInt);
		bool binarySearchW(uint64_t, uint64_t, uInt);
		bool binarySearchW(uint64_t, uint64_t, uInt, uInt);
		bool sortWeighted(pair<uInt, uInt>*a, pair<uInt, uInt>*b);
	};

struct compareIncludes {
    bool operator()(std::pair<uInt, uInt>* p, uInt value) const {
        return p->first < value;
    }

    bool operator()(uInt value, std::pair<uInt, uInt>* p) const {
        return value < p->first;
    }
};

struct CompareFind {
    bool operator()(std::pair<uInt, uInt>* p, uInt value) const {
        return p->first == value;
    }
};

#endif