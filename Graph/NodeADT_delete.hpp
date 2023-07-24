#ifndef NODEADT_HPP
#define NODEADT_HPP

#include "../Utils/Utils.hpp"

using namespace std;

typedef vector<uInt>::iterator AdjacentsIterator;
typedef vector<pair<uInt, uInt>>::iterator WeightedIterator; 

class NodeADT
{
protected:
	// PRIVATE VARIABLES
	uInt id;
	bool modified = false;
	bool selfLoop = false;
public:
    NodeADT(uInt id) : id(id) {};
    
    uInt getId() 
    {
        return id; 
    }

    bool hasSelfLoop()
    {
        return selfLoop;
    }

    bool isModified()
    {
        return modified;
    }

    void setModified(bool modified)
    {
        this->modified = modified;
    }
    void setSelfLoop(bool selfloop)
    {
        this->selfLoop = selfloop;
    }

	//virtual ~NodeADT() = 0;
	virtual uint64_t edgesSize() = 0;
	virtual bool findAdjacent(uInt) = 0;
	virtual void shrinkToFit() = 0;
	virtual void sort() = 0;
	//virtual void sortByFrecuency(unordered_map<uInt, uint32_t> *) = 0;
	virtual bool includes(vector<uInt> *) = 0;
	virtual bool removeAdjacent(uInt) = 0;
	//virtual void moveToCache(unordered_map<uInt, uint32_t> *, uint16_t) = 0;
	virtual uInt getFrontAdjacent() = 0;
	virtual bool restore() = 0;
	virtual void print() = 0;
	virtual void deleteExtracted(vector<uInt>*) = 0;



	// PRIVATE METHODS
};

#endif