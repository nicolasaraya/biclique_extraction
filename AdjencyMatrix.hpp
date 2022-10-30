#ifndef ADJ_MATRIX
#define ADJ_MATRIX

#include <iostream>
#include <fstream> 
#include <string>
#include <cstdint>
#include <vector>
#include <sstream> 

using namespace std;


class AdjencyMatrix{
    public:
        AdjencyMatrix();
        ~AdjencyMatrix();
        void loadFileTxt(const string);
    private:
        /*
            vars
        */
        uint64_t countNodes; 
        string path;
        vector<pair<uint64_t, vector<uint64_t>>> matrix; 
        
        /*
            methods
        */
        
        vector<uint64_t> splitString(string, string);

};

#endif