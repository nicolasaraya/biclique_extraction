#include <GraphStd.hpp>
#include <Utils.hpp>
#include <Biclique.hpp>
#include <Cluster.hpp>
#include <Trie.hpp>
#include <AttrMgr.hpp>

#include <cassert>
#include <memory>

void GraphStd::extract()              
{
  _numSignatures = AttrMgr::get().numSignatures();
  _minClusterSize = AttrMgr::get().minClusterSize();
  _minAdyNodes = AttrMgr::get().minAdyNodes();
  _bicliqueSize = AttrMgr::get().bicliqueSize();
  _bsDecrease = AttrMgr::get().bsDecrease();
  _shingleSize = AttrMgr::get().shingleSize();
  _threshold = AttrMgr::get().threshold();
  _maxIterations = AttrMgr::get().maxIterations();

  std::ofstream file;
  _pathLog =  utils::modify_path(getPath(), 4 ,"_log.txt");
  file.open(_pathLog, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero log
  file.close();

  _pathBicliques = utils::modify_path(getPath(), 4 ,"_bicliques.txt");
  file.open(_pathBicliques, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero bicliques y se cierra
  file << maxValueEdge() << std::endl;
  file.close();

  compress();
}

void GraphStd::compress()
{
  uint64_t adjacency_matrixOriginalSize = size();
  uint64_t adjacency_matrixOriginalEdgesSize = all_edges_size();
  uint64_t num_edges = adjacency_matrixOriginalEdgesSize;
  
  uint16_t currIteration = 1;

  TIMERSTART(extraction_biclique);
  while (currIteration <= _maxIterations) {
    if (utils::SigHnd::get_state()) {
      std::cout << "Interrupt in " << currIteration << " iteration" << std::endl;
      break;
    }
    std::cout << std::endl << std::endl << "Iteration: " << currIteration << std::endl;
    std::cout << "Compute Shingles" << std::endl;
    TIMERSTART(compute_shingles);
    auto signatures = computeShingles();
    TIMERSTOP(compute_shingles);
    assert(not signatures->empty());

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

    _totalBiclique += _n_bicliques_iter;
    uint64_t current_edges = all_edges_size();
    double compressionPercentage = 100 - ((double(current_edges) * 100) / double(num_edges));

    std::cout << "Biclique iteration: " << _n_bicliques_iter << std::endl;
    std::cout << "Total Bicliques founded: " << _totalBiclique << std::endl;
    std::cout << "Compression percentage iteration: " << compressionPercentage << std::endl;

    std::ofstream file;
    file.open(_pathLog, std::fstream::app);
    file << "****************************************************************" << std::endl;
    file << "Iteration nº: " << currIteration << std::endl;
    file << "Adjacency _matrix Size: " << size() << std::endl;
    file << "Current Edges Size Adjacency_matrix: " << current_edges << std::endl;
    file << "Min Biclique Size: " << _bicliqueSize << std::endl;
    file << "Clusters found: " << _clusterSize << std::endl;
    file << "Bicliques found: " << _n_bicliques_iter << std::endl;
    file << "Compression percentage iteration: " << compressionPercentage << std::endl;
    file.close();

    num_edges = current_edges; 

    if (_n_bicliques_iter < _threshold) {
      if (_bsDecrease > _bicliqueSize) {
        break;
      }
      else if(_bicliqueSize == _min_sxc_size) {
        break;
      } else {
        _bicliqueSize -= _bsDecrease;
        if (_bicliqueSize < _min_sxc_size) {
          _bicliqueSize = _min_sxc_size;
          //break; 
        }
      }
    }
    currIteration += 1;
    _n_bicliques_iter = 0;
  }

  TIMERSTOP(extraction_biclique);

  double compressionPercentage = (double(num_edges) * 100) / double(adjacency_matrixOriginalEdgesSize);

  std::ofstream file;
  file.open(_pathLog, std::fstream::app);
  file << "****************************************************************" << std::endl;
  file << "Original Size Adjacency_matrix: " << adjacency_matrixOriginalSize << std::endl;
  file << "Current Size Adjacency_matrix: " << size() << std::endl
        << std::endl;
  file << "Original Edges Size Adjacency_matrix: " << adjacency_matrixOriginalEdgesSize << std::endl;
  file << "Current Edges Size Adjacency_matrix: " << all_edges_size() << std::endl
        << std::endl;
  file << "Number of Bicliques Extracted: " << _totalBiclique << std::endl;
  file << "Sum of S: " << _biclique_s_size << std::endl;
  file << "Sum of C: " << _biclique_c_size << std::endl;
  file << "Sum of S + C: " << _biclique_s_size + _biclique_c_size << std::endl;
  file << "Sum of Multiplication of S x C: " << _biclique_sxc_size << std::endl;
  file << " | S x C | / | S + C |: " << float(_biclique_sxc_size) / float(_biclique_s_size + _biclique_c_size) << std::endl;
  file << "Compression percentage: " << compressionPercentage << std::endl;
  file.close();

  std::cout << "****************************************************************" << std::endl;
  std::cout << "Original Size Adjacency_matrix: " << adjacency_matrixOriginalSize << std::endl;
  std::cout << "Current Size Adjacency_matrix: " << size() << std::endl
        << std::endl;
  std::cout << "Original Edges Size Adjacency_matrix: " << adjacency_matrixOriginalEdgesSize << std::endl;
  std::cout << "Current Edges Size Adjacency_matrix: " << all_edges_size() << std::endl
        << std::endl;
  std::cout << "Number of Bicliques Extracted: " << _totalBiclique << std::endl;
  std::cout << "Sum of S: " << _biclique_s_size << std::endl;
  std::cout << "Sum of C: " << _biclique_c_size << std::endl;
  std::cout << "Sum of S + C: " << _biclique_s_size + _biclique_c_size << std::endl;
  std::cout << "Sum of Multiplication of S x C: " << _biclique_sxc_size << std::endl;
  std::cout << " | S x C | / | S + C |: " << float(_biclique_sxc_size) / float(_biclique_s_size + _biclique_c_size) << std::endl;
  std::cout << "Compression percentage: " << compressionPercentage << std::endl;

  if (_totalBiclique > 0) {
      setCompressed(true);
      writeAdjacencyList();
      writeBinaryFile();
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
  return groups; // Devuelve el unique_ptr, transfiriendo la propiedad
}

void GraphStd::computeClusters(Signatures* group, uint16_t column)
{       
  sortBySignatures(group, column);
  std::unique_ptr<Group> candidates = makeGroups(group, column);

  if(column == 0) std::cout << "num candidates: " << candidates->size() << std::endl;

  std::vector<NodePtr> discarted;

  for (auto& cluster : *candidates) {
      //printSignatures(*cluster);
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
  auto shingle = std::make_unique<Shingle>(_numSignatures, _minAdyNodes, _shingleSize);
  auto group = std::make_unique<Signatures>();
  for (auto i = begin(); i != end(); ++i) {
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
	uint64_t size = 0;
	for (const auto& it : _matrix)
	{
		size += it->edgesSize();
	}
	return size;
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
	_matrix.push_back(node);
	if (node->getBackAdjacent() > _maxEdge) {
		_maxEdge = node->getBackAdjacent();
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
	for (size_t i = 0; i < _matrix.size(); i++) {
		_matrix.at(i)->restore();
	}
}

double GraphStd::getAverageDegree()
{
	double res = 0;
	uint64_t count = 0;

	for (auto i : _matrix) {
		if (i->edgesSize() > 0) {
			res += i->edgesSize();
			count++;
		}
  }
	res = res / double(count); 
  return res;
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

	if (size() == _numNodes) {
		return at(node_id-1);
	}
	return binarySearch(0, size()-1, node_id);
}

NodePtr GraphStd::binarySearch(uInt l, uInt r, uInt node_id)
{
  if (r >= l) {
      uInt mid = l + (r - l) / 2;
      if (_matrix.at(mid)->getId() == node_id){
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
