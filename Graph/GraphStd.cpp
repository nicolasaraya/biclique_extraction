#include <GraphStd.hpp>
#include <Utils.hpp>
#include <BicliqueExtracted.hpp>
#include <Cluster.hpp>
#include <Trie.hpp>
#include <AttrMgr.hpp>

#include <cassert>
#include <memory>
#include <cmath>
#include <sstream>


void GraphStd::extract()              
{
  _numSignatures = AttrMgr::get().numSignatures();
  _minClusterSize = AttrMgr::get().minClusterSize();
  //_minAdyNodes = AttrMgr::get().minAdyNodes();
  //_bicliqueSize = AttrMgr::get().bicliqueSize();
  _bsDecrease = AttrMgr::get().bsDecrease();
  _shingleSize = AttrMgr::get().shingleSize();
  _threshold = AttrMgr::get().threshold();
  _maxIterations = AttrMgr::get().maxIterations();
  _optimize = AttrMgr::get().optimize();

  std::ofstream file;
  _pathLog =  utils::modify_path(getPath(), 4 ,"_log.txt");
  file.open(_pathLog, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero log
  file.close();

  if (AttrMgr::get().saveTxt()) {
    _pathBicliques = utils::modify_path(getPath(), 4 ,"_bicliques.txt");
    file.open(_pathBicliques, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero bicliques y se cierra
    file << _numNodes << std::endl; //optional
    file.close();
  }

  compress();
}



void GraphStd::compress()
{
  uint64_t adjacency_matrixOriginalSize = size();
  uint64_t adjacency_matrixOriginalEdgesSize = all_edges_size();
  //uint64_t num_edges = adjacency_matrixOriginalEdgesSize;
  _numEdges = adjacency_matrixOriginalEdgesSize;
  uint16_t currIteration = 1;
  float currentCompression = 0.0;
  uint64_t n_bicliques_iter_prev = 0;

  if (_optimize) {
    showStats();
    std::cout << "Estimating _minAdyNodes with percentile: " << AttrMgr::get().lowerBound() << std::endl;
    auto minAdyNodes = computePercentile(AttrMgr::get().lowerBound());
    _minMinAdyNodes = computePercentile(10); 
    AttrMgr::get().set_minAdyNodes(minAdyNodes);
    
    //auto mid = _minAdyNodes + std::abs(_avgDegree - _minAdyNodes);
    // auto mid = computePercentile((AttrMgr::get().upperBound() + AttrMgr::get().lowerBound())/2);
    // _bicliqueSize = (_minAdyNodes * _minAdyNodes * 0.5);    
    
    // if (_bicliqueSize < _min_sxc_size) {
    //   _bicliqueSize = _min_sxc_size;
    // }
    
    auto per5 = computePercentile(5);
    AttrMgr::get().set_bicliqueSize(per5 * per5);
    std::cout << "Estimated _minAdyNodes: " << minAdyNodes << std::endl;
    //std::cout << "Estimated _bicliqueSize: " << _bicliqueSize << std::endl;
    AttrMgr::get().show();
  }

  TIMERSTART(extraction_biclique);
  while (currIteration <= _maxIterations) {
    
    if (utils::SigHnd::get_state()) {
      std::cout << "Interrupt in " << currIteration << " iteration" << std::endl;
      break;
    }

    TIMERSTART(iteration);
    std::cout << std::endl << std::endl << "Iteration: " << currIteration << std::endl;

    uint64_t initialEdges = _numEdges;
    _n_bicliques_iter = 0;

    std::cout << "Compute Shingles" << std::endl;
    TIMERSTART(compute_shingles);
    auto signatures = computeShingles();
    TIMERSTOP(compute_shingles);
    
    std::cout << "Signatures founded: " << signatures->size() << std::endl;
    if (signatures->empty()) {
      break;
    } 

    std::cout << "Compute Clusters and Extract Bicliques" << std::endl;
    TIMERSTART(compute_clusters_and_bicliques);
    sortBySignatures(signatures.get(), 0);
    computeClusters(signatures.get(), 0);
    TIMERSTOP(compute_clusters_and_bicliques);

    signatures.release();

    TIMERSTART(restaure_nodes);
    std::cout << "Restoring nodes" << std::endl;
    restoreNodes();
    TIMERSTOP(restaure_nodes);
    TIMERSTOP(iteration);

    _totalBiclique += _n_bicliques_iter;
    uint64_t current_edges = all_edges_size();
    currentCompression = (double(initialEdges - current_edges) * 100) / double(_originalNumEdges);
    _sumCompression += currentCompression;
    auto avgBicliques = (double)_totalBiclique / currIteration;

    std::ostringstream iter_summary;
    iter_summary << "****************************************************************" << std::endl;
    iter_summary << "Iteration nº: " << currIteration << std::endl;
    iter_summary << "Adjacency _matrix Size: " << size() << std::endl;
    iter_summary << "Current Edges Size Adjacency_matrix: " << current_edges << std::endl;
    iter_summary << "Min Biclique Size: " << AttrMgr::get().bicliqueSize() << std::endl;
    iter_summary << "Clusters found: " << _clusterSize << std::endl;
    iter_summary << "Bicliques found: " << _n_bicliques_iter << std::endl;
    iter_summary << "Total Bicliques founded: " << _totalBiclique << std::endl;
    iter_summary << "Compression percentage iteration: " << currentCompression << std::endl;
    iter_summary << "Total Compression percentage: " << _sumCompression << std::endl;

    std::ofstream file;
    file.open(_pathLog, std::fstream::app);
    file << iter_summary.str();
    file.close();

    std::cout << iter_summary.str();

    if (_optimize and _endOptimizations) {
      std::cout << "Ending optimizations: _endOptimizations == true" << std::endl;
      break;
    }

    if (_optimize and _n_bicliques_iter < avgBicliques * 0.8 and currIteration > 1) {
      auto minAdyNodes = AttrMgr::get().minAdyNodes() * 0.8;
      
      if (minAdyNodes <= _minMinAdyNodes) {
        minAdyNodes = _minMinAdyNodes;
        _endOptimizations = true;
      }
      AttrMgr::get().set_minAdyNodes(minAdyNodes);
      
      //auto mid = _minAdyNodes + std::abs(_avgDegree - _minAdyNodes);
      // _bicliqueSize = _bicliqueSize * 0.8;  
      
      // if (_bicliqueSize <= _min_sxc_size) {
      //   _bicliqueSize = _min_sxc_size;
      //   _optimize = false;
      // }
      
      // AttrMgr::get().set_bicliqueSize(_bicliqueSize);
      std::cout << "Estimated _minAdyNodes: " << minAdyNodes << std::endl;
      AttrMgr::get().show();
    }
    
    if (_n_bicliques_iter < AttrMgr::get().threshold() and not _optimize) {
      if (AttrMgr::get().bsDecrease() > AttrMgr::get().bicliqueSize()) {
        std::cout << "Ending iterations: _bsDecrease > _bicliqueSize (" << _bsDecrease << ">" << AttrMgr::get().bicliqueSize() << std::endl;
        break;
      }
      else if (AttrMgr::get().bicliqueSize() == _min_sxc_size) {
        std::cout << "Ending iterations: _bsDecrease = _bicliqueSize (" << _bsDecrease << "=" << AttrMgr::get().bicliqueSize() << std::endl;
        break;
      } else {
        std::cout << "_bicliqueSize deacreased from " << AttrMgr::get().bicliqueSize(); 
        auto bicliqueSize = AttrMgr::get().bicliqueSize() - AttrMgr::get().bsDecrease();
        if (bicliqueSize < _min_sxc_size) {
          bicliqueSize = _min_sxc_size;
          //break; 
        }
        AttrMgr::get().set_bicliqueSize(bicliqueSize);
        std::cout << " to " << AttrMgr::get().bicliqueSize() << std::endl;
      }
    }
    currIteration += 1;
    n_bicliques_iter_prev = _n_bicliques_iter;
  }

  TIMERSTOP(extraction_biclique);

  double compressionPercentage = (double(_originalNumEdges - all_edges_size()) * 100) / double(_originalNumEdges);

  std::ofstream file;
  std::ostringstream summary;
  summary << "****************************************************************" << std::endl;
  summary << "Original Size: " << adjacency_matrixOriginalSize
          << " | Current Size: " << size() << std::endl;
  summary << "Original Edges: " << adjacency_matrixOriginalEdgesSize
          << " | Current Edges: " << all_edges_size() << std::endl;
  summary << "Bicliques: " << _totalBiclique
          << " | S: " << _biclique_s_size
          << " | C: " << _biclique_c_size
          << " | S+C: " << (_biclique_s_size + _biclique_c_size)
          << " | SxC: " << _biclique_sxc_size << std::endl;
  summary << "SxC/S+C: " << (_biclique_s_size + _biclique_c_size > 0 ?
                  float(_biclique_sxc_size) / float(_biclique_s_size + _biclique_c_size) : 0)
          << " | Compression: " << compressionPercentage << "%" << std::endl;

  file.open(_pathLog, std::fstream::app);
  file << summary.str();
  file.close();

  std::cout << summary.str();

  if (_totalBiclique > 0) {
    setCompressed(true);
    if (AttrMgr::get().saveTxt()) {
      writeAdjacencyList();
    }
    if (AttrMgr::get().saveBin()) {
      writeBinaryFile();
      writeBicliqueBinary();
    }
  }
  return;
}

std::unique_ptr<Group> GraphStd::makeGroups(Signatures* group, int column)
{
  auto groups = std::make_unique<Group>(); 
  groups->push_back(std::make_unique<Signatures>());
  Signatures* partRawPtr = groups->back().get();

  for (auto& signNode : *group) {
    if (!partRawPtr->empty() && partRawPtr->front()->minHash.at(column) != signNode->minHash.at(column)) {
      groups->push_back(std::make_unique<Signatures>());
      partRawPtr = groups->back().get();
    }
    partRawPtr->emplace_back(signNode.release());
  }
  return groups;
}

void GraphStd::computeClusters(Signatures* group, uint16_t column)
{       
  sortBySignatures(group, column);
  std::unique_ptr<Group> candidates = makeGroups(group, column);

  if (column == 0) {
    std::cout << "num candidates: " << candidates->size() << std::endl;
  }

  std::vector<NodePtr> discarted;

  for (auto& cluster : *candidates) {
    uInt clusterSize = cluster->size();

    if (clusterSize >= _minClusterSize and column < _numSignatures - 1) {
      computeClusters(cluster.get(), column + 1);
    } else if (clusterSize > 1) {   
      std::vector<NodePtr> newCluster;
      for (auto& node : *cluster) {
          newCluster.push_back(node->ptrNode);
      }              
      extractBicliques(cluster.get()); 
    } else if (clusterSize == 1) {
      discarted.push_back( cluster->front()->ptrNode );
    }
  }

  if (discarted.size() > 1) {
    extractBicliques(&discarted);
  }
}

void GraphStd::extractBicliques(Signatures* cluster)
{
  Cluster c(cluster);
  _clusterSize++;
  c.computeTrie();
  getBicliques(c);
}

void GraphStd::extractBicliques(std::vector<NodePtr>* cluster)
{
  Cluster c(cluster);
  _clusterSize++;
  c.computeTrie();
  getBicliques(c);
}

void GraphStd::getBicliques(Cluster& cluster)
{   
  std::vector<BicliquePtr>& bicliques = cluster.getBicliques();
  if (bicliques.empty()) {
    return; 
  }
  writeBicliques(bicliques);
  return; 
}

std::unique_ptr<Signatures> GraphStd::computeShingles()
{

  auto shingle = std::make_unique<Shingle>(AttrMgr::get().numSignatures(), AttrMgr::get().minAdyNodes(), AttrMgr::get().shingleSize());
  auto group = std::make_unique<Signatures>();
  for (auto i = begin(); i != end(); ++i) {
    if ((*i)->edgesSize() < 0) {
      continue;
    }

    SignNode sn = shingle->computeShingle(*i);
    if (sn != nullptr) {
      group->emplace_back(sn.release());
    }
  }

  return group; // Devuelve el unique_ptr, transfiriendo la propiedad
}

void GraphStd::printSignatures(const Signatures& group)
{
  for (auto& i : group) {
    std::cout << i->ptrNode->getId();
    for (int j = 0; j < _numSignatures; j++) {
        std::cout << " | MH[" << j << "]: " << i->minHash.at(j);
    }
    std::cout << std::endl;
  }
}

void GraphStd::sortBySignatures(Signatures* group, const uint16_t index)
{
  std::sort(group->begin(), group->end(), std::bind(&GraphStd::compareMinHash, this, std::placeholders::_1, std::placeholders::_2, index));
}

void GraphStd::sortBicliques(std::vector<BicliquePtr>& bicliques)
{
  std::sort(bicliques.begin(), bicliques.end(), std::bind(&GraphStd::compareBicliqueRank, this, std::placeholders::_1, std::placeholders::_2));
}

bool GraphStd::sortS(const NodePtr& a, const NodePtr& b)
{
  return a->getId() < b->getId();
}

bool GraphStd::compareMinHash(const SignNode& a, const SignNode& b, uint16_t index)
{
  return (a->minHash.at(index) < b->minHash.at(index));
}

bool GraphStd::compareBicliqueRank(const BicliquePtr& a, const BicliquePtr& b)
{
  if(not a->C.empty() and not b->C.empty()){
    return a->S.size() * a->C.size() < b->S.size() * b->C.size();
  } else {
    return a->S.size() * a->C_w.size() < b->S.size() * b->C_w.size();
  }
  return false; 
}

uint64_t GraphStd::size()
{
  return _matrix.size();
}

NodePtr GraphStd::back()
{
  return _matrix.back();
}

uint64_t GraphStd::all_edges_size()
{
  _numEdges = 0;
	for (const auto& it : _matrix)
	{
		_numEdges += it->edgesSize();
	}
	return _numEdges;
}

std::string GraphStd::getPath()
{
  return _path;
};

void GraphStd::setPath(std::string path)
{
  _path = path;
}

void GraphStd::print()
{
	for (size_t i = 0; i < _matrix.size(); ++i) {
		if (_matrix.at(i)->edgesSize() > 0) _matrix.at(i)->print();
  }
}

uint64_t GraphStd::maxValueEdge()
{
  return _maxEdge;
}

bool GraphStd::isTransposed()
{
  return _transposed;
}

void GraphStd::setCompressed(bool b){
  _compressed = b;
}

void GraphStd::printAsMatrix()
{
	uInt temp = 0;
	for (size_t i = 0; i < _matrix.size(); ++i) {
		while(temp < _matrix.at(i)->getId() ) {
			std::cout << "Node " << temp << ": " << 0 << std::endl;
			temp++; 
		}
		temp++;
		_matrix.at(i)->printBinary();
	}
}

void GraphStd::insert(NodePtr node)
{
  size_t edges = node->edgesSize();
  
  if (edges == 0) {
    return; // No insert nodes with no edges
  }

	_matrix.push_back(node);
	if (node->getBackAdjacent() > _maxEdge) {
		_maxEdge = node->getBackAdjacent();
	}

  _numEdges += edges;

  if (_optimize) {
    _distribution[edges]++;
  }
  if (_minDegree > edges) {
    _minDegree = edges;
  } 

  if (_maxDegree < edges) {
    _maxDegree = edges;
  }

}

GraphIterator GraphStd::begin()
{
	return _matrix.begin();
}

GraphIterator GraphStd::end()
{
	return _matrix.end();
}

void GraphStd::restoreNodes()
{
  if (_optimize) {
    _distribution.clear();
    _numEdges = 0;
    _numNodes = 0;
    _avgDegree = 0;
    _minDegree = UINT64_MAX;
    _maxDegree = 0;
  }

	for (size_t i = 0; i < _matrix.size(); i++) {
		_matrix.at(i)->restore();
    size_t degree = _matrix.at(i)->edgesSize();
    if (degree == 0) {
      continue;
    }
    if (_optimize) {
      _distribution[degree]++;
      if (_minDegree > degree) {
        _minDegree = degree;
      }

      if (_maxDegree < degree) {
        _maxDegree = degree;
      }
      
    }
    _numNodes++;
    _numEdges+= degree;
	}
  if (_optimize) {
    _avgDegree += (double)_numEdges / (double)_numNodes;
  }
}

double GraphStd::getAverageDegree()
{
  _avgDegree = 0;
	uint64_t count = 0;

	for (auto i : _matrix) {
		if (i->edgesSize() > 0) {
			_avgDegree += i->edgesSize();
			count++;
		}
  }
	_avgDegree = _avgDegree / double(count); 
  return _avgDegree;
}

NodePtr GraphStd::at(uInt pos)
{
	return _matrix.at(pos);
}

NodePtr GraphStd::find(uInt node_id)
{
	if(_matrix.empty()) {
		return nullptr;
	}

	return binarySearch(0, size()-1, node_id);
}

NodePtr GraphStd::binarySearch(uInt l, uInt r, uInt node_id)
{
  if (r >= l) {
    uInt mid = l + (r - l) / 2;
    if (_matrix.at(mid)->getId() == node_id) {
      return _matrix.at(mid);
    }
    if (_matrix.at(mid)->getId() > node_id) {
      if (mid == 0) {
        return nullptr;
      }
      return binarySearch(l, mid - 1, node_id);
    }
    return binarySearch(mid + 1, r, node_id);
  }
  return nullptr;
}

void GraphStd::sort() 
{
	std::sort(_matrix.begin(), _matrix.end(), std::bind(&GraphStd::compareNodes, this, std::placeholders::_1, std::placeholders::_2));
}


bool GraphStd::compareNodes(const NodePtr& a, const NodePtr& b)
{
  return a->getId() < b->getId();
}

void GraphStd::transpose()
{
	std::vector<NodePtr> t__matrix; 

	for(auto i : _matrix) {
		if (i->edgesSize() == 0){
			continue;
		} 

		while (t__matrix.size() <= (i->getBackWeighted())->first) {
			t__matrix.push_back(std::make_shared<Node>(t__matrix.size(), true));
		}

		for (auto it = i->wAdjacentsBegin(); it != i->wAdjacentsEnd(); it++) {
			t__matrix.at((*it).first)->addAdjacent(i->getId(), (*it).second);
		}
	}
	_matrix.clear();

	for (auto i : t__matrix) {
		if (i->edgesSize() > 0) {
			_matrix.push_back(i);
		} else {
		}
	}
	t__matrix.clear();

	sort();
	
	_transposed = not _transposed; 
}

void GraphStd::dfs(NodePtr node, std::unordered_set<uInt> visited, std::vector<NodePtr>& component)
{
  visited.insert(node->getId());
  component.push_back(node);
  for (auto adjacent = node->adjacentsBegin(); adjacent != node->adjacentsEnd(); adjacent++){
    if (visited.find((*adjacent)) == visited.end()) {
        dfs(find(*adjacent), visited, component);
    }
  }
}

uInt GraphStd::computePercentile(float percentile)
{
  size_t accumulated = 0;
  size_t threshold = (percentile/100) * _numNodes;
  assert(not _distribution.empty());

  std::vector<uInt> degrees;
  for (const auto& [degree, count] : _distribution) {
      degrees.push_back(degree);
  }
  std::sort(degrees.begin(), degrees.end());

  for (const auto& degree : degrees) {
      accumulated += _distribution.at(degree);
      if (accumulated >= threshold) {
          return degree;
      }
  }

  return degrees.back();
}

void GraphStd::showStats()
{
  std::cout << "_avgDegre = " << _avgDegree << std::endl;
  std::cout << "_minDegree = " << _minDegree << std::endl;
  std::cout << "_maxDegree = " << _maxDegree << std::endl;
  std::cout << "_density = " << _density << std::endl;
}