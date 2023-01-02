#ifndef ADJ_MATRIX
#define ADJ_MATRIX

#include <iostream>
#include <fstream> 
#include <string>
#include <cstdint>
#include <vector>
#include <sstream> 
#include <chrono>
#include <algorithm>
#include "Utils.hpp"

using namespace std;

class AdjacencyMatrix{
    public:
        AdjacencyMatrix(const string);
        ~AdjacencyMatrix();

        void insert(Node*);
        uint64_t size();
        uint64_t all_edges_size();
        Node* getNode(uint64_t);
        void print();
        void makeAdjacencyList();
        void reWork();

    private:
        /*
            vars
        */
        vector<Node*> matrix; 
        string path;
        
        /*
            methods
        */
        vector<uint64_t> splitString(string, string);
        
};

#endif