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