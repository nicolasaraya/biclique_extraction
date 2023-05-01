#include "Utils.hpp"


std::string now_time(){
    std::time_t t = std::time(0);   // get time now
    std::tm* t_now = std::localtime(&t);
    std::string now =  std::to_string(t_now->tm_year + 1900) + '-' + std::to_string(t_now->tm_mon + 1) + '-' + std::to_string(t_now->tm_mday) + "-" + std::to_string(t_now->tm_hour) + std::to_string(t_now->tm_min) + std::to_string(t_now->tm_sec)  ;
    return now; 
}


std::vector<std::string> splitString(std::string line, std::string delims){
	std::string::size_type bi, ei;
    std::vector<std::string> words;
    bi = line.find_first_not_of(delims);     
    while(bi != std::string::npos) {
  	    ei = line.find_first_of(delims, bi);
    	if(ei == std::string::npos) ei = line.length();
		std::string aux = line.substr(bi,ei-bi);
    	words.push_back(aux.c_str());
    	bi = line.find_first_not_of(delims, ei);
  	}                 
    return words;
}

std::unordered_map<std::string, std::string> parseArguments(int argc, char* argv[]){
    std::unordered_map<std::string, std::string> arguments{
        {"file", "../data/dblp-2011.txt"},
        {"numSignatures", "2"},
        {"minClusterSize", "10"},
        {"bicliqueSize", "2"},
        {"minAdyNodes", "500"},
        {"bsDecrease", "500"},
        {"shingleSize", "1"},
        {"selfLoop", "1"},
        {"threshold", "100"}
    };

    const std::string prefix = "--"; 

    for(int i = 1; i < argc - 1; i+=2){
        auto arg = std::string(argv[i]);
        arg.erase(arg.begin());
        arg.erase(arg.begin()); 
        
        auto f = arguments.find(arg); 
        if(f == arguments.end()){
            std::cout << "No se encuentra el argumento: " << prefix + std::string(arg) << std::endl;
        }

        arguments[arg] = std::string(argv[i+1]);
        //std::cout << arg << ", " << arguments[arg] << std::endl;
    }

    return arguments; 

}
