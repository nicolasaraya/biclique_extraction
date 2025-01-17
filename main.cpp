
#include <iostream>
#include <unistd.h>
#include <csignal>

#include <Graph.hpp>
#include <GraphWeighted.hpp>
#include <Utils/Utils.hpp>
#include <AttrMgr.hpp>


int main(int argc, char const *argv[])
{
  signal(SIGINT, utils::SigHnd::signal_handler);
  AttrMgr::get().parseArguments(argc, argv);
  AttrMgr::get().show();

  if (AttrMgr::get().weighted()) {
    weighted::Graph g(AttrMgr::get().file());
    g.extract();
  } else {
    boolean::Graph g(AttrMgr::get().file());
    g.extract();
  }
  return 0;
}

// ./biclique_extractor ../data/dblp-2011.txt 2 10 2 500 400 1
//./biclique_extractor ../data/ej1.txt 2 1 2 2 400 1
// pmap pid : memory usage
