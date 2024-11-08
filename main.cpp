
#include <iostream>
#include <unistd.h>
#include <csignal>

#include <Graph.hpp>
#include <GraphWeighted.hpp>
#include <Utils/Utils.hpp>


int main(int argc, char const *argv[])
{
  signal(SIGINT, utils::SigHnd::signal_handler);

  std::unordered_map<std::string, std::string> input_arguments {
    {"file", "../data/dblp-2011.txt"},
    {"numSignatures", "2"},
    {"minClusterSize", "50"},
    {"bicliqueSize", "1000"},
    {"minAdyNodes", "10"},
    {"bsDecrease", "100"},
    {"shingleSize", "1"},
    {"selfLoop", "0"},
    {"threshold", "200"},
    {"debug", "0"},
    {"weighted", "0"},
    {"iterations", "10"}
  };

  auto arguments = utils::parseArguments(argc, argv, &input_arguments);

  #if defined(parallel)
    std::cout<<"Using: " << NUM_THREADS << " threads" << std::endl;
  #endif
  
  std::cout << "file: " << arguments["file"] << std::endl;
  std::cout << "Num_signatures:  " << arguments["numSignatures"] << std::endl;
  std::cout << "Min_Cluster_Size:  " << arguments["minClusterSize"] << std::endl;
  std::cout << "Min_Adyacency_Nodes:  " << arguments["minAdyNodes"] << std::endl;
  std::cout << "Biclique_Size:  " << arguments["bicliqueSize"] << std::endl;
  std::cout << "Biclique_Size_Decrease:  " << arguments["bsDecrease"] << std::endl;
  std::cout << "Shingle Size:  " << arguments["shingleSize"] << std::endl;
  std::cout << "SelfLoop:  " << arguments["selfLoop"] << std::endl;
  std::cout << "Debug flag:  " << arguments["debug"] << std::endl;
  std::cout << "Threshold: " << arguments["threshold"] << std::endl;
  std::cout << "Iterations: " << arguments["iterations"] << std::endl;
  std::cout << "Weighted: " << arguments["weighted"] << std::endl;
  std::cout << "Bits: " << sizeof(Int) * 8 << std::endl;

  
  if (not atoi(arguments["weighted"].c_str())) {
    boolean::Graph g(arguments["file"], atoi(arguments["selfLoop"].c_str()));
    g.extract(
      atoi(arguments["numSignatures"].c_str()),
      atoi(arguments["minClusterSize"].c_str()),
      atoi(arguments["minAdyNodes"].c_str()),
      atoi(arguments["bicliqueSize"].c_str()),
      atoi(arguments["bsDecrease"].c_str()),
      atoi(arguments["shingleSize"].c_str()),
      atoi(arguments["threshold"].c_str()),
      atoi(arguments["iterations"].c_str())
      );
  } else {
    weighted::Graph g(arguments["file"]);
    g.extract(
      atoi(arguments["numSignatures"].c_str()),
      atoi(arguments["minClusterSize"].c_str()),
      atoi(arguments["minAdyNodes"].c_str()),
      atoi(arguments["bicliqueSize"].c_str()),
      atoi(arguments["bsDecrease"].c_str()),
      atoi(arguments["shingleSize"].c_str()),
      atoi(arguments["threshold"].c_str()),
      atoi(arguments["iterations"].c_str())
      );
  }

    
    return 0;
}

// ./biclique_extractor ../data/dblp-2011.txt 2 10 2 500 400 1
//./biclique_extractor ../data/ej1.txt 2 1 2 2 400 1
// pmap pid : memory usage
