#include <GraphWeighted.hpp>
#include <Utils.hpp>
#include <AttrMgr.hpp>

#include <cassert>

namespace Weighted
{
  Graph::Graph(const std::string path)
  {
    _weighted = true; 
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
    //print();
  }

  void Graph::buildBin_alt()
  {
    std::ifstream file;
    file.open(_path, std::ios::in | std::ios::binary);
    file.seekg (0, file.beg);
    assert(file.is_open());

    uInt* buffer = new uInt(0);
    NodePtr temp = nullptr;
    while (not file.eof()) {
      file.read((char*)buffer, sizeof(uInt));
      uInt id = *buffer;
      file.read((char*)buffer, sizeof(uInt));
      uInt adj = *buffer;
      file.read((char*)buffer, sizeof(uInt));
      uInt w = *buffer;
      if (temp == nullptr or temp->getId() != id) {
        if (temp != nullptr) {
          insert(temp);
          temp->shrinkToFit();
          temp->sort();
        }
        temp = nullptr;
        temp = find(id);
        if(temp == nullptr) temp = std::make_unique<Node>(id,true);
      } 
      temp->addAdjacent(adj, w);
      //if (_matrix.size() > 400000 ) break;
    }
    if (temp->edgesSize() > 0) {
      //insert(temp);
      temp->shrinkToFit();
      temp->sort();
    }
    _matrix.shrink_to_fit();
    sort();
    delete buffer;
    file.close();
  }


  void Graph::buildBin()
  {
    std::ifstream file;
    file.open(_path, std::ios::in | std::ios::binary);
    file.seekg (0, file.beg);
    assert(file.is_open());

    Int* buffer = new Int(-1);
    NodePtr temp = nullptr;

    while (not file.eof()) {
      file.read((char*)buffer, sizeof(Int));
      if(*buffer < 0) {
        if (temp != nullptr) {
          temp->shrinkToFit();
          temp->sort();
        }
        temp = nullptr;
        temp = find(-(*buffer));
        if (temp == nullptr) {
          temp = std::make_unique<Node>(-(*buffer), true);
          insert(temp);
        }
      } else {
        if(file.eof()) break;
        uInt adj = *buffer;
        file.read((char*)buffer, sizeof(Int));
        uInt weight = *buffer;
        temp->addAdjacent(adj, weight);
      }
      //temp->print();
      //if (_matrix.size() > 400000 ) break;
    }
    if(temp->edgesSize() > 0) {
      temp->shrinkToFit();
      temp->sort();
      //insert(temp);
    }
    _matrix.shrink_to_fit();
    sort();
    delete buffer;
    file.close();
  }


  void Graph::buildTxt()
  {
    std::cout << "format: " << "id ady weight" << std::endl;
    std::ifstream file; 
    file.open(_path);
    assert(file.is_open());
    std::string line;
    NodePtr temp = nullptr;

    //int x = 0;
    while(getline(file,line)){
      if(line.front() == '%') continue; 
      auto content = utils::splitString(line, " ");
      if (content.size() <= 1) continue;
      uInt id = atoi(content[0].c_str());
      uInt adj = atoi(content[1].c_str());
      uInt weight = atoi(content[2].c_str());
      if(temp == nullptr or temp->getId() != id){
        if(temp != nullptr) {
          insert(temp);
          temp->shrinkToFit();
          temp->sort();
        }
        temp = nullptr;
        temp = find(id);
        if(temp == nullptr) temp = std::make_unique<Node>(id, true);
      }
      _numEdges++;
      temp->addAdjacent(adj, weight); 
      //cout << id << " " << adj << " " << weight << endl;
    }
    if(temp->edgesSize() > 0) {
      insert(temp);
      temp->shrinkToFit();
      temp->sort();
    }

    _matrix.shrink_to_fit();
    sort();
    file.close();
    _numNodes = _matrix.size();
    std::cout << "nodes: " << _numNodes << ", edges: " << _numEdges << std::endl;
    //print();
  }

  void Graph::writeAdjacencyList()
  {
    std::ofstream file;
    std::string pathFile = utils::modify_path(_path, 4 ,".txt");
    if (_compressed) {
      pathFile = utils::modify_path(_path, 4 ,"_compressed.txt");
    } 	
    std::cout << "Save: " << pathFile << std::endl;
    file.open(pathFile, std::ofstream::out | std::ofstream::trunc); 
    assert(file.is_open());
    for(auto& i : _matrix){
      for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
        file << i->getId() << " " << (*j).first << " " << (*j).second << std::endl;
      }
    }
    file.close();
  }

  void Graph::writeBinaryFile_alt()
  {
    std::ofstream file;
    std::string pathFile = utils::modify_path(_path, 4 ,".bin");
    if (_compressed) {
      pathFile = utils::modify_path(_path, 4 ,"_compressed.bin");
    }
    std::cout << "Save: " << pathFile << std::endl;
    file.open(pathFile, std::ios::out | std::ios::binary | std::ios::trunc); 
    assert(file.is_open());

    for(auto& i : _matrix){
      uInt id = i->getId();
      for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
        //file << i->getId() << " " << j->first << " " << j->second << endl;
        file.write((char*)&(id), sizeof(uInt));
        file.write((char*)&((*j).first), sizeof(uInt));
        file.write((char*)&((*j).second), sizeof(uInt));
      }
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
    std::cout << "Save: " << pathFile << std::endl;
    file.open(pathFile, std::ios::out | std::ios::binary | std::ios::trunc); 
    assert(file.is_open());

    for(auto& i : _matrix){
      if (i->edgesSize() == 0) continue; 
      Int id = -(i->getId());
      file.write((char*)&(id), sizeof(Int));
      for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
        //file << i->getId() << " " << j->first << " " << j->second << endl;
        //file.write((char*)&(id), sizeof(uInt));
        file.write((char*)&((*j).first), sizeof(uInt));
        file.write((char*)&((*j).second), sizeof(uInt));
      }
    }
    file.close();
  }

  void Graph::writeBicliques(std::vector<BicliquePtr>& bicliques)
  {   
    std::ofstream file;
    file.open(_pathBicliques, std::fstream::app);
    assert(file.is_open());
    for (auto& biclique : bicliques) {
      std::vector<NodePtr>& S = biclique->S;
      std::vector<Pair>& C_w = biclique->C_w;

      uInt S_size = S.size();
      uInt C_size = C_w.size(); 

      if(S_size * C_size < _bicliqueSize) {
        continue; 
      }

      std::sort(S.begin(), S.end(), std::bind(&GraphStd::sortS, this, std::placeholders::_1, std::placeholders::_2));
      std::sort(C_w.begin(), C_w.end(), std::bind(&Graph::sortC, this, std::placeholders::_1, std::placeholders::_2));

      for(auto& it : S){
        it->deleteExtracted(C_w);  
        file << it->getId() << " "; 
      }

      file << ";";
      for(auto it = C_w.begin(); it != C_w.end(); it++){
        file << " (" << (*it).first << "," << (*it).second << ")"; 
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

  bool Graph::sortC(const Pair& a, const Pair& b)
  {
    return a.first < b.first;
  }
};
