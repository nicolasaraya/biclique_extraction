#include "Node.hpp"
using namespace std;

// PUBLIC METHODS

Node::Node(uInt id) : id(id) {}

Node::Node(uInt id, bool weighted) : id(id), weighted(weighted) {}

Node::~Node()
{
	adjacentNodes.clear();
	cacheNodes.clear();
	wAdjacentNodes.clear();
	wCacheNodes.clear();
}

uInt Node::getId() 
{
	return id; 
}

bool Node::hasSelfLoop()
{
	return selfLoop;
}

bool Node::isModified()
{
	return modified;
}

bool Node::isWeighted()
{
	return weighted; 
}

void Node::setModified(bool modified)
{
	this->modified = modified;
}
void Node::setSelfLoop(bool selfloop)
{
	this->selfLoop = selfloop;
}

uint64_t Node::edgesSize()
{
	if (weighted) {
		return wAdjacentNodes.size();
	} else {
		return adjacentNodes.size();
	}
}

void Node::addAdjacent(uInt id_adj)
{
	if (not weighted) {
		adjacentNodes.push_back(id_adj);
	}
}

void Node::addAdjacent(uInt id_adj, uInt weight_adj)
{
	if (adjacentNodes.empty()) {
		wAdjacentNodes.push_back(make_pair(id_adj, weight_adj));
		weighted = true; 
	}
}

bool Node::deleteAdjacent(uInt key)
{
	if(weighted) return false; 
	auto f = find(adjacentNodes.begin(), adjacentNodes.end(), key);
	if (f != adjacentNodes.end()) {
		adjacentNodes.erase(f);
		return true; 
	}
    return false;
}

bool Node::deleteAdjacent(uInt key, uInt w)
{
	if(not weighted) return false; 
	auto f = find(wAdjacentNodes.begin(), wAdjacentNodes.end(), make_pair(key,w));
	if (f != wAdjacentNodes.end()) {
		wAdjacentNodes.erase(f);
		return true; 
	}
    return false;
}

void Node::shrinkToFit()
{
	if (weighted) {
		wAdjacentNodes.shrink_to_fit();
	} else {
		adjacentNodes.shrink_to_fit();
	}
}

void Node::deleteExtracted(vector<uInt>* C)
{
	if (weighted) return;
	sort();
	vector<uInt> newAdj; 
	int index = 0;
	for(auto i : *C){
		while(index < adjacentNodes.size()){
			if(adjacentNodes.at(index) != i){
				newAdj.push_back(adjacentNodes.at(index));
			}
			index++;
		}
	}	
	adjacentNodes.swap(newAdj);
}

void Node::deleteExtracted(vector<pair<uInt, uInt>>* C)
{
	if (not weighted) return; 
	sort();
	vector<pair<uInt, uInt>> newAdj; 
	int index = 0;
	for(auto i : *C){
		while(index < wAdjacentNodes.size()){
			if(wAdjacentNodes.at(index).first != i.first or wAdjacentNodes.at(index).second != i.second){
				newAdj.push_back(wAdjacentNodes.at(index));
			}
			index++;
		}
	}
	wAdjacentNodes.swap(newAdj);
}

vector<uInt> Node::findToErase(vector<uInt> *C)
{
	vector<uInt> weights;
	if(not weighted) {
		vector<uInt> new_adjacentNodes;
		vector<uInt>::iterator it = C->begin();
		vector<uInt>::iterator it_end = C->end();
		for (size_t i = 0; i < adjacentNodes.size(); i++)
		{
			if (it == it_end)
			{
				new_adjacentNodes.push_back(adjacentNodes.at(i));
				continue;
			}

			if ((*it) == adjacentNodes.at(i))
				it++;
			else
				new_adjacentNodes.push_back(adjacentNodes.at(i));
		}
		adjacentNodes.swap(new_adjacentNodes);
		new_adjacentNodes.clear(); 
	} else {
		vector<pair<uInt, uInt>> new_adjacentNodes;
		auto it = C->begin();
		auto it_end = C->end();
		for (size_t i = 0; i < wAdjacentNodes.size(); i++)
		{
			if (it == it_end)
			{
				new_adjacentNodes.push_back(wAdjacentNodes.at(i));
				continue;
			}

			if ((*it) == wAdjacentNodes.at(i).first){
				weights.push_back(wAdjacentNodes.at(i).second);
				it++;
			}
			else {
				new_adjacentNodes.push_back(wAdjacentNodes.at(i));
			}
		}
		wAdjacentNodes.swap(new_adjacentNodes);
		new_adjacentNodes.clear();
	}
	return weights;
}

bool Node::findAdjacent(uInt num){
	if (not weighted and adjacentNodes.empty()) {
        return false;
	}
	if (weighted and wAdjacentNodes.empty()) {
        return false;
	}
	if (not weighted) { 
		return binarySearch(0, edgesSize()-1, num);
	} else {
		return binarySearchW(0, edgesSize()-1, num);
	}
}

bool Node::findAdjacent(uInt num, uInt w){
	if (not weighted or wAdjacentNodes.empty()) { 
		return false;
	} else {
		return binarySearchW(0, edgesSize()-1, num, w);
	}
}

bool Node::binarySearch(uint64_t l, uint64_t r, uInt num){
    if (r >= l) {
        uint64_t mid = l + (r - l) / 2;

        if (adjacentNodes[mid] == num) {
			return true;
		}
        if (adjacentNodes[mid] > num){
			if(mid == 0) {
				return false;
			}
            return binarySearch(l, mid - 1, num);
		}
        return binarySearch(mid + 1, r, num);
    }
    return false;
}

bool Node::binarySearchW(uint64_t l, uint64_t r, uInt num){
    if (r >= l) {
        uint64_t mid = l + (r - l) / 2;

        if (wAdjacentNodes[mid].first == num){
            return true;
		}
        if (wAdjacentNodes[mid].first > num){
			if(mid == 0){
				return false;
			}
            return binarySearchW(l, mid - 1, num);
		}
        return binarySearchW(mid + 1, r, num);
    }
    return false;
}

bool Node::binarySearchW(uint64_t l, uint64_t r, uInt num, uInt w){
    if (r >= l) {
        uint64_t mid = l + (r - l) / 2;
        if (wAdjacentNodes[mid].first == num and wAdjacentNodes[mid].second == w){
            return true;
		}
        if (wAdjacentNodes[mid].first > num){
			if(mid == 0){
				return false;
			}
            return binarySearchW(l, mid - 1, num, w);
		}
        return binarySearchW(mid + 1, r, num, w);
    }
    return false;
}

void Node::sort()
{
	if (edgesSize() == 0) {		
		return; 
	}
	if(not weighted){
		std::sort(adjacentNodes.begin(), adjacentNodes.end());
	} else {
		std::sort(wAdjacentNodes.begin(), wAdjacentNodes.end(), bind(&Node::sortWeighted, this, placeholders::_1, placeholders::_2)); 
	}

}

void Node::sortByFrecuency(unordered_map<uInt, uint32_t> *mapFrecuency)
{
	std::sort(adjacentNodes.begin(), adjacentNodes.end(), bind(&Node::sortFrecuencyComp, this, placeholders::_1, placeholders::_2, mapFrecuency));
}

void Node::sortByFrecuency(unordered_map<string, uint32_t> *mapFrecuency)
{
	std::sort(wAdjacentNodes.begin(), wAdjacentNodes.end(), bind(&Node::sortFrecuencyCompWeighted, this, placeholders::_1, placeholders::_2, mapFrecuency));
}

bool Node::includes(vector<uInt> *c)
{
	if(not weighted) {
		return std::includes(adjacentNodes.begin(), adjacentNodes.end(), c->begin(), c->end());
	}
	return std::includes(wAdjacentNodes.begin(), wAdjacentNodes.end(), c->begin(), c->end(), compareIncludes());
}

bool Node::removeAdjacent(uInt id_adj)
{
	if(not weighted){
		auto element = std::find(adjacentNodes.begin(), adjacentNodes.end(), id_adj);
		if (element != adjacentNodes.end()) {
			adjacentNodes.erase(element);
			return true;
		} else {
			return false;
		}
	} else {
		auto element = std::find_if(wAdjacentNodes.begin(), wAdjacentNodes.end(), [id_adj](const std::pair<int, int>& p){
		 	return p.first == id_adj;
		});
		if (element != wAdjacentNodes.end()) {
			wAdjacentNodes.erase(element);
			return true;
		} else {
			return false;
		}
	}
	
}

void Node::moveToCache(unordered_map<uInt, uint32_t> *mapFrecuency, uint16_t minFreq)
{
	for (auto it = (adjacentNodes.end() - 1); it != adjacentNodes.begin(); it--) { // eliminar freq 1
		if (mapFrecuency->at(*it) <= minFreq) {
			cacheNodes.push_back(*it);
			adjacentNodes.erase(it);
		} else {
			break;
		}
	}
}

void Node::moveToCache(unordered_map<string , uint32_t> *mapFrecuency, uint16_t minFreq)
{
	for (auto it = (wAdjacentNodes.end() - 1); it != wAdjacentNodes.begin(); it--) { // eliminar freq 1
		string it_id = to_string((*it).first) + "," + to_string((*it).second);
		if (mapFrecuency->at(it_id) <= minFreq) {
			wCacheNodes.push_back(*it);
			wAdjacentNodes.erase(it);
		} else {
			break;
		}
	}
}

uInt Node::getFrontAdjacent()
{
	if (not weighted and adjacentNodes.size() > 0) {
		return adjacentNodes.front();	
	} else if (weighted and wAdjacentNodes.size() > 0){
		return wAdjacentNodes.front().first;
	}
	return -1;
}

pair<uInt, uInt> Node::getFrontWeighted()
{
	if (weighted and not wAdjacentNodes.empty()){
		return wAdjacentNodes.front();
	}
	return make_pair(-1,-1);
}
bool Node::restore()
{
	if (not weighted){
		while (not cacheNodes.empty()) {
			adjacentNodes.push_back(cacheNodes.back());
			cacheNodes.pop_back();
		}
		sort();
	} else {
		while (not wCacheNodes.empty()) {
			wAdjacentNodes.push_back(wCacheNodes.back());
			wCacheNodes.pop_back();
		}
		sort();
	}
	
	return true;
}

void Node::print()
{
	if (not weighted) {
		cout << "Node " << id << ": ";
		for (size_t i = 0; i < adjacentNodes.size(); i++)
			cout << adjacentNodes.at(i) << " ";
		if (cacheNodes.size() > 0)
			cout << " || ";
		for (size_t j = 0; j < cacheNodes.size(); j++)
			cout << cacheNodes[j] << " ";
		cout << endl;

	} else {
		cout << "Node " << id << ": ";
		for (size_t i = 0; i < wAdjacentNodes.size(); i++)
			cout <<"("<< wAdjacentNodes.at(i).first << "," << wAdjacentNodes.at(i).second << ") ";
		if (cacheNodes.size() > 0)
			cout << " || ";
		for (size_t j = 0; j < wCacheNodes.size(); j++)
			cout <<"("<< wCacheNodes.at(j).first << "," << wCacheNodes.at(j).second << ") ";
		cout << endl;

	}
	
	
}

AdjacentsIt Node::adjacentsBegin()
{
	return adjacentNodes.begin();
}

AdjacentsIt Node::adjacentsEnd()
{
	return adjacentNodes.end();
}

WeightedIt Node::wAdjacentsBegin()
{
	return wAdjacentNodes.begin();
}

WeightedIt Node::wAdjacentsEnd()
{
	return wAdjacentNodes.end();
}

// PRIVATE METHODS

bool Node::sortFrecuencyComp(const uInt &a, const uInt &b, unordered_map<uInt, uint32_t> *mapFrecuency)
{
	if (mapFrecuency->at(a) > mapFrecuency->at(b)) {
		return true;
	} else if (mapFrecuency->at(a) == mapFrecuency->at(b)) {
		return a < b;
	} else {
		return false;
	}
}

bool Node::sortFrecuencyCompWeighted(const pair<uInt, uInt> &a, const pair<uInt, uInt> &b, unordered_map<string, uint32_t> *mapFrecuency)
{
	string a_id = to_string(a.first) + "," + to_string(a.second);
	string b_id = to_string(b.first) + "," + to_string(b.second);
	if (mapFrecuency->at(a_id) > mapFrecuency->at(b_id)) {
		return true;
	} else if (mapFrecuency->at(a_id) == mapFrecuency->at(b_id)) {
		return a.first < b.first;
	} else {
		return false;
	}
}

bool Node::sortWeighted(const pair<uInt, uInt> &a, const pair<uInt, uInt> &b){
	return a.first < b.first; 
}