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
    uint16_t lowerBound() const { return static_cast<uint16_t>(std::atoi(arguments.at(_lowerBound).c_str())); }
    uint16_t upperBound() const { return static_cast<uint16_t>(std::atoi(arguments.at(_upperBound).c_str())); }
    bool useDelta() const { return std::stoi(arguments.at(_useDelta)); }
    bool saveTxt() const { return std::stoi(arguments.at(_saveTxt)); }
    bool saveBin() const { return std::stoi(arguments.at(_saveBin)); }

    //
    void set_file(const std::string& f) { arguments[_file] = optimize() ? f : arguments[_file]; }
    void set_numSignatures(const uint16_t& value) { arguments[_numSignatures] = optimize() ? std::to_string(value) : arguments[_numSignatures]; }
    void set_minClusterSize(const uint32_t& value) { arguments[_minClusterSize] = optimize() ? std::to_string(value) : arguments[_minClusterSize]; }
    void set_bicliqueSize(const uint32_t& value) { arguments[_bicliqueSize] = optimize() ? std::to_string(value) : arguments[_bicliqueSize]; }
    void set_minAdyNodes(const uint32_t& value) { arguments[_minAdyNodes] = optimize() ? std::to_string(value) : arguments[_minAdyNodes]; }
    void set_bsDecrease(const uint16_t& value) { arguments[_bsDecrease] = optimize() ? std::to_string(value) : arguments[_bsDecrease]; }
    void set_shingleSize(const uint32_t& value) { arguments[_shingleSize] = optimize() ? std::to_string(value) : arguments[_shingleSize]; }
    void set_threshold(const uint32_t& value) { arguments[_threshold] = optimize() ? std::to_string(value) : arguments[_threshold]; }
    void set_lowerBound(const uint32_t& value) { arguments[_lowerBound] = optimize() ? std::to_string(value) : arguments[_lowerBound]; }

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
    std::string _useDelta = "useDelta";
    std::string _saveTxt = "saveTxt";
    std::string _saveBin = "saveBin";
    
    //new
    std::string _lowerBound = "lowerBound";
    std::string _upperBound = "upperBound";

    std::unordered_map<std::string, std::string> arguments {
        {_file, ""}, 
        {_numSignatures, "2"},
        {_shingleSize, "1"},

        {_minClusterSize, "50"},    //used
        {_bicliqueSize, "1000"},    //optimized
        {_minAdyNodes, "10"},       //optimized
        {_bsDecrease, "100"},       //not used in optimize
        //
        {_selfLoop, "0"}, //boolean
        {_threshold, "200"},
        {_weighted, "0"}, //boolean
        {_maxIterations, "10"},

        //
        {_optimize, "0"},  //boolean
        {_lowerBound, "80"},
        {_upperBound, "100"},
        {_useDelta, "0"},   //boolean
        {_saveTxt, "0"},   //boolean
        {_saveBin, "1"}   //boolean
    };
};

#endif