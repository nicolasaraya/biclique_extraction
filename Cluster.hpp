#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <algorithm>
#include <iostream>
#include <map>

#include "Utils.hpp"

using namespace std;


class Cluster{
    public:
        Cluster(vector<Node*>*);
        ~Cluster();

        void computeHistogram();
        void printCluster();

    private:
        vector<Node*>* nodes;
};

struct edgeCmp{

    bool operator()(const Node& rpStart, const Node& rpEnd ){
        if ((rpStart.second).size() == (rpEnd.second).size()){
	        return rpStart.first < rpEnd.first;
        }
        else{
            return (rpStart.second).size() > (rpEnd.second).size();
        } 
    }
};


#endif