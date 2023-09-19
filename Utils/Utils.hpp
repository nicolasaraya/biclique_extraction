#ifndef UTILS_HPP
#define UTILS_HPP

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
#include <map>
#include <unordered_map>
#include <math.h>
#include <thread>
#include <future>
#include <type_traits>
#include <limits.h>
#include <set>

#if defined(BITS32)
    typedef int Int;
    typedef unsigned int uInt;
    #define UINTMAX 4294967295
    #define INTMAX 2147483647
#else
    typedef long long int Int;
    typedef unsigned long long int uInt;
    #define UINTMAX ULONG_MAX
    #define INTMAX LONG_MAX
#endif


#define TIMERSTART(label)                                                           \
    std::chrono::time_point<std::chrono::high_resolution_clock> a##label, b##label; \
    a##label = std::chrono::high_resolution_clock::now();

#define TIMERSTOP(label)                                              \
    b##label = std::chrono::high_resolution_clock::now();             \
    std::chrono::duration<double> delta##label = b##label - a##label; \
    std::cout << "# elapsed time (" << #label << "): "                \
              << delta##label.count() << "s" << std::endl;

std::string now_time();
std::vector<std::string> splitString(std::string line, std::string delims);
std::unordered_map<std::string, std::string> parseArguments(int argc, char const *argv[], std::unordered_map<std::string, std::string> *);
std::string modify_path(std::string old_path, int n,std::string text);
bool validateExtension(std::string, std::string);

#endif