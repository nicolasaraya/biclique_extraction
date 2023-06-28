#!/bin/bash

g++ generator.cpp -O3 -o generator.out

graphNodes="1000"
averageEdges="30"
numBicliques="30"
averageBicliqueSize="30"
averageWeight="10"
greedy="0.2"

./generator.out $graphNodes $averageEdges $numBicliques $averageBicliqueSize $averageWeight $greedy