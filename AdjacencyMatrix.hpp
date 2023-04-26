#ifndef ADJ_MATRIX
#define ADJ_MATRIX

#include "Node.hpp"

using namespace std;

typedef vector<Node*>::iterator AdjMatrixIterator;

class AdjacencyMatrix{
    public:
        AdjacencyMatrix(const string, bool);
        AdjacencyMatrix();
        ~AdjacencyMatrix();
        void build();
        void insert(Node*);
        uint64_t size();
        uint64_t all_edges_size();
        void print();
        void writeAdjacencyList();
        void restoreNodes();
        AdjMatrixIterator begin();
        AdjMatrixIterator end();
        AdjMatrixIterator find(Node*);
        
    private:
        vector<Node*> matrix; 
        string path;
        bool selfLoops = false; 
};

#endif