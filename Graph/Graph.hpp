#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <GraphStd.hpp>
#include <Node.hpp>
#include <Utils.hpp>

namespace boolean
{
  class Graph : public GraphStd
  {
    public:
      Graph(const std::string, bool);
      Graph(const std::string);
      Graph() = default;

      void standardize(std::vector<uInt>*);
      void buildTxt() override;
      void buildBin() override;
      void writeAdjacencyList() override;
      void writeBinaryFile() override;
      std::string getPath();
      void writeBicliques(std::vector<BicliquePtr>& bicliques);
      bool sortC(const uInt& a, const uInt& b);
  };
};
#endif