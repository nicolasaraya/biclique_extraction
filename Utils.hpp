#ifndef UTILS_HPP
#define UTILS_HPP

#define NUM_THREADS 8
#define DEBUG_LEVEL 3

#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <fstream> 
#include <string>
#include <cstdint>
#include <vector>
#include <sstream> 
#include <chrono>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <functional>
#include <omp.h>
#include <math.h>

#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::high_resolution_clock> a##label, b##label; \
	a##label = std::chrono::high_resolution_clock::now();


#define TIMERSTOP(label)                                                   \
    b##label = std::chrono::high_resolution_clock::now();                           \
    std::chrono::duration<double> delta##label = b##label-a##label;        \
    std::cout << "# elapsed time ("<< #label <<"): "                       \
                  << delta##label.count()  << "s" << std::endl;


std::string now_time();
std::vector<std::string> splitString(std::string line, std::string delims);


#endif 