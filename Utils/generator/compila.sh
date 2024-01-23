#!/bin/bash

g++ generator.cpp ../Utils.hpp ../Utils.cpp ../../Graph/Node.hpp ../../Graph/Node.cpp ../../Graph/GraphWeighted.hpp ../../Graph/GraphWeighted.cpp -O3 -o generator.out

