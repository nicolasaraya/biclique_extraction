#ifndef BICLIQUE_HPP
#define BICLIQUE_HPP

#include <Node.hpp>
#include <Utils.hpp>

#include <memory>
#include <vector>


struct Biclique 
{
    std::vector<NodePtr>& S;
    std::vector<uInt>& C;
    std::vector<Pair>& C_w;
};

typedef std::unique_ptr<Biclique> BicliquePtr;

// class BicliquePtr {
// public:
//     explicit BicliquePtr(std::unique_ptr<Biclique> ptr) : ptr_(std::move(ptr)) {}

//     Biclique* get() const { return ptr_.get(); }
//     Biclique& operator*() const { return *ptr_; }
//     Biclique* operator->() const { return ptr_.get(); }

// private:
//     std::unique_ptr<Biclique> ptr_;
// };



#endif