#ifndef SHINGLE_HPP
#define SHINGLE_HPP

#include "Utils/Utils.hpp"
#include "Graph/Node.hpp"

struct SignNodeStruct 
{
    NodePtr ptrNode;
    std::vector<uInt> minHash; 


    void print()
    {
      std::cout << ptrNode->getId() << ":";
      for (auto i : minHash) {
        std::cout << " " << i;
      }
      std::cout << std::endl;
      return;
    }
    
  
};

typedef std::unique_ptr<SignNodeStruct> SignNode; 
typedef std::vector<SignNode> Signatures;
typedef std::vector<std::unique_ptr<Signatures>> Group;

class Shingle
{
  public:
    Shingle(uint16_t, uint32_t, uint32_t);
    ~Shingle();
    SignNode computeShingle(NodePtr node);

  private:
    uint16_t num_signatures;
    uint32_t minAdyNodes;
    uint32_t shingle_size = 1;

    #ifdef BITS64
    uInt prime = (1ULL << 61ULL) - 1ULL;
    #else
    uInt prime = (1ULL << 31ULL) - 1ULL;
    #endif
  
    std::hash<std::string> hash_nodes;
    std::vector<uInt> A;
    std::vector<uInt> B;
};

#endif