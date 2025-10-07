#ifndef GRAPHW_HPP
#define GRAPHW_HPP

#include <GraphStd.hpp>
#include <Node.hpp>
#include <Utils.hpp>

namespace Weighted
{
  class Graph : public GraphStd
  {
    public:
      Graph(const std::string);
      Graph() = default;
      //~GraphWeighted();
      void buildBin() override;
      void buildBin_alt();
      void buildTxt() override;
      void writeAdjacencyList();
      void writeBinaryFile();
      void writeBinaryFile_alt(); 
      void writeBicliques(std::vector<BicliquePtr>& bicliques);
      bool sortC(const Pair& a, const Pair& b);
      void writeBicliqueBinary() override;

  };
};

#endif