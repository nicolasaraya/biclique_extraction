#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Utils.h"


class Cluster{
    public:
        Cluster();
        ~Cluster();

    private:
        vector<Node> nodes;
};



#endif