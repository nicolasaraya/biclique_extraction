#!/bin/bash

./biclique_extractor \
    --numSignatures 4 \
    --minClusterSize 3 \
    --bicliqueSize 5 \
    --minAdyNodes 4 \
    --shingleSize 1 \
    --bsDecrease 1 \
    --threshold 1 \
    --selfLoop 1 \
    --debug 0 \
    --iterations 10 \
    --file Utils/generator/graph_generated.txt