#ifndef ADJ_MATRIXW
#define ADJ_MATRIXW

#include <GraphStd.hpp>
#include <Node.hpp>

class GraphWeighted : public GraphStd
{
  public:
    GraphWeighted(const std::string);
    //~GraphWeighted();
    void buildBin();
    void buildBin_alt();
    void buildTxt();
    void writeAdjacencyList();
    void writeBinaryFile();
    void writeBinaryFile_alt(); 
    void writeBicliques(std::vector<BicliquePtr>& bicliques);
    bool sortC(const Pair& a, const Pair& b);
};

#endif