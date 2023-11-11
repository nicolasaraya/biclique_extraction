#!/bin/bash

g++ generator.cpp ../Utils.hpp ../Utils.cpp ../../Graph/Node.hpp ../../Graph/Node.cpp ../../Graph/GraphWeighted.hpp ../../Graph/GraphWeighted.cpp -O0 -g -o generator.out

