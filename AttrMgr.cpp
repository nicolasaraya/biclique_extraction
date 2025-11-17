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
      std::cout << "Argument does not exist: " << prefix + std::string(arg) << std::endl;
      continue;
    }
    arguments.at(arg) = std::string(argv[i + 1]);
  }
}

void AttrMgr::show()
{
  std::cout << "--" << _file << ": " << file() << std::endl;
  std::cout << "--" << _numSignatures << ": " << numSignatures() << std::endl;
  std::cout << "--" << _minClusterSize << ": " << minClusterSize() << std::endl;
  std::cout << "--" << _minAdyNodes << ": " << minAdyNodes() << std::endl;
  std::cout << "--" << _bicliqueSize << ": " << bicliqueSize() << std::endl;
  std::cout << "--" << _bsDecrease << ": " << bsDecrease() << std::endl;
  //std::cout << "--" << _shingleSize << ": " << shingleSize() << std::endl;
  std::cout << "--" << _selfLoop << ": " << (selfLoop() ? "true" : "false") << std::endl;

  #ifdef DEBUG_LEVEL
  std::cout << "Debug level:  " <<  DEBUG_LEVEL << std::endl;
  #endif

  std::cout << "--" << _threshold << ": " << threshold() << std::endl;
  std::cout << "--" << _maxIterations << ": " << maxIterations() << std::endl;
  std::cout << "--" << _weighted << ": " << (weighted() ? "true" : "false") << std::endl;
  std::cout << "--" << _adaptive << ": " << (adaptive() ? "true" : "false") << std::endl;
  std::cout << "--" << _useDelta << ": " << (useDelta() ? "true" : "false") << std::endl;
  std::cout << "--" << _saveTxt << ": " << (saveTxt() ? "true" : "false") << std::endl;
  std::cout << "--" << _saveBin << ": " << (saveBin() ? "true" : "false") << std::endl;
  std::cout << "--" << _lowerBound << ": " << lowerBound() << std::endl;
  std::cout << "--" << _upperBound << ": " << upperBound() << std::endl;

  std::cout << "Using: " << sizeof(Int) * 8 << " bits" << std::endl;
  #if defined(parallel)
    std::cout<< "Using: " << NUM_THREADS << " threads" << std::endl;
  #endif
}

void AttrMgr::showDescription()
{
  std::cout << "--" << _file << ": Path to the input file" << std::endl; 
  std::cout << "--" << _numSignatures << ": Number of signatures used for MinHashing" << std::endl;
  std::cout << "--" << _minClusterSize << ": Minimum cluster size required to extract a biclique" << std::endl;
  std::cout << "--" << _minAdyNodes << ": Minimum adjacent nodes considered during MinHashing" << std::endl;
  std::cout << "--" << _bicliqueSize << ": Minimum |S| x |C| size for bicliques" << std::endl;
  std::cout << "--" << _bsDecrease << ": Reduction factor applied to the biclique size" << std::endl;
  //std::cout << "--" << _shingleSize << ": Shingle size" << std::endl;
  std::cout << "--" << _selfLoop << ": Set to true to add self-loops for every node" << std::endl;

  #ifdef DEBUG_LEVEL
  std::cout << "Debug level: " << DEBUG_LEVEL << std::endl;
  #endif

  std::cout << "--" << _threshold << ": Minimum number of bicliques required before decreasing the biclique size" << std::endl;
  std::cout << "--" << _maxIterations << ": Maximum number of iterations for the algorithm"  << std::endl;
  std::cout << "--" << _weighted << ": Set to true to work with weighted graphs" << std::endl;
  std::cout << "--" << _adaptive << ": Set to true to enable adaptive parameters" << std::endl;
  std::cout << "--" << _useDelta << ": Set to true to save results using Delta16 binary format" << std::endl;
  std::cout << "--" << _saveTxt << ": Set to true to save results as plain text" << std::endl;
  std::cout << "--" << _saveBin << ": Set to true to save results in binary format" << std::endl;
  std::cout << "--" << _lowerBound << ": Lower percentile bound used when computing percentiles"  << std::endl;
  std::cout << "--" << _upperBound << ": Upper percentile bound used when computing percentiles"  << std::endl;

  std::cout << "Using: " << sizeof(Int) * 8 << " bits" << std::endl;
  #if defined(parallel)
    std::cout<< "Using: " << NUM_THREADS << " threads" << std::endl;
  #endif
}
