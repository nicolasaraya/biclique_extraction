#include <Graph.hpp>

#include <cassert>
#include <string>
#include <vector>
#include <fstream>

namespace Boolean 
{
  Graph::Graph(const std::string path, bool selfLoop)
  {
    _selfLoop = selfLoop;
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
    std::ifstream file;
    file.open(_path);
    if (!file.is_open()) {
      std::cout << "No se puede leer fichero" << std::endl;
      std::cout << _path << std::endl;
      exit(0);
    }
    std::string line;
    uInt id;
    getline(file, line); // num nodes
    _numNodes = atoi(line.c_str());
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
      tempNode->shrinkToFit();
      tempNode->sort();
      insert(tempNode);
      _numEdges += tempNode->edgesSize();
    }
    file.close();
    _matrix.shrink_to_fit();

    std::cout << "nodes: " << _numNodes << ", edges: " << _numEdges << std::endl;
  }

  void Graph::buildBin()
  {
    std::ifstream binFile; 
    binFile.open(_path, std::ios::in | std::ios::binary);    
    assert(binFile.is_open());

    Int* nodes = new Int(0); 
    binFile.read((char*)nodes, sizeof(Int)); //num_nodes
    //std::std::cout << "cantidad de nodos: " << *nodes << std::endl;
    _numNodes = *nodes;
    Int* buffer = new Int(0); 
    NodePtr tempNode = nullptr;
      while (binFile.read((char*)buffer, sizeof(Int))) {
      //std::cout << *buffer << std::endl;
        if((*buffer) < 0) {
          uint64_t id = (*buffer) * -1;
          if (tempNode != nullptr) {
            insert(tempNode);
            tempNode->sort();
            tempNode->shrinkToFit();
          }
          tempNode = std::make_shared<Node>(id);
          if (_selfLoop) {
            tempNode->setSelfLoop(true);
          }
        } else {
          if (tempNode == nullptr) {
            continue; //num de aristas
          }
          tempNode->addAdjacent(*buffer);
        }
      }

    if (tempNode != nullptr) {
      insert(tempNode);
      tempNode->sort();
      tempNode->shrinkToFit();
    }
    delete buffer;
    delete nodes;
    binFile.close();
    return;
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

    file << _numNodes << std::endl;

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

  void Graph::writeBinaryFile()
  {
    std::ofstream file;
    std::string pathFile = utils::modify_path(_path, 4 ,".bin");
    if (_compressed) {
      pathFile = utils::modify_path(_path, 4 ,"_compressed.bin");
    } 
    std::cout << "Writing: " << pathFile << std::endl;
    file.open(pathFile, std::ios::out | std::ios::binary | std::ios::trunc); 
    assert(file.is_open());
    
    Int size = _matrix.size();
    file.write((char*)&size, sizeof(Int));
    Int count_nodes = 0;
    Int count_edges = 0;
    for (const auto& i : _matrix) {
      if (i->edgesSize() == 0) {
        continue;
      }
      count_nodes++;
      Int id = i->getId() * -1;
      file.write((char*)&(id), sizeof(Int));
      count_edges += i->edgesSize();
      for (auto j = i->adjacentsBegin(); j != i->adjacentsEnd(); ++j) {
        //file << i->getId() << " " << j->first << " " << j->second << std::endl;
        file.write((char*)&(*j), sizeof(Int));
      }
    }

    file.seekp(0);
    file.write((char*)&size, sizeof(Int));
    //file.write((char*)&count_edges, sizeof(Int));
    file.close();
  }

  void Graph::writeBicliques(std::vector<BicliquePtr>& bicliques)
  {
    std::ofstream file;
    file.open(_pathBicliques, std::fstream::app);
    assert(file.is_open());
    for (auto& biclique : bicliques) {
      std::vector<NodePtr>& S = biclique->S;
      std::vector<uInt>& C = biclique->C; 

      uInt S_size = S.size();
      uInt C_size = C.size();
        
      if (S_size * C_size < _bicliqueSize) {
        continue; 
      }

      std::sort(S.begin(), S.end(), std::bind(&GraphStd::sortS, this, std::placeholders::_1, std::placeholders::_2));
      std::sort(C.begin(), C.end(), std::bind(&Graph::sortC, this, std::placeholders::_1, std::placeholders::_2));

      file << "S: ";
      for (auto& it : S) {
        it->deleteExtracted(C);
        file << it->getId() << " "; 
      }

      file << std::endl << "C: ";
      for (auto it : C) {
        file << it << " "; 
      }
      
      file << std::endl;
      //file << "SxC = " << C_size * S_size << endl;   
      //file << "SxC - C = " << (C_size * S_size) - C_size << endl; 
      _n_bicliques_iter += 1;
      _biclique_s_size += S_size;
      _biclique_c_size += C_size;
      _biclique_sxc_size += (S_size * C_size);

      if (_keepBicliques) {
        _savedBicliques.push_back(std::move(biclique));
      }
    }
    file.close();
    return; 
  }

  bool Graph::sortC(const uInt& a, const uInt& b)
  {
      return a < b;
  }
};
