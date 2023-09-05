#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <../Utils.hpp>

using namespace std;

// Script to convert konect files to txt files without comments and weights

int main(int argc, char **argv){
    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <path>, <num skip lines> " << std::endl;
        return 1;
    }
    string path = argv[1];
    int num_skip_lines = atoi(argv[2]);
    std::cout << "path: " << path << std::endl;

    ifstream input_file;
    input_file.open(path);
    if(!input_file.is_open()){
        std::cout << "Could not open file: " << path << std::endl;
        return 1;
    }
    map<uint32_t, vector<uint32_t>> graph;
    string line;
    
    bool hasZero = false;
    uint32_t max_node = 0;

    for(int i = 0; i < num_skip_lines; ++i) getline(input_file, line); // skip lines

    while(getline(input_file, line)){
        if(line == "") break;
        vector<string> words = splitString(line, " ");
        uint32_t node = stoi(words[0]);
        uint32_t v = stoi(words[1]);

        if(node == 0 || v == 0) hasZero = true;

        if(node > max_node) max_node = node;
        if(v > max_node) max_node = v;

        if(graph.find(node) != graph.end()){
            vector<uint32_t> neighbors = graph[node];
            neighbors.push_back(v);
            graph[node] = neighbors;
        }
        else{
            vector<uint32_t> neighbors;
            neighbors.push_back(v);
            graph[node] = neighbors;
        }
    }
    input_file.close();

    ofstream output_file;
    output_file.open(path + ".txt");

    if(hasZero) max_node++;
    output_file << max_node << endl;

    for(auto it = graph.begin(); it != graph.end(); ++it){
        uint32_t node = it->first;
        vector<uint32_t> neighbors = it->second;
        if(hasZero) node++;
        output_file << node << " ";
        for(uint32_t v : neighbors){
            if(hasZero) v++;
            output_file << v << " ";
        }
        output_file << endl;
    }

    output_file.close();

    return 0;
}