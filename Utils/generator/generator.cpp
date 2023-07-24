#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string.h>
#include <time.h>
#include <set>
#include <algorithm>
#include "../Utils.hpp"
#include "../../Graph/Node.hpp"

using namespace std;

float greedy = 0;
const float perS = 0.2;
bool rndom = false;

typedef struct{
    set<unsigned> S;
    set<pair<unsigned,unsigned>> C; 
}Biclique;

void clear(){
	system("@cls||clear");
}

map<unsigned, set<pair<unsigned, unsigned>>>* generateBicliques(unsigned graphSize, unsigned numBicliques, unsigned averageBicliqueSize, unsigned averageWeight){
    srand(time(NULL));
    ofstream file; 
    file.open("bicliques_generated.txt", std::ofstream::out | std::ofstream::trunc);

    auto info = new map<unsigned, set<pair<unsigned, unsigned>>>();
    
    for(size_t i = 0; i < numBicliques; i++){
        unsigned size; 
        unsigned sizeS;
        unsigned sizeC; 

        if(not rndom){
            size = averageBicliqueSize;
            sizeS = (unsigned) size * perS;
            sizeC = size/sizeS;
            size = sizeS * sizeC;  
        } else {
            size = averageBicliqueSize + rand()%(unsigned)(averageBicliqueSize * greedy);
            size = size - rand()%(unsigned)(averageBicliqueSize * greedy);
            sizeS = (unsigned)(size * perS) + rand()%(unsigned)(size * perS);
            sizeC = (unsigned)(size/sizeS);
            size = sizeS * sizeC;
        }

        Biclique temp; 

        while(temp.S.size() < sizeS){
            int tempSize = temp.S.size();
            while(temp.S.size() == tempSize){
                unsigned id = rand()%graphSize;
                temp.S.insert(id);
            }
        }

        while(temp.C.size() < sizeC){
            int tempSize = temp.C.size();
            while(temp.C.size() == tempSize){
                unsigned ady = rand()%graphSize; //random index 
                unsigned weight;

                if(not rndom){
                    weight = averageWeight;
                } else {
                    const unsigned boostG = 3;
                    unsigned weight = averageWeight + rand()%(unsigned)(averageWeight * greedy * boostG);
                    weight = weight - rand()%(unsigned)(averageWeight * greedy * boostG);
                }
        
                temp.C.insert(make_pair(ady,weight));
            }
        }

        for(auto id : temp.S){
           if(info->count(id) > 0){
                for(auto it : temp.C){
                    info->at(id).insert(it);
                }
            }
            else{
                info->insert(make_pair(id, temp.C));
            }
            file << id;
            if(id != *temp.S.end()){
                file << " ";
            }
        }
        file << endl;
        for(auto ady : temp.C){
            file << "(" << ady.first << "," << ady.second << ")";
            if(ady != *temp.C.end()){
                file << " ";
            }
        }
        file << endl;

    }   

    file.close(); 
    return info;

}

void generateGraph(unsigned graphNodes, unsigned averageEdges, unsigned averageWeight, map<unsigned, set<pair<unsigned, unsigned>>>* info){
    ofstream file; 
    file.open("graph_generated.txt", std::ofstream::out | std::ofstream::trunc);
    clear();
    cout << "Writing file: " << endl; 

    for(size_t i = 0; i < graphNodes; i++){
        if(i % int(graphNodes/100) == 0){
            clear();
            cout << (i * 100) / graphNodes << "%" << endl;
        }

        unsigned size;
        if(not rndom){
            size = averageEdges;
        } else {
            size = averageEdges + rand()%(unsigned)(averageEdges * greedy);
            size = size - rand()%(unsigned)(averageEdges * greedy);
        }

        set<pair<unsigned, unsigned>>* adyacents; 
        auto f = info->find(i);
        if(f != info->end()){
            adyacents = &(*f).second;
        }
        else {
            adyacents = new set<pair<unsigned, unsigned>>();
        }

        while(adyacents->size() < size){
            int tempSize = adyacents->size();
            while(adyacents->size() == tempSize){
                unsigned ady = rand()%graphNodes; //random index
                unsigned weight; 
                if(not rndom){
                    weight = averageWeight;
                }
                else {
                    weight = averageWeight + rand()%(unsigned)(averageWeight * 0.5);
                    weight = weight - rand()%(unsigned)(averageWeight * 0.5);
                }
                adyacents->insert(make_pair(ady,weight));
            }

        }
        for(auto ady : *adyacents){
            file << i << " " << ady.first << " " << ady.second << endl;
        }
    }
    file.close();

}


void convertToWeighted(const string path, const int weight){
    ifstream file;
    file.open(path);

    string outPath = path;
    while(outPath.back() != '.') outPath.pop_back();
    outPath.pop_back();
    ofstream outFile;
    outFile.open(outPath + "_weighted.txt", std::ofstream::out | std::ofstream::trunc);

    assert(file.is_open());
    assert(outFile.is_open());


    string line; 

    while(getline(file, line)){
        auto adjacents = splitString(line, " ");
        if(adjacents.empty()) continue; 

        long long int id = atoi(adjacents.at(0).c_str());

        for(int i = 1; i < adjacents.size(); i++){
            outFile << id << " " << adjacents[i] << " " << weight << endl;
        }

    }
    file.close();
    outFile.close();
}

void buildNetflixTxt(const string path){
    vector<Node*> matrix;
    cout << "format: " << "id rank ady" << endl;
	ifstream file;
	file.open(path);
	assert(file.is_open());
	string line; 


    cout << "Reading file" << endl;
	long long int count = 0;
	while(getline(file, line)){
		if(line.front() < '0' and line.front() > '9') continue;
		auto content = splitString(line, " ");
		uInt id = atoi(content[0].c_str()); //id

		while(matrix.size() <= id){
			matrix.push_back(new Node(matrix.size(), true));
		}
		uInt weight = atoi(content[1].c_str()); //ratings
		uInt adj = atoi(content[2].c_str()); //movie
		(matrix.at(id))->addAdjacent(adj, weight);
		if(count++ == 6000000) break;
	}
	
    cout << "Writing File" << endl; 


    string outPath = path;
    while(outPath.back() != '.'){
        outPath.pop_back();
    }
    outPath.pop_back();
    outPath += "normalized.txt"; 
    ofstream outFile; 
    outFile.open(outPath, std::ofstream::out | std::ofstream::trunc);
    outFile << "% " << path << endl; 
    for(auto i : matrix){
        for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
            outFile << i->getId() << " " << j->first << " " << j->second << endl; 
        }
    }

}



int main(int argc, char const *argv[])
{
    if(argc == 3){
        convertToWeighted(argv[1], atoi(argv[2]));
        return 0;
    }

    if(argc == 2){
        buildNetflixTxt(argv[1]);
    }

    if(argc < 6){
        return 0;
    } 

    unsigned graphNodes = atoll(argv[1]);
    unsigned averageEdges = atoll(argv[2]);
    unsigned numBicliques = atoll(argv[3]);
    unsigned averageBicliqueSize = atoll(argv[4]);
    unsigned averageWeight = atoll(argv[5]);
    if(argc >= 7) {
        greedy = atof(argv[6]);
        rndom = true;
    }

    if(averageWeight > averageEdges){
        averageWeight = averageEdges * 0.75;
    }

    auto info = generateBicliques(  graphNodes, 
                                    numBicliques, 
                                    averageBicliqueSize, 
                                    averageWeight   );

    generateGraph(graphNodes, averageEdges, averageWeight, info);

    auto n = new Node(1);

    return 0;
}
