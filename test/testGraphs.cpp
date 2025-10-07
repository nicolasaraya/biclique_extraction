
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <zlib.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <numeric>
#include <cassert>

#include <Graph.hpp>
#include <GraphWeighted.hpp>
#include <../Utils/Utils.hpp>
#include <AttrMgr.hpp>

#include <Node.hpp>

namespace local
{
  std::vector<std::string> splitString(std::string line, std::string delims)
  {
    std::string::size_type bi, ei;
    std::vector<std::string> words;
    bi = line.find_first_not_of(delims);
    while (bi != std::string::npos)
    {
      ei = line.find_first_of(delims, bi);
      if (ei == std::string::npos)
          ei = line.length();
      std::string aux = line.substr(bi, ei - bi);
      words.push_back(aux.c_str());
      bi = line.find_first_not_of(delims, ei);
    }
    return words;
  }
};

struct LocalBiclique {
  std::vector<uInt> S;
  std::vector<uInt> C;
};

std::vector<LocalBiclique> deserializeBic(std::istream& is)
{
  std::vector<LocalBiclique> bicliques;
  while (1) {
    uInt sSize;
    if (not is.read(reinterpret_cast<char*>(&sSize), sizeof(sSize))) {
      break;
    }
    //std::cout << "S read: " << sSize << std::endl;

    LocalBiclique b;
    b.S.reserve(sSize);
    uInt value;
    while (sSize--) {
      is.read(reinterpret_cast<char*>(&value), sizeof(value));
      b.S.push_back(value);
      //std::cout << "value: " << value << std::endl;
    }

    uInt cSize;
    is.read(reinterpret_cast<char*>(&cSize), sizeof(cSize));
    //std::cout << "C read: " << cSize << std::endl;

    b.C.reserve(cSize);

    while (cSize--) {
      is.read(reinterpret_cast<char*>(&value), sizeof(value));
      b.C.push_back(value);
      //std::cout << "value: " << value << std::endl;
    }
    bicliques.push_back(b);
  }
  return bicliques;
}

std::vector<LocalBiclique> readBicliquesBin(std::string path)
{
  gzFile gz = gzopen(path.c_str(), "rb");
  if (not gz) {
    std::cout << "No se pudo abrir: " << path << std::endl;
    return std::vector<LocalBiclique>();
  }
  std::vector<char> buffer;
  constexpr size_t CHUNK = 4096;
  char temp[CHUNK];
  int bytes;
  while ((bytes = gzread(gz, temp, CHUNK)) > 0) {
      buffer.insert(buffer.end(), temp, temp + bytes);
  }
  gzclose(gz);

  std::istringstream iss(std::string(buffer.data(), buffer.size()),
                           std::ios::binary);

  return deserializeBic(iss);
}

std::vector<LocalBiclique> readBicliquesTxt(std::string path)
{
 
  std::vector<LocalBiclique> bicliques;
  std::ifstream file;
  file.open(path);
  if (not file.is_open()) {
    std::cout << "No se pudo abrir: " << path << std::endl;
    return bicliques;
  }

  std::string s;

  while (getline(file, s)) {
    LocalBiclique b;
    //auto values = utils::splitString(s, ";");
    s.erase(0, 1); //remove S 
    s.erase(0, 1); //remove :
    auto S = utils::splitString(s, " ");
    for (auto &i : S) {
      b.S.push_back(std::atoi(i.c_str()));
    }

    getline(file, s);
    s.erase(0, 1); //remove C 
    s.erase(0, 1); //remove :
    auto C = utils::splitString(s, " ");
    for (auto &i : C) {
      b.C.push_back(std::atoi(i.c_str()));
    }

    bicliques.push_back(b);

  }
  file.close();

  return bicliques;
}

//Compare files bin and txt
int main(int argc, char const *argv[])
{
  std::string name = argv[1];
  std::string nameBin = name + ".bin";
  std::string nameTxt = name + ".txt";
  std::string nameBicBin = name + "_bicliques.bin";
  std::string nameBicTxt = name + "_bicliques.txt";
  std::cout << "File bin: " << nameBin << std::endl;
  std::cout << "File txt: " << nameTxt << std::endl;
  std::cout << "File bicliques bin: " << nameBicBin << std::endl;
  std::cout << "File bicliques end: " << nameBicTxt << std::endl;
  Boolean::Graph gBin(nameBin);
  Boolean::Graph gTxt(nameTxt);

  if (gTxt.size() != gBin.size()) {
    std::cout << "Bin size: " << gBin.size() << std::endl;
    std::cout << "Graphs doesnt have the same size" << std::endl;
    std::cout << "Txt size: " << gTxt.size() << std::endl;
    return 0;
  }

  auto itBin = gBin.begin();
  auto itTxt = gTxt.begin();

  while (itBin != gBin.end() and itTxt != gTxt.end()) {
    NodePtr nodeBin = *itBin;
    NodePtr nodeTxt = *itTxt;
    if (nodeBin->getId() != nodeTxt->getId()) {
      std::cout << "Id's doesnt match" << std::endl;
      std::cout << "Bin id: " << nodeBin->getId() << std::endl;
      std::cout << "Txt id: " << nodeTxt->getId() << std::endl;

    } else if (nodeBin->edgesSize() != nodeTxt->edgesSize()) {
      std::cout << "Edges size doesnt match" << std::endl;
      std::cout << "Bin size: " << nodeBin->edgesSize() << std::endl;
      std::cout << "Txt size: " << nodeTxt->edgesSize() << std::endl;

    }
    itBin++;
    itTxt++;
  }

  std::cout << "Match Graphs" << std::endl;

  auto bicBin = readBicliquesBin(nameBicBin);
  auto bicTxt = readBicliquesTxt(nameBicTxt);

  if (bicBin.size() == 0 or bicTxt.size() == 0) {
    std::cout << "Cannot compare bicliques" << std::endl;
  } else if (bicBin.size() != bicTxt.size()) {
    std::cout << "Bicliques sets doesnt have the same size" << std::endl;
    std::cout << "Bin bicliques size: " << bicBin.size() << std::endl;
    std::cout << "Txt bicliques size: " << bicTxt.size() << std::endl;
    return 0;
  }

  auto itBBin = bicBin.begin();
  auto itBTxt = bicTxt.begin();
  
  int count =0;

  while (itBBin != bicBin.end() and itBTxt != bicTxt.end()) {
    auto& bBin = *itBBin;
    auto& bTxt = *itBTxt;
    if (bBin.S.size() != bTxt.S.size()) {
      std::cout << "S size doesnt match" << std::endl;
      std::cout << "Bin size: " << bBin.S.size() << std::endl;
      std::cout << "Txt size: " << bTxt.S.size() << std::endl;
      return 0;
    }

    if (bBin.C.size() != bTxt.C.size()) {
      std::cout << "C size doesnt match" << std::endl;
      std::cout << "Bin size: " << bBin.C.size() << std::endl;
      std::cout << "Txt size: " << bTxt.C.size() << std::endl;
      return 0;
    }
    itBBin++;
    itBTxt++;
    
    //std::cout << "it: " << count++ << std::endl;
  }

  std::cout << "Match Bicliques" << std::endl;



  return 0;
}
//original
// rawSize: 15834628
// CompSize: 2171150

//compressed
// rawSize: 8575180
// CompSize: 1990948
