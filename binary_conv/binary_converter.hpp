#ifndef BINARY_CONV
#define BINARY_CONV

#include <iostream>
#include <string.h>
#include <fstream>
#include <cassert>
#include <vector>
#include <bitset>

typedef long long int var;
//typedef int var;

void text2binary(const std::string path); 
void binary2text(const std::string path);

std::vector<std::string> splitString(std::string line, std::string delims);

#endif