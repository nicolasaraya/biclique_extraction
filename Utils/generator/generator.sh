#!/bin/bash

g++ generator.cpp ../Utils.hpp ../Utils.cpp ../../Graph/Node.hpp ../../Graph/Node.cpp ../../Graph/NodeWeighted.hpp ../../Graph/NodeWeighted.cpp -O0 -g -o generator.out

graphNodes="100"
averageEdges="10"
numBicliques="5"
averageBicliqueSize="10"
averageWeight="10"
#greedy="0.1"

# ./generator.out $graphNodes $averageEdges $numBicliques $averageBicliqueSize $averageWeight $greedy