#ifndef ATTRIBUTE_MANAGER
#define ATTRIBUTE_MANAGER

#include <cstdint>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <cstdlib>

class AttrMgr
{
  public:
    static AttrMgr& get() {
      static AttrMgr instance;
      return instance;
    }

    void parseArguments(int argc, char const *argv[]);
    void show();
    
    //accessors
    std::string file() const { return arguments.at(_file); }
    uint16_t numSignatures() const { return static_cast<uint16_t>(std::atoi(arguments.at(_numSignatures).c_str())); }
    uint32_t minClusterSize() const { return static_cast<uint32_t>(std::atoi(arguments.at(_minClusterSize).c_str())); }
    uint32_t bicliqueSize() const { return static_cast<uint32_t>(std::atoi(arguments.at(_bicliqueSize).c_str()));}
    uint32_t minAdyNodes() const { return static_cast<uint32_t>(std::atoi(arguments.at(_minAdyNodes).c_str()));}
    uint16_t bsDecrease() const { return static_cast<uint16_t>(std::atoi(arguments.at(_bsDecrease).c_str())); }
    uint32_t shingleSize() const { return static_cast<uint32_t>(std::atoi(arguments.at(_shingleSize).c_str()));}
    bool selfLoop() const { return std::stoi(arguments.at(_selfLoop)); }
    uint32_t threshold() const { return static_cast<uint32_t>(std::atoi(arguments.at(_threshold).c_str()));}
    bool weighted() const { return std::stoi(arguments.at(_weighted)); }
    uint16_t maxIterations() const { return static_cast<uint16_t>(std::atoi(arguments.at(_maxIterations).c_str())); }
    bool optimize() const { return std::stoi(arguments.at(_optimize)); }

    //
    void set_file(const std::string& f) { arguments[_file] = optimize() ? f : arguments[_file]; }
    void set_numSignatures(const uint16_t& value) { arguments[_numSignatures] = optimize() ? std::to_string(value) : arguments[_numSignatures]; }
    void set_minClusterSize(const uint32_t& value) { arguments[_minClusterSize] = optimize() ? std::to_string(value) : arguments[_minClusterSize]; }
    void set_bicliqueSize(const uint32_t& value) { arguments[_bicliqueSize] = optimize() ? std::to_string(value) : arguments[_bicliqueSize]; }
    void set_minAdyNodes(const uint32_t& value) { arguments[_minAdyNodes] = optimize() ? std::to_string(value) : arguments[_minAdyNodes]; }
    void set_bsDecrease(const uint16_t& value) { arguments[_bsDecrease] = optimize() ? std::to_string(value) : arguments[_bsDecrease]; }
    void set_shingleSize(const uint32_t& value) { arguments[_shingleSize] = optimize() ? std::to_string(value) : arguments[_shingleSize]; }
    void set_threshold(const uint32_t& value) { arguments[_threshold] = optimize() ? std::to_string(value) : arguments[_threshold]; }

    AttrMgr(const AttrMgr&) = delete;
    AttrMgr& operator=(const AttrMgr&) = delete;
  
  private:
    AttrMgr() = default;
    ~AttrMgr() = default;


    //default params id
    std::string _optimize = "optimize";
    std::string _file = "file";
    std::string _numSignatures = "numSignatures";
    std::string _minClusterSize = "minClusterSize";
    std::string _bicliqueSize = "bicliqueSize";
    std::string _minAdyNodes = "minAdyNodes";
    std::string _bsDecrease = "bsDecrease";
    std::string _shingleSize = "shingleSize";
    std::string _selfLoop = "selfLoop";
    std::string _threshold = "threshold";
    std::string _weighted = "weighted";
    std::string _maxIterations = "maxIterations";

    std::unordered_map<std::string, std::string> arguments {
        {_file, ""}, 
        {_numSignatures, "2"}, 
        {_minClusterSize, "50"}, 
        {_bicliqueSize, "1000"}, 
        {_minAdyNodes, "10"},
        {_bsDecrease, "100"},
        {_shingleSize, "1"},
        {_selfLoop, "0"}, //boolean
        {_threshold, "200"},
        {_weighted, "0"}, //boolean
        {_maxIterations, "10"},
        {_optimize, "0"}  //boolean
    };
};

#endif