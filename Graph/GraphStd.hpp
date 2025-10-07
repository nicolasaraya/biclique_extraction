#ifndef GRAPHSTD_HPP
#define GRAPHSTD_HPP

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <cstdint>
#include <type_traits>
#include <mutex>
#include <memory>
#include <limits>

#include <Cluster.hpp>
#include <Shingle.hpp>
#include <Trie.hpp>
#include <BicliqueExtracted.hpp>
#include <AttrMgr.hpp>
#include <Graph/Node.hpp>

typedef std::vector<NodePtr>::iterator GraphIterator;

class GraphStd
{
  public:  
    uint64_t size();
    uint64_t all_edges_size();

    uint64_t getOriginalEdges() { return _originalNumEdges; }
    uint64_t getOriginalNodes() { return _originalNumNodes; }
    
    std::string getPath();
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

    void extract();

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

    void dfs(NodePtr node, std::unordered_set<uInt> visited, std::vector<NodePtr>& component);
    uint32_t computePercentile(float percentile);
    void showStats();

    ~GraphStd() = default;
    virtual void buildTxt() = 0;
    virtual void buildBin() = 0;
    virtual void writeAdjacencyList() = 0;
    virtual void writeBinaryFile() = 0;
    virtual void writeBicliques(std::vector<BicliquePtr>& bicliques) = 0;
    virtual void writeBicliqueBinary() = 0;


  protected:
   // GraphStd(std::string path, bool selfLoop) ;
    GraphStd() = default;

    std::vector<NodePtr> _matrix; //podria ser std::list? 
    std::vector<BicliquePtr> _savedBicliques;

    std::string _path = "graph.txt";
    std::string _format;
    std::string _pathBicliques;
    std::string _pathLog;

    bool _selfLoop = false;
    bool _compressed = false;
    bool _transposed = false;
    bool _keepBicliques = false;
    bool _weighted = false;

    uint64_t _maxEdge = 0;
    uint64_t _numNodes = 0;
    uint64_t _numEdges = 0;

    uint64_t _originalNumNodes = 0;
    uint64_t _originalNumEdges = 0;

    //stats
    double _avgDegree = 0.0;
    uint64_t _minDegree = UINT64_MAX;
    uint64_t _maxDegree = 0;
    double _density = 0.0;
    std::unordered_map<uInt, uInt> _distribution;


    //params
    #if 1 //from AttrMgr, needs a refactor
    uint16_t _numSignatures;
    uint32_t _minClusterSize;
    //uint32_t _bicliqueSize;
    //uint32_t _minAdyNodes;
    uint32_t _minMinAdyNodes;
    uint16_t _bsDecrease;
    uint32_t _shingleSize;
    uint32_t _threshold;
    uint16_t _maxIterations;
    #endif

    bool _optimize;
    bool _endOptimizations = false;
    uint16_t _concurrentOptimizations = 0;
    uint16_t _avgSxC = 0;
    float _minCompression = std::numeric_limits<float>::max();
    float _sumCompression = 0.0;
    const int _maxConcurrentOptimizations = 4;

    uint64_t _totalBiclique = 0;
    uint64_t _biclique_s_size = 0;
    uint64_t _biclique_c_size = 0;
    uint64_t _biclique_sxc_size = 0;

    uint64_t _biclique_s_sizeIter = 0;
    uint64_t _biclique_c_sizeIter = 0;
    uint64_t _biclique_sxc_sizeIter = 0;

    float _k = 0.0;

    uint16_t _min_sxc_size = 20;
    uint64_t _clusterSize = 0;
    uint64_t _n_bicliques_iter = 0;

    std::mutex _mtxWriteBiclique;
    std::mutex _mtxSignatures;

    std::ostringstream ossBicliques;
};

#endif