#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Utils.hpp"


class Cluster{
    public:
        Cluster(vector<Node*>);
        ~Cluster();

    private:
        vector<Node*> nodes;
};



#endif