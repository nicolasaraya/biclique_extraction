#include <Graph.hpp>
#include <GraphStd.hpp>
#include <Node.hpp>

#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include <zlib.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <numeric>

namespace Boolean 
{
  Graph::Graph(const std::string path, bool selfLoop)
  {
    _selfLoop = selfLoop;
    _path = path;
    TIMERSTART(build_matrix);
    if (_path.find(".txt" )!= std::string::npos) {
      buildTxt();
      _format = "txt";
    } else if (_path.find(".bin")!= std::string::npos) {
      buildBin();
      _format = "bin";
    } else {
      std::cout << "Invalid file format" << std::endl;
      exit(0);
    }
    TIMERSTOP(build_matrix);
  }

  Graph::Graph(const std::string path) : Graph(path, false) {;}

  // funcion que agrega los nodos intermedios que no contienen ninguna arista
  // para facilitar la busqueda de los nodos source
  void Graph::standardize(std::vector<uInt>* aux)
  {
    std::cout << "Standarize" << std::endl;
    std::vector<NodePtr> new_matrix;
    auto it = _matrix.rbegin();
    uint64_t last_node = back()->getId();
    for (uint64_t cont = last_node; cont > 0; ++it, --cont) {
      if (it != _matrix.rend() && cont == (*it)->getId()) {
        //continue;
        new_matrix.push_back(std::move(*it));
        _matrix.pop_back();
      } else {
        new_matrix.push_back(std::make_shared<Node>(cont));
        aux->push_back(cont-1);
        it--;
      }
    }

    for (size_t cont = last_node; cont > 0; --cont) {
      _matrix.push_back(std::move(new_matrix.back()));
      new_matrix.pop_back();
    }
  }

  void Graph::buildTxt()
  {
    _optimize = AttrMgr::get().optimize();
    std::ifstream file;
    file.open(_path);
    if (!file.is_open()) {
      std::cout << "No se puede leer fichero" << std::endl;
      std::cout << _path << std::endl;
      exit(0);
    }
    std::string line;
    uInt id;
    long long int countOut = 0;
    getline(file, line); // num nodes
    _originalNumNodes = atoi(line.c_str());
    //_numNodes = atoi(line.c_str());
    while (!file.eof()) {
      getline(file, line);
      auto adjacents = utils::splitString(line, " ");

      if (adjacents.empty()) {
        continue;
      }
      
      id = atoi(adjacents.at(0).c_str());

      NodePtr tempNode = std::make_shared<Node>(id);
      
      if (_selfLoop) {
        tempNode->setSelfLoop(true);
      }

      for (size_t i = 1; i < adjacents.size(); ++i) {
        uint64_t adjId = atoi(adjacents[i].c_str());
        tempNode->addAdjacent(adjId);
      }

      if (not tempNode->edgesSize()) {
        continue;
      }

      tempNode->shrinkToFit();
      tempNode->sort();
      
      insert(tempNode);
      
    }
    file.close();
    _matrix.shrink_to_fit();
    _numNodes = _matrix.size();
    _avgDegree = (double)_numEdges / (double)_numNodes;
    _density = _numEdges / (_numNodes * _numNodes);
    _originalNumEdges = _numEdges;
    std::cout << "nodes: " << _numNodes << ", edges: " << _numEdges << std::endl;
  }

  void Graph::buildBin()
  {
    _optimize = AttrMgr::get().optimize();
    std::ifstream file(_path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
      std::cout << "No se puede leer fichero" << std::endl;
      std::cout << _path << std::endl;
      exit(0);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        std::cout << size << " bytes readed" << std::endl;
    }

    std::string blob(buffer.data(), buffer.size());
    buffer.clear();

    std::istringstream iss(blob, std::ios::in | std::ios::binary);

     if (_path.find("delta16") != std::string::npos) {
      deserializeDelta16(iss);
    } else {
      deserialize(iss);
    }

    std::cout << "nodes: " << _matrix.size();
    std::cout << ", edges: " << all_edges_size() << std::endl;

    _matrix.shrink_to_fit();
    _numNodes = _matrix.size();
    _avgDegree = (double)_numEdges / (double)_numNodes;
    _density = _numEdges / (_numNodes * _numNodes);
    _originalNumEdges = _numEdges;
  }

  void Graph::writeAdjacencyList()
  {
    if (size() == 0){
      std::cout << "Matrix Empty" << std::endl;
      return;
    }
    std::ofstream file;
    //int count = 0;
    uint64_t matrix_size = size();
    std::string pathFile = utils::modify_path(_path, 4 ,".txt");
    if (_compressed) {
      pathFile = utils::modify_path(_path, 4 ,"_compressed.txt");
    } 
    std::cout << "Writing: " << pathFile<< std::endl;
    file.open(pathFile, std::ofstream::out | std::ofstream::trunc); // limpia el contenido del fichero

    file << _originalNumNodes << std::endl;

    for (uint64_t i = 0; i < matrix_size ; i++) {
      if (_matrix.at(i) == nullptr) {
        continue; 
      }

      file << _matrix.at(i)->getId() << ":";

      for (auto adj = _matrix.at(i)->adjacentsBegin(); adj != _matrix.at(i)->adjacentsEnd(); adj++) {
        //if (not matrix[i]->hasSelfLoop() and *adj == matrix[i]->getId()) { //si no tiene un selfloop natural 
        //	continue;
        //}
        file << " " << *adj;
      }
      file << std::endl;
    }
    file.close();
  }

  void Graph::serialize(std::ostream& os)
  {
    _matrix.shrink_to_fit();
    uint32_t size = 0;
    std::stringstream ss;
    for (const auto& node : _matrix) {
      if (not node) {
        continue;
      }
      node->serialize(ss);
      ++size;
    }

    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os << ss.rdbuf();
  }

  void Graph::serializeDelta16(std::ostream& os)
  {
    uint32_t numNodes = _matrix.size();
    os.write(reinterpret_cast<const char*>(&numNodes), sizeof(numNodes));
    for (const auto& node : _matrix) {
      if (node && node->edgesSize() > 0) {
        node->serializeDelta16(os);
      }
    }
  }

  void Graph::deserialize(std::istream& is) {
    uint32_t cnt;
    is.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));

    _matrix.reserve(cnt);

    while (cnt--) {
      auto node = Node::deserialize(is);
      if (node->edgesSize() > 0) {
        insert(node);
      } else {
        //std::cout << "skip:"<< node->getId();
        continue;
      }
    }
  }

  void Graph::deserializeDelta16(std::istream& is)
  {
    uint32_t numNodes = 0;
    is.read(reinterpret_cast<char*>(&numNodes), sizeof(numNodes));
    _matrix.clear();
    _matrix.reserve(numNodes);

    for (uint32_t i = 0; i < numNodes; ++i) {
      auto node = Node::deserializeDelta16(is); // Usa la versión delta16 de Node::deserialize
      if (node && node->edgesSize() > 0) {
        //std::cout << "Insert node: " << node->getId() << " with " << node->edgesSize() << " edges." << std::endl;
        insert(node);
      }
    }
  }

  void Graph::writeBinaryFile()
  {
    std::string pathFile = utils::modify_path(_path, 4 ,".bin");

    if (AttrMgr::get().useDelta()) {
      std::cout << "Using delta encoding for adjacency lists." << std::endl;
      pathFile = utils::modify_path(pathFile, 4 ,"_delta16.bin");
    }

    if (_compressed) {
      pathFile = utils::modify_path(pathFile, 4 ,"_compressed.bin");
    }

    std::ostringstream oss(std::ios::out | std::ios::binary);

    if (AttrMgr::get().useDelta()) {
      serializeDelta16(oss);
    } else {
      serialize(oss);
    }

    std::cout << "Writing: " << pathFile << std::endl;


    std::string raw = oss.str();

    std::ofstream file(pathFile,
                      std::ios::out | std::ios::binary);
    assert(file.is_open());
    file.write(raw.data(), raw.size());
    file.close();

    if (not _compressed) {
      uLong rawSize = raw.size();
      auto compBytes = std::filesystem::file_size(pathFile);
      auto edges = all_edges_size();
      double bpeRaw  = (rawSize * 8.0) / double(edges);
      double bpeGz   = (compBytes * 8.0) / double(edges);
      double BypeRaw  = (rawSize) / double(edges);
      double BypeGz   = (compBytes) / double(edges);

      std::cout
      << "Raw size     : " << rawSize  << " bytes\n"
      << "File compressed: " << compBytes << " bytes\n"
      << "Edges total  : " << edges     << "\n\n"
      << "Bits/edge raw: " << bpeRaw  << "\n"
      << "Bits/edge file : " << bpeGz   << "\n"
      << "Bytes/edge raw: " << BypeRaw  << "\n"
      << "Bytes/edge file : " << BypeGz   << "\n";
    }
  }

  void Graph::writeBicliqueBinary()
  {
    std::string pathFileBic = utils::modify_path(_pathBicliques, 4 ,".bin");

    const std::string& raw = ossBicliques.str();
    uLong rawSize = raw.size();

    std::cout << "Writing: " << pathFileBic << std::endl;

    std::ofstream file(pathFileBic, std::ios::out | std::ios::binary);
    file.write(raw.data(), raw.size());
    file.close();

    std::string pathFile = _path;
    if (AttrMgr::get().useDelta()) {
      std::cout << "Using delta encoding for adjacency lists." << std::endl;
      pathFile = utils::modify_path(_path, 4 ,"_delta16.bin");
    }

    pathFile = utils::modify_path(pathFile, 4 ,"_compressed.bin");
    auto compBytes = std::filesystem::file_size(pathFile) + std::filesystem::file_size(pathFileBic);
    auto edges = _originalNumEdges;
    double bpeRaw  = (rawSize * 8.0) / double(edges);
    double bpeGz   = (compBytes * 8.0) / double(edges);
    double BypeRaw  = (rawSize) / double(edges);
    double BypeGz   = (compBytes) / double(edges);

    std::cout
    << "Raw size     : " << rawSize  << " bytes\n"
    << "File compressed: " << compBytes << " bytes\n"
    << "Edges total  : " << edges     << "\n\n"
    << "Bits/edge raw: " << bpeRaw  << "\n"
    << "Bits/edge file : " << bpeGz   << "\n"
    << "Bytes/edge raw: " << BypeRaw  << "\n"
    << "Bytes/edge file : " << BypeGz   << "\n";
  }



  void Graph::writeBicliques(std::vector<BicliquePtr>& bicliques)
  {
    bool saveTxt = AttrMgr::get().saveTxt();
    bool saveBin = AttrMgr::get().saveBin();

    std::ofstream file;
    if (saveTxt) {
      file.open(_pathBicliques, std::fstream::app);
      assert(file.is_open());
    }
    

    for (auto& biclique : bicliques) {
      std::vector<NodePtr>& S = biclique->S;
      std::vector<uInt>& C = biclique->C; 

      uInt S_size = S.size();
      uInt C_size = C.size();
        
      if (S_size * C_size < AttrMgr::get().bicliqueSize()) {
        continue; 
      }

      std::sort(S.begin(), S.end(), std::bind(&GraphStd::sortS, this, std::placeholders::_1, std::placeholders::_2));
      std::sort(C.begin(), C.end(), std::bind(&Graph::sortC, this, std::placeholders::_1, std::placeholders::_2));

      if (saveTxt) {
        file << "S: ";
      }
      if (saveBin) {
        ossBicliques.write(reinterpret_cast<const char*>(&S_size), sizeof(S_size));
      }
      
      for (auto& it : S) {
        it->deleteExtracted(C);
        const uInt& id = it->getId();
        if (saveTxt) {
          file << it->getId() << " ";
        }
        if (saveBin) {
          ossBicliques.write(reinterpret_cast<const char*>(&id), sizeof(id));
        }
      }

      if (saveTxt) {
        file << std::endl << "C: ";
      }
      if (saveBin) {
        ossBicliques.write(reinterpret_cast<const char*>(&C_size), sizeof(C_size));
      }

      for (auto& it : C) {
        if (saveTxt) {
          file << it << " ";
        }
        if (saveBin) {
          ossBicliques.write(reinterpret_cast<const char*>(&it), sizeof(it));
        }
      }
      
      if (saveTxt) {
        file << std::endl;
      }

      //file << "SxC = " << C_size * S_size << endl;   
      //file << "SxC - C = " << (C_size * S_size) - C_size << endl; 
      _n_bicliques_iter += 1;
      _biclique_s_size += S_size;
      _biclique_s_sizeIter = S_size;

      _biclique_c_size += C_size;
      _biclique_c_sizeIter = C_size;
      
      _biclique_sxc_size += (S_size * C_size);
      _biclique_sxc_sizeIter = S_size * C_size;

      if (_keepBicliques) {
        _savedBicliques.push_back(std::move(biclique));
      }
    }
    if (saveTxt) {
      file.close();
    }
    return; 
  }



  bool Graph::sortC(const uInt& a, const uInt& b)
  {
      return a < b;
  }
};
