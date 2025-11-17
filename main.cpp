
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>

#include <Graph.hpp>
#include <GraphWeighted.hpp>
#include <Utils/Utils.hpp>
#include <AttrMgr.hpp>

int main(int argc, char const *argv[])
{
  if (argc <= 1) {
    std::cout << "No arguments provided." << std::endl;
    std::cout << "Usage: " << argv[0]
              << " --file <path> [--numSignatures 2 --minClusterSize 50 ...]" << std::endl;
    std::cout << "Available attributes:" << std::endl;
    AttrMgr::get().showDescription();
    return EXIT_FAILURE;
  }

  signal(SIGINT, utils::SigHnd::signal_handler);
  AttrMgr::get().parseArguments(argc, argv);
  AttrMgr::get().show();

  if (AttrMgr::get().weighted()) {
    Weighted::Graph g(AttrMgr::get().file());
    g.extract();
  } else {
    Boolean::Graph g(AttrMgr::get().file());
    g.writeBinaryFile();
    g.extract();
  }
  return 0;
}

// ./biclique_extractor ../data/dblp-2011.txt 2 10 2 500 400 1
//./biclique_extractor ../data/ej1.txt 2 1 2 2 400 1
// pmap pid : memory usage
