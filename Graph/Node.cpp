#include <Node.hpp>

#include <functional>
#include <cassert>
#include <zlib.h>
#include <cstring>

Node::Node(uInt id) : id(id), weighted(false) {}

Node::Node(uInt id, bool weighted) : id(id), weighted(weighted) {}

Node::Node(std::istream& is) 
{
  deserialize(is);
}


Node::~Node()
{
	if (not adjacentNodes.empty()) adjacentNodes.clear();
	if (not cacheNodes.empty()) cacheNodes.clear();
	if (not wAdjacentNodes.empty()) wAdjacentNodes.clear();
	if (not wCacheNodes.empty()) wCacheNodes.clear();

}

uInt Node::getId() const
{
	return id; 
}

bool Node::hasSelfLoop()
{
	return naturalSelfLoop;
}

/*
bool Node::isModified()
{
	return modified;
}
*/

bool Node::isWeighted()
{
	return weighted; 
}

/*
void Node::setModified(bool modified)
{
	this->modified = modified;
}
*/
void Node::setSelfLoop(bool selfloop)
{
	if (weighted) return; 
	addAdjacent(id);
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
		if(selfLoop and id_adj == id) {
			naturalSelfLoop = true; 
			return;
		}
		adjacentNodes.push_back(id_adj);
		sorted = false;
	}
}

void Node::addAdjacent(uInt id_adj, uInt weight_adj)
{
	if (adjacentNodes.empty()) {
		if (wAdjacentNodes.size() > 0 and id_adj < wAdjacentNodes.back().first) sorted = false;
		wAdjacentNodes.push_back(Pair(id_adj, weight_adj));
		weighted = true; 
	}
}

bool Node::deleteAdjacent(uInt key)
{
	if(weighted) return false; 
	auto f = std::find(adjacentNodes.begin(), adjacentNodes.end(), key);
	if (f != adjacentNodes.end()) {
		adjacentNodes.erase(f);
		sorted = false;
		return true; 
	}
    return false;
}

bool Node::deleteAdjacent(uInt key, uInt w)
{
	if(not weighted) return false; 
	auto f = std::find_if(wAdjacentNodes.begin(), wAdjacentNodes.end(), [key, w](Pair& p){
		 	return (p.first == key) and (w == p.second);
		});
	if (f != wAdjacentNodes.end()) {
		wAdjacentNodes.erase(f);
		sorted = false;
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

void Node::deleteExtracted(std::vector<uInt>& C)
{
	if (weighted) return;
	if (not sorted) sort();
	std::vector<uInt> newAdj; 
	uInt index = 0;

	for(auto& i : C){
		for(; index < adjacentNodes.size(); index++) {
			if(adjacentNodes.at(index) == i) {
				index++;
				break; 
			} else {
				newAdj.push_back(adjacentNodes.at(index));
			}
		}
	}

	while(index < adjacentNodes.size()) {
		newAdj.push_back(adjacentNodes.at(index));
		index++;
	}	
	adjacentNodes.swap(newAdj);
}

void Node::deleteExtracted(std::vector<Pair>& C)
{
	if (not weighted) return; 
	if (not sorted) sort();
	//print();
	std::vector<Pair> newAdj; 
	uInt index = 0;
	//int countC = 0; 
	for(auto& i : C){
		for(; index < wAdjacentNodes.size(); index++) {
			if(wAdjacentNodes.at(index).first == i.first and wAdjacentNodes.at(index).second == i.second) {
				index++;
				break; 
			} else {
				newAdj.push_back(wAdjacentNodes.at(index));
			}
		}
	}

	while(index < wAdjacentNodes.size()) {
		newAdj.push_back(wAdjacentNodes.at(index));
		index++;
	}

	wAdjacentNodes.swap(newAdj); 
	//sleep(60);
}
/*
vector<uInt> Node::findToErase(vector<uInt> *C)
{
	vector<uInt> weights;
	if(not weighted) {
		vector<uInt> new_adjacentNodes;
		vector<uInt>::iterator it = C->begin();
		vector<uInt>::iterator it_end = C->end();
		for (size_t i = 0; i < adjacentNodes.size(); i++) {
			if (it == it_end) {
				new_adjacentNodes.push_back(adjacentNodes.at(i));
				continue;
			}

			if ((*it) == adjacentNodes.at(i)) {
				it++;
			} else { 
				new_adjacentNodes.push_back(adjacentNodes.at(i));
			}
		}
		adjacentNodes.swap(new_adjacentNodes);
		new_adjacentNodes.clear(); 
	} else {
		vector<pair<uInt, uInt>> new_adjacentNodes;
		auto it = C->begin();
		auto it_end = C->end();
		for (size_t i = 0; i < wAdjacentNodes.size(); i++) {
			if (it == it_end) {
				new_adjacentNodes.push_back(wAdjacentNodes.at(i));
				continue;
			}

			if ((*it) == wAdjacentNodes.at(i).first){
				weights.push_back(wAdjacentNodes.at(i).second);
				it++;
			} else {
				new_adjacentNodes.push_back(wAdjacentNodes.at(i));
			}
		}
		wAdjacentNodes.swap(new_adjacentNodes);
		new_adjacentNodes.clear();
	}
	return weights;
}*/

bool Node::findAdjacent(uInt num){
	if (not weighted and adjacentNodes.empty()) {
        return false;
	}
	if (weighted and wAdjacentNodes.empty()) {
        return false;
	}
	if(not sorted) sort();
	if (not weighted) { 
		return binarySearch(0, edgesSize()-1, num);
	} else {
		return binarySearchW(0, edgesSize()-1, num);
	}
}

uInt Node::findAdjacentWeighted(uInt num){
	if (not weighted) return 0;
	auto it = std::find_if(wAdjacentNodes.begin(), wAdjacentNodes.end(),
              [&](Pair& pair) { return pair.first == num; });	
	if (it != wAdjacentNodes.end()) return (*it).second;
	return 0;
}

bool Node::increaseWeight(uInt id, uInt increase)
{
	if (not weighted) return false;
	auto it = std::find_if(wAdjacentNodes.begin(), wAdjacentNodes.end(),
              [&](Pair& pair) { return pair.first == id; });
	if (it != wAdjacentsEnd()) {
		//cout << "Encuentra el nodo adj " << id << "en el nodo " << id << endl; 
		//cout << "previo valor: " << (*it).second << endl;
		(*it).second += increase;
		//cout << "nuevo valor: " << (*it).second << endl; 
		 return true; 
	}

    return false;
}

bool Node::findAdjacent(uInt num, uInt w){
	if (not weighted or wAdjacentNodes.empty()) { 
		return false;
	} else {
		if (not sorted) sort();
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
		std::sort(wAdjacentNodes.begin(), wAdjacentNodes.end(), std::bind(&Node::sortWeighted, this, std::placeholders::_1, std::placeholders::_2)); 
	}
	sorted = true;

}

void Node::sortByFrecuency(std::unordered_map<uInt, uint32_t> *mapFrecuency)
{
	std::sort(adjacentNodes.begin(), adjacentNodes.end(), std::bind(&Node::sortFrecuencyComp, this, std::placeholders::_1, std::placeholders::_2, mapFrecuency));
	sorted = false;
}

void Node::sortByFrecuency(std::unordered_map<std::string, uint32_t> *mapFrecuency)
{
	std::sort(wAdjacentNodes.begin(), wAdjacentNodes.end(), bind(&Node::sortFrecuencyCompWeighted, this, std::placeholders::_1, std::placeholders::_2, mapFrecuency));
	sorted = false;
}

bool Node::includes(std::vector<uInt> *c)
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
			sorted = false;
			return true;
		} else {
			return false;
		}
	} else {
		auto element = std::find_if(wAdjacentNodes.begin(), wAdjacentNodes.end(), [id_adj](Pair& p){
		 	return p.first == id_adj;
		});
		if (element != wAdjacentNodes.end()) {
			wAdjacentNodes.erase(element);
			sorted = false;
			return true;
		} else {
			return false;
		}
	}
	
}

void Node::moveToCache(std::unordered_map<uInt, uint32_t> *mapFrecuency, uint16_t minFreq)
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

void Node::moveToCache(std::unordered_map<std::string, uint32_t> *mapFrecuency, uint16_t minFreq)
{
	for (auto it = (wAdjacentNodes.end() - 1); it != wAdjacentNodes.begin(); it--) { // eliminar freq 1
		std::string it_id = std::to_string((*it).first) + "," + std::to_string((*it).second);
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

uInt Node::getBackAdjacent()
{
    if (not weighted and adjacentNodes.size() > 0) {
		return adjacentNodes.back();	
	} else if (weighted and wAdjacentNodes.size() > 0){
		return wAdjacentNodes.back().first;
	}
	return 0;
}

Pair* Node::getFrontWeighted()
{
	if (weighted and not wAdjacentNodes.empty()){
		return &wAdjacentNodes.front();
	}
	return nullptr;
}

Pair* Node::getBackWeighted()
{
	if (weighted and not wAdjacentNodes.empty()){
		return &(wAdjacentNodes.back());
	}
	return nullptr;
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
	std::cout << "Node " << id << ": ";
	if (not weighted) {
		for (size_t i = 0; i < adjacentNodes.size(); i++)
			std::cout << adjacentNodes.at(i) << " ";
		if (cacheNodes.size() > 0)
			std::cout << " || ";
		for (size_t j = 0; j < cacheNodes.size(); j++)
			std::cout << cacheNodes[j] << " ";
	} else {
		for (size_t i = 0; i < wAdjacentNodes.size(); i++) {
			std::cout <<"("<< wAdjacentNodes.at(i).first << "," << wAdjacentNodes.at(i).second << ") ";
		}
		if (cacheNodes.size() > 0) {
			std::cout << " || ";
		}
		for (size_t j = 0; j < wCacheNodes.size(); j++) {
			std::cout <<"("<< wCacheNodes.at(j).first << "," << wCacheNodes.at(j).second << ") ";
		}
	}
	std::cout << std::endl;
}

void Node::printBinary()
{
	std::cout << "Node " << id << ": ";
	uInt index = 0; 
	uInt i = 1;

	if (not weighted) {
		while(index < adjacentNodes.size()) {
			while(i < adjacentNodes.at(index) ) {
				std::cout << "0 ";
				i++;
			}
			std::cout << "1 ";
			index++; 
			i++;
		}
	} else {
		while(index < wAdjacentNodes.size()) {
			while(i < wAdjacentNodes.at(index).first ) {
				std::cout << "0 ";
				i++;
			}
			std::cout << wAdjacentNodes.at(index).second << " ";
			index++; 
			i++;
		}
	}
	std::cout << std::endl;
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

bool Node::sortFrecuencyComp(const uInt &a, const uInt &b, std::unordered_map<uInt, uint32_t> *mapFrecuency)
{
	if (mapFrecuency->at(a) > mapFrecuency->at(b)) {
		return true;
	} else if (mapFrecuency->at(a) == mapFrecuency->at(b)) {
		return a < b;
	} else {
		return false;
	}
}

bool Node::sortFrecuencyCompWeighted(Pair& a, Pair& b, std::unordered_map<std::string, uint32_t> *mapFrecuency)
{
	try {
		std::string a_id = std::to_string(a.first) + "," + std::to_string(a.second);
		std::string b_id = std::to_string(b.first) + "," + std::to_string(b.second);
		if (mapFrecuency->at(a_id) > mapFrecuency->at(b_id)) {
			return true;
		} else if (mapFrecuency->at(a_id) == mapFrecuency->at(b_id)) {
			return a.first < b.first;
		} else {
			return false;
		}
	} catch (std::exception &e) {
		std::cout << "a ref: " << &a << std::endl;
		std::cout << "A: " << a.first << "," << a.second << std::endl;
		std::cout << "B: " << b.first << "," << b.second << std::endl;
		std::cout << e.what() << std::endl;
		 
	}
	return false;
}

bool Node::sortWeighted(Pair& a, Pair& b)
{
	return a.first < b.first; 
}

bool Node::serialize(std::ostream& os) 
{
  // 1) ID
  shrinkToFit();

  uInt n = adjacentNodes.size();

  if (n == 0) {
    return false;
  }

  os.write(reinterpret_cast<const char*>(&id), sizeof(id));

  // 2) Cantidad de adyacentes
  os.write(reinterpret_cast<const char*>(&n), sizeof(n));

  // 3) Vector de adyacentes (bloque contiguo)
  os.write(reinterpret_cast<const char*>(adjacentNodes.data()), n * sizeof(uInt));

  return true;
  
}

void Node::serializeDelta16(std::ostream& os) 
{
  shrinkToFit();
  uInt n = adjacentNodes.size();
  if (n == 0) return;

  os.write(reinterpret_cast<const char*>(&id), sizeof(id));
  os.write(reinterpret_cast<const char*>(&n), sizeof(n));

  os.write(reinterpret_cast<const char*>(&adjacentNodes[0]), sizeof(uInt));

  // Ahora diferencias a partir del segundo
  for (size_t i = 1; i < n; ++i) {
    uInt diff = adjacentNodes[i] - adjacentNodes[i-1];
    uInt rest = diff;
    while (rest > std::numeric_limits<uint16_t>::max()) {
      uint16_t zero = 0;
      os.write(reinterpret_cast<const char*>(&zero), sizeof(zero));
      rest -= std::numeric_limits<uint16_t>::max();
    }
    uint16_t d16 = static_cast<uint16_t>(rest);
    os.write(reinterpret_cast<const char*>(&d16), sizeof(d16));
  }
}

std::shared_ptr<Node> Node::deserialize(std::istream& is) 
{
  // 1) ID
  auto temp = std::make_shared<Node>(0);
  is.read(reinterpret_cast<char*>(&temp->id), sizeof(temp->id));

  // 2) Cantidad de adyacentes
  uInt n;
  is.read(reinterpret_cast<char*>(&n), sizeof(n));

  // 3) Vector de adyacentes
  temp->adjacentNodes.resize(n);
  if (n > 0) {
    is.read(reinterpret_cast<char*>(temp->adjacentNodes.data()),
            n * sizeof(uInt));
  }
  //temp->print();
  return temp;
}

std::shared_ptr<Node> Node::deserializeDelta16(std::istream& is) 
{
  auto temp = std::make_shared<Node>(0);
  is.read(reinterpret_cast<char*>(&temp->id), sizeof(temp->id));

  uInt n;
  is.read(reinterpret_cast<char*>(&n), sizeof(n));

  temp->adjacentNodes.clear();
  temp->adjacentNodes.reserve(n);

  if (n == 0) return temp;

  uInt prev = 0;
  is.read(reinterpret_cast<char*>(&prev), sizeof(prev));
  temp->adjacentNodes.push_back(prev);

  for (uInt i = 1; i < n; ++i) {
    uInt diff = 0;
    while (true) {
      uint16_t d16 = 0;
      is.read(reinterpret_cast<char*>(&d16), sizeof(d16));
      if (d16 == 0) {
        diff += std::numeric_limits<uint16_t>::max();
      } else {
        diff += d16;
        break;
      }
    }
    prev += diff;
    temp->adjacentNodes.push_back(prev);
  }
  return temp;
}
