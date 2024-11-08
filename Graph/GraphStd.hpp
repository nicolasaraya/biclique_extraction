#ifndef GRAPHSTD
#define GRAPHSTD

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <cstdint>
#include <type_traits>
#include <mutex>
#include <memory>

#include <Cluster.hpp>
#include <Shingle.hpp>
#include <Trie.hpp>
#include <Biclique.hpp>

typedef std::vector<NodePtr>::iterator GraphIterator;

class GraphStd
{
  public:
    GraphStd(std::string path): _path(path) {;}
    GraphStd() {};
    
    uint64_t size();
    uint64_t all_edges_size();
    std::string& getPath();
    void setPath(std::string path);
    void print();
    uint64_t maxValueEdge();
    bool isTransposed();
    void setCompressed(bool b);
    void printAsMatrix();
    void insert(NodePtr node);
    GraphIterator begin();
    GraphIterator end();
    void restoreNodes();
    double getAverageDegree();
    NodePtr at(uInt pos);
    NodePtr find(uInt node_id);
    NodePtr binarySearch(uInt l, uInt r, uInt node_id);
    NodePtr back();
    void sort();
    bool compareNodes(const NodePtr& a, const NodePtr& b);
    void transpose();

    void extract( uint16_t num_signatures,
                  uint16_t minClusterSize,
                  uint16_t minAdyNodes,
                  uint32_t biclique_size,
                  uint16_t bs_decrease,
                  uint32_t shingleSize,
                  uint32_t threshold,
                  uint16_t maxIterations
                );

    void compress();
    std::unique_ptr<Group> makeGroups(Signatures* group, int column);
    void computeClusters(Signatures* group, uint16_t column);
    void extractBicliques(Signatures* cluster);
    void extractBicliques(std::vector<NodePtr>* cluster);
    void getBicliques(Cluster& cluster);
    std::unique_ptr<Signatures> computeShingles();
    void printSignatures(const Signatures& group);
    void sortBySignatures(Signatures* group, const uint16_t index);
    void sortBicliques(std::vector<BicliquePtr>& bicliques);
    bool sortS(const NodePtr& a, const NodePtr& b);
    bool compareMinHash(const SignNode& a, const SignNode& b, uint16_t index);
    bool compareBicliqueRank(const BicliquePtr& a, const BicliquePtr& b);


    ~GraphStd() = default;
    virtual void buildTxt() = 0;
    virtual void buildBin() = 0;
    virtual void writeAdjacencyList() = 0;
    virtual void writeBinaryFile() = 0;
    virtual void writeBicliques(std::vector<BicliquePtr>& bicliques) = 0;

  protected:
    std::vector<NodePtr> _matrix;
    std::vector<BicliquePtr> _savedBicliques;

    std::string _path = "graph.txt";
    std::string _format;
    std::string _pathBicliques;
    std::string _pathLog;

    bool _selfLoop = false;
    bool _compressed = false;
    bool _transposed = false;
    bool _keepBicliques = true;

    uint64_t _maxEdge = 0;
    uint64_t _numNodes = 0;
    uint64_t _numEdges = 0;

    //params
    uint16_t _numSignatures;
    uint32_t _minClusterSize;
    uint32_t _bicliqueSize;
    uint32_t _minAdyNodes;
    uint16_t _bsDecrease;
    uint32_t _shingleSize;
    uint32_t _threshold;
    uint16_t _maxIterations;

    uint64_t _totalBiclique = 0;
    uint64_t _biclique_s_size = 0;
    uint64_t _biclique_c_size = 0;
    uint64_t _biclique_sxc_size = 0;
    uint16_t _min_sxc_size = 20;
    uint64_t _clusterSize = 0;
    uint64_t _n_bicliques_iter = 0;

    std::mutex _mtxWriteBiclique;
    std::mutex _mtxSignatures;    
};

#endif