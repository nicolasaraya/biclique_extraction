#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <GraphStd.hpp>
#include <Node.hpp>

class Graph : public GraphStd
{
public:
    Graph(const std::string, bool);
    Graph(const std::string);

    void standardize(std::vector<uInt>*);
    void buildTxt();
    void buildBin();
    void writeAdjacencyList();
    void writeBinaryFile();
    std::string getPath();
    void writeBicliques(std::vector<BicliquePtr>& bicliques);
    bool sortC(const uInt& a, const uInt& b);
};
#endif