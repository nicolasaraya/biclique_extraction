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

class AdjencyMatrix{
    public:
        AdjencyMatrix(const string);
        ~AdjencyMatrix();
        void insert(Node*);
        uint64_t size();
        Node* getNode(uint64_t);
        void print();
        void makeAdjencyList();
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