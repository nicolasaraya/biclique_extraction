#ifndef NODE_HPP
#define NODE_HPP

#include "../Utils/Utils.hpp"

using namespace std;

typedef vector<pair<uInt, uInt>>::iterator WeightedIt;
typedef vector<uInt>::iterator AdjacentsIt; 

class Node {
	public:
		Node(uInt);
		Node(uInt, bool);
		~Node();
		uInt getId();
		void setModified(bool);
		bool isModified();
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
		pair<uInt, uInt> getFrontWeighted();
		bool restore();
		void print();

	private:
		vector<uInt> adjacentNodes;
		vector<pair<uInt, uInt>> wAdjacentNodes; 
		vector<pair<uInt, uInt>> wCacheNodes;
		vector<uInt> cacheNodes;
		bool weighted = false; 
		bool modified = false;
		bool selfLoop = false;
		uInt id;

		bool sortFrecuencyComp(const uInt &a, const uInt &b, unordered_map<uInt, uint32_t> *mapFrecuency);
		bool sortFrecuencyCompWeighted(const pair<uInt, uInt> &a, const pair<uInt, uInt> &b, unordered_map<string, uint32_t> *mapFrecuency);
		bool binarySearch(uint64_t, uint64_t, uInt);
		bool binarySearchW(uint64_t, uint64_t, uInt);
		bool binarySearchW(uint64_t, uint64_t, uInt, uInt);
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