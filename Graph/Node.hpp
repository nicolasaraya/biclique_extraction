#ifndef NODE_HPP
#define NODE_HPP

#include <unistd.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <Utils/Utils.hpp>
#include <memory>

struct Pair
{
	uInt first;
	uInt second; 

	Pair(uInt f, uInt s) : first(f), second(s) {;}
};

typedef std::vector<Pair>::iterator WeightedIt;
typedef std::vector<uInt>::iterator AdjacentsIt; 

#if 0
class NodeStd {
public:
    NodeStd(uint32_t id, bool isWeighted) : id(id), weighted(isWeighted) {}
    virtual ~NodeStd() = default;

    virtual uint32_t getId() const = 0;
    virtual bool hasSelfLoop() const = 0;
    virtual void setSelfLoop(bool loop) = 0;
    virtual bool isWeighted() const = 0;
    virtual uint64_t edgesSize() const = 0;
    virtual void addAdjacent(uint32_t target) = 0;
    virtual void addAdjacent(uint32_t target, uint32_t weight) = 0;
    virtual bool deleteAdjacent(uint32_t target) = 0;
    virtual bool deleteAdjacent(uint32_t target, uint32_t weight) = 0;
    virtual std::vector<uint32_t> findToErase(std::vector<uint32_t>*) = 0;
    virtual void deleteExtracted(std::vector<uint32_t>&) = 0;
    virtual void deleteExtracted(std::vector<std::pair<uint32_t, uint32_t>>&) = 0;
    virtual bool findAdjacent(uint32_t target) const = 0;
    virtual bool findAdjacent(uint32_t target, uint32_t weight) const = 0;
    virtual uint32_t findAdjacentWeighted(uint32_t target) const = 0;
    virtual bool increaseWeight(uint32_t target, uint32_t increment) = 0;
    virtual void shrinkToFit() = 0;
    virtual void sort() = 0;
    virtual void sortByFrecuency(std::unordered_map<uint32_t, uint32_t>*) = 0;
    virtual void sortByFrecuency(std::unordered_map<std::string, uint32_t>*) = 0;
    virtual bool includes(std::vector<uint32_t>*) const = 0;
    virtual bool removeAdjacent(uint32_t target) = 0;
    virtual void moveToCache(std::unordered_map<uint32_t, uint32_t>*, uint16_t) = 0;
    virtual void moveToCache(std::unordered_map<std::string, uint32_t>*, uint16_t) = 0;
    virtual uint32_t getFrontAdjacent() const = 0;
    virtual uint32_t getBackAdjacent() const = 0;
    virtual std::pair<uint32_t, uint32_t>* getFrontWeighted() = 0;
    virtual std::pair<uint32_t, uint32_t>* getBackWeighted() = 0;
    virtual bool restore() = 0;
    virtual void print() const = 0;
    virtual void printBinary() const = 0;

protected:
    uint32_t id;               // Identificador del nodo
    bool weighted = false;     // Indicador de si el nodo tiene peso
};
#endif


class Node
{
	public:
		Node(uInt);
		Node(uInt, bool);
    Node(std::istream& is);
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
    //void writeInBinaryFile(std::ostream& os);
    bool serialize(std::ostream& os);
    void serializeDelta16(std::ostream& os);
    static std::shared_ptr<Node> deserialize(std::istream& is);
    static std::shared_ptr<Node> deserializeDelta16(std::istream& is);


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