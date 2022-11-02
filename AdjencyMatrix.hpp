#ifndef ADJ_MATRIX
#define ADJ_MATRIX

#include <iostream>
#include <fstream> 
#include <string>
#include <cstdint>
#include <vector>
#include <sstream> 
#include "Utils.h"

using namespace std;

class AdjencyMatrix{
    public:
        AdjencyMatrix();
        ~AdjencyMatrix();
        void insert(Node);
        void sort();
        uint64_t size();
        Node getNode(uint64_t);
    private:
        /*
            vars
        */
        vector<Node> matrix; 
        
        /*
            methods
        */
        vector<uint64_t> splitString(string, string);

};

#endif