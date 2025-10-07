#include <AttrMgr.hpp>

#include <iostream>
#include <Utils/Utils.hpp>

void AttrMgr::parseArguments(int argc, char const *argv[])
{
  const std::string prefix = "--";

  for (int i = 1; i < argc - 1; ++i){
    auto arg = std::string(argv[i]);
    if (arg.front() != '-') {
      continue;
    }

    if(arg.front() == '-') arg.erase(arg.begin()); 
    if(arg.front() == '-') arg.erase(arg.begin()); 

    auto f = arguments.find(arg);
    if (f == arguments.end()) {
      std::cout << "No se encuentra el argumento: " << prefix + std::string(arg) << std::endl;
      continue;
    }
    arguments.at(arg) = std::string(argv[i + 1]);
  }
}

void AttrMgr::show()
{
  std::cout << "file: " << file() << std::endl;
  std::cout << "Num_signatures:  " << numSignatures() << std::endl;
  std::cout << "Min_Cluster_Size:  " << minClusterSize() << std::endl;
  std::cout << "Min_Adyacency_Nodes:  " << minAdyNodes() << std::endl;
  std::cout << "Biclique_Size:  " << bicliqueSize() << std::endl;
  std::cout << "Biclique_Size_Decrease:  " << bsDecrease() << std::endl;
  std::cout << "Shingle Size:  " << shingleSize() << std::endl;
  std::cout << "SelfLoop:  " << (selfLoop() ? "true" : "false") << std::endl;

  #ifdef DEBUG_LEVEL
  std::cout << "Debug level:  " <<  DEBUG_LEVEL << std::endl;
  #endif

  std::cout << "Threshold: " << threshold() << std::endl;
  std::cout << "Max Iterations: " << maxIterations() << std::endl;
  std::cout << "Is Weighted: " << (weighted() ? "true" : "false") << std::endl;
  std::cout << "Bits: " << sizeof(Int) * 8 << std::endl;
  std::cout << "Optimize Attributes: " << (optimize() ? "true" : "false") << std::endl;
  #if defined(parallel)
    std::cout<< "Using: " << NUM_THREADS << " threads" << std::endl;
  #endif
  std::cout << "useDelta: " << (useDelta() ? "true" : "false") << std::endl;
  std::cout << "saveTxt: " << (saveTxt() ? "true" : "false") << std::endl;
}