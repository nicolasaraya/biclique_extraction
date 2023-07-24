#include "NodeWeighted.hpp"
using namespace std;


NodeWeighted::NodeWeighted(uInt id) : NodeADT(id) {;}

NodeWeighted::~NodeWeighted()
{
	adjacentNodes.clear();
	cacheNodes.clear();
}

void NodeWeighted::addAdjacent(uInt id_adj, uInt weight)
{
	adjacentNodes.push_back(make_pair(id_adj, weight));
}

void NodeWeighted::shrinkToFit()
{
	adjacentNodes.shrink_to_fit();
	
}
uint64_t NodeWeighted::edgesSize(){
	return adjacentNodes.size();
}
vector<uInt> NodeWeighted::find_to_erase(vector<uInt> *C)
{
	vector<uInt> weights; 
	vector<pair<uInt, uInt>> new_adjacentNodes;
	auto it = C->begin();
	auto it_end = C->end();
	for (size_t i = 0; i < adjacentNodes.size(); i++)
	{
		if (it == it_end)
		{
			new_adjacentNodes.push_back(adjacentNodes.at(i));
			continue;
		}

		if ((*it) == adjacentNodes.at(i).first){
			weights.push_back(adjacentNodes.at(i).second);
			it++;
		}
		else {
			new_adjacentNodes.push_back(adjacentNodes.at(i));
		}
	}
	adjacentNodes.swap(new_adjacentNodes);
	new_adjacentNodes.clear();
	return weights;
}

bool NodeWeighted::findAdjacent(uInt num){
	if( adjacentNodes.empty() )
		return false;
	return binarySearch(0, edgesSize()-1, num);
}

bool NodeWeighted::binarySearch(uInt l, uInt r, uInt num){
    if (r >= l) {
        uInt mid = l + (r - l) / 2;

        if (adjacentNodes[mid].first == num)
            return true;
 
        if (adjacentNodes[mid].first > num){
			if(mid == 0)
				return false;
            return binarySearch(l, mid - 1, num);
		}
 
        return binarySearch(mid + 1, r, num);
    }
    return false;
}

void NodeWeighted::sort()
{
	if (edgesSize() == 0)
		return;
	
	std::sort(adjacentNodes.begin(), adjacentNodes.end(), bind(&NodeWeighted::sortWeighted, this, placeholders::_1, placeholders::_2)); 
}

void NodeWeighted::sortByFrecuency(unordered_map<string, uint32_t> *mapFrecuency)
{
	std::sort(adjacentNodes.begin(), adjacentNodes.end(), bind(&NodeWeighted::sortFrecuencyComp, this, placeholders::_1, placeholders::_2, mapFrecuency));
}

bool NodeWeighted::includes(vector<uInt> *c)
{
	return std::includes(adjacentNodes.begin(), adjacentNodes.end(), c->begin(), c->end(), compareIncludes());
}

bool NodeWeighted::removeAdjacent(uInt idAdj)
{
	auto element = std::find_if(adjacentNodes.begin(), adjacentNodes.end(), [idAdj](const std::pair<int, int>& p){
		 return p.first == idAdj;
	});
	if (element != adjacentNodes.end())
	{
		adjacentNodes.erase(element);
		return true;
	}
	else
	{
		return false;
	}
}

void NodeWeighted::moveToCache(unordered_map<string , uint32_t> *mapFrecuency, uint16_t minFreq)
{
	for (auto it = (adjacentNodes.end() - 1); it != adjacentNodes.begin(); it--)
	{ // eliminar freq 1
		string it_id = to_string((*it).first) + "," + to_string((*it).second);
		if (mapFrecuency->at(it_id) <= minFreq)
		{
			cacheNodes.push_back(*it);
			adjacentNodes.erase(it);
		}
		else
			break;
	}
}

void NodeWeighted::deleteExtracted(vector<uInt>* C)
{
	std::sort(C->begin(), C->end());
	sort();
	vector<pair<uInt, uInt>> newAdj; 
	for(auto i : *C){
		int index = 0;
		while(index < adjacentNodes.size()){
			if(adjacentNodes.at(index).first != i){
				newAdj.push_back(adjacentNodes.at(index));
			}
			index++;
		}
		
	}
}

uInt NodeWeighted::getFrontAdjacent()
{
	if(not adjacentNodes.empty()) {
		return adjacentNodes.front().first;
	}
	return -1;
}

pair<uInt, uInt> NodeWeighted::getFrontWeighted()
{
	if (not adjacentNodes.empty()){
		return adjacentNodes.front();
	}
	return make_pair(-1,-1);
}
bool NodeWeighted::restore()
{
	while (not cacheNodes.empty())
	{
		adjacentNodes.push_back(cacheNodes.back());
		cacheNodes.pop_back();
	}
	sort();
	return true;
}

void NodeWeighted::print()
{
	cout << "Node " << id << ": ";
	for (size_t i = 0; i < adjacentNodes.size(); i++)
		cout <<"("<< adjacentNodes.at(i).first << "," << adjacentNodes.at(i).second << ") ";
	if (cacheNodes.size() > 0)
		cout << " || ";
	for (size_t j = 0; j < cacheNodes.size(); j++)
		cout <<"("<< cacheNodes.at(j).first << "," << cacheNodes.at(j).second << ") ";
	cout << endl;
}

WeightedIterator NodeWeighted::adjacentsBegin()
{
	return adjacentNodes.begin();
}

WeightedIterator NodeWeighted::adjacentsEnd()
{
	return adjacentNodes.end();
}

// PRIVATE METHODS

bool NodeWeighted::sortFrecuencyComp(const pair<uInt, uInt> &a, const pair<uInt, uInt> &b, unordered_map<string, uint32_t> *mapFrecuency)
{
	string a_id = to_string(a.first) + "," + to_string(a.second);
	string b_id = to_string(b.first) + "," + to_string(b.second);
	if (mapFrecuency->at(a_id) > mapFrecuency->at(b_id))
	{
		return true;
	}
	else if (mapFrecuency->at(a_id) == mapFrecuency->at(b_id))
	{
		return a.first < b.first;
	}
	else
	{
		return false;
	}
}

bool NodeWeighted::sortWeighted(const pair<uInt, uInt> &a, const pair<uInt, uInt> &b){
	return a.first < b.first; 
}


