#ifndef NODE_HPP
#define NODE_HPP

#include <unistd.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <Utils.hpp>
#include <memory>

struct Pair
{
	uInt first;
	uInt second; 

	Pair(uInt f, uInt s) : first(f), second(s) {;}
};

typedef std::vector<Pair>::iterator WeightedIt;
typedef std::vector<uInt>::iterator AdjacentsIt; 

class Node 
{
	public:
		Node(uInt);
		Node(uInt, bool);
		~Node();
		uInt getId() const;
		bool hasSelfLoop();
		void setSelfLoop(bool);
		bool isWeighted();
		uint64_t edgesSize();
		void addAdjacent(uInt);
		void addAdjacent(uInt, uInt);
		bool deleteAdjacent(uInt);
		bool deleteAdjacent(uInt, uInt);
		std::vector<uInt> findToErase(std::vector<uInt>*);
		void deleteExtracted(std::vector<uInt>&);
		void deleteExtracted(std::vector<Pair>&);
		bool findAdjacent(uInt);
		bool findAdjacent(uInt, uInt);
		uInt findAdjacentWeighted(uInt);
		bool increaseWeight(uInt, uInt);
		void shrinkToFit();
		void sort();
		void sortByFrecuency(std::unordered_map<uInt, uint32_t>*);
		void sortByFrecuency(std::unordered_map<std::string, uint32_t>*);
		bool includes(std::vector<uInt>*);
		bool removeAdjacent(uInt);
		void moveToCache(std::unordered_map<uInt, uint32_t> *, uint16_t);
		void moveToCache(std::unordered_map<std::string, uint32_t> *, uint16_t);
		AdjacentsIt adjacentsBegin();
		AdjacentsIt adjacentsEnd();
		WeightedIt wAdjacentsBegin();
		WeightedIt wAdjacentsEnd();
		uInt getFrontAdjacent();
		uInt getBackAdjacent();
		Pair* getFrontWeighted();
		Pair* getBackWeighted();
		bool restore();
		void print();
		void printBinary();

	private:
		uInt id;
		bool weighted = false; 
		std::vector<uInt> adjacentNodes;
		std::vector<Pair> wAdjacentNodes; 
		std::vector<Pair> wCacheNodes;
		std::vector<uInt> cacheNodes;
		//bool modified = false;
		bool naturalSelfLoop = false; 
		bool selfLoop = false;
		bool sorted = false;

		bool sortFrecuencyComp(const uInt &a, const uInt &b, std::unordered_map<uInt, uint32_t> *mapFrecuency);
		bool sortFrecuencyCompWeighted(Pair& a, Pair& b, std::unordered_map<std::string, uint32_t> *mapFrecuency);
		bool binarySearch(uint64_t, uint64_t, uInt);
		bool binarySearchW(uint64_t, uint64_t, uInt);
		bool binarySearchW(uint64_t, uint64_t, uInt, uInt);
		bool sortWeighted(Pair& a, Pair& b);
	};

typedef std::shared_ptr<Node> NodePtr;

struct compareIncludes 
{
  bool operator()(Pair& p, uInt value) const 
  {
    return p.first < value;
  }

  bool operator()(uInt value, Pair& p) const 
  {
    return value < p.first;
  }
};

struct CompareFind 
{
  bool operator()(Pair& p, uInt value) const 
  {
    return p.first == value;
  }
};

#endif