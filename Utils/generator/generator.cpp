#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string.h>
#include <time.h>
#include <set>
#include <algorithm>

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


int main(int argc, char const *argv[])
{
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

    return 0;
}
