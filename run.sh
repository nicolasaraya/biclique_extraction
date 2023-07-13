#!/bin/bash

./biclique_extractor \
    --numSignatures 4 \
    --minClusterSize 10 \
    --bicliqueSize 100 \
    --minAdyNodes 4 \
    --shingleSize 1 \
    --bsDecrease 20 \
    --threshold 100 \
    --selfLoop 1 \
    --debug 0 \
    --iterations 10 \
    --file Utils/generator/graph_generated.txt