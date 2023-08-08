#include <iostream>
#include <fstream>

#include "../../Graph/Node.hpp"
#include "../Utils.hpp"

typedef vector<Node*> Graph;

Graph* buildGraphWeighted(string path)
{
    vector<Node*>* graph = new vector<Node*>();
    ifstream file;
	file.open(path);
	assert(file.is_open());
	string line;
	while(getline(file,line)){
		auto content = splitString(line, " ");
        uInt id = atoi(content[0].c_str());
		uInt adj = atoi(content[1].c_str());
		uInt weight = atoi(content[2].c_str());
        while (graph->size() <= id){
            graph->push_back(new Node(graph->size(), true));
        }
        graph->at(id)->addAdjacent(adj, weight);
	}
    return graph;
}

void addBicliquesWeighted(string path, Graph* graph){
    ifstream file;
    file.open(path);
    assert(file.is_open());
    string line;
    while(not file.eof()){
        getline(file, line);
        auto contentS = splitString(line, " ");
        getline(file, line);
        auto contentC = splitString(line, " ");
        for(size_t i = 1; i < contentS.size(); i++){
            uInt index = atoi(contentS[i].c_str());
            for(size_t j = 1; j < contentC.size(); j++){
                auto values = splitString(contentC[j], ",");
                values[0].erase(values[0].begin()); //delete "("
                values[1].pop_back(); //delete ")" 
                assert(not graph->at(index)->findAdjacent(atoi(values[0].c_str()), atoi(values[1].c_str()))); //no debe estar la arista en el grafo. 
                graph->at(index)->addAdjacent(atoi(values[0].c_str()), atoi(values[1].c_str()));
                //assert(not graph->at(index)->findAdjacent(atoi(values[0].c_str()), atoi(values[1].c_str()))); //no debe estar la arista en el grafo. 
            }
        }
    }
    return;
}

void compareWeighted(string path, Graph* graph)
{
    vector<string> diff; 
    ifstream file;
	file.open(path);
	assert(file.is_open());
	string line;
    uInt idPrev = -1;
    uInt countLines = 0;
	while(getline(file,line)){
        if(line.empty() or line.front() == '%') continue;
		auto content = splitString(line, " ");
        uInt id = atoi(content[0].c_str());
		uInt adj = atoi(content[1].c_str());
		uInt weight = atoi(content[2].c_str());

        if (id != idPrev) {
            graph->at(id)->sort();
        }
        if (not graph->at(id)->findAdjacent(adj, weight)) {
            diff.push_back(line);
            cout << "diferentes: " << endl; 
            cout << line << endl;
            graph->at(id)->print();
            cout << "Line: " << countLines << endl;
            return; 
        }
        idPrev = id; 
        countLines++; 
	}
    if(diff.empty()) cout << "iguales" << endl;
}

int main(int argc, char const *argv[])
{
    if(argc != 4) return 0; 
    
    cout << "Building graph" << endl; 
    auto g = buildGraphWeighted(argv[1]);
    cout << "Adding bicliques" << endl; 
    addBicliquesWeighted(argv[2], g); 
    cout << "Comparing" << endl;
    compareWeighted(argv[3], g);

    return 0;
}
